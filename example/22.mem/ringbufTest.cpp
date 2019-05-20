/**
  ******************************************************************************
  * @file   : *.cpp
  * @author : shentq
  * @version: V1.2
  * @date   : 2016/08/14

  * @brief   ebox application example .
	*					 2018-8-2	通过引入bsp，定义硬件端口，方便例程可以在不同平台上运行
  * Copyright 2016 shentq. All Rights Reserved.
  ******************************************************************************
 */


#include "ebox.h"
#include "ringbuf.h"
#include "bsp_ebox.h"

/**
	*	1	此例程演示了如何使用RingBufxx类
	*
	*/

/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"EventGPIO&EventVar example"
#define EXAMPLE_DATE	"2019-05-17"

Timer timer2(TIM2);
RingBufUint8 ring;

void t2it()
{

    static uint8_t i = 0;
    i++;
    ring.write(i);
}
void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);
    //event_io_1.begin(1);
    ring.begin(100);
    
    timer2.begin(10);
    timer2.attach(t2it);
    timer2.interrupt(ENABLE);
    timer2.start();
    UART.printf("\r\ntimer clock       = %dMhz", timer2.get_timer_source_clock() / 1000000);
    UART.printf("\r\nmax interrupt frq = %dKhz", timer2.get_max_frq() / 1000);
}

int main(void)
{
    setup();

    while(1)
    {
        if(ring.available())
        {
            UART.println(ring.read());
        }
        
    }
}
