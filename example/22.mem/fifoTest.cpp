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
#include "fifo.h"
#include "bsp_ebox.h"

/**
	*	1	此例程演示了如何使用fifo。
	*   使用ebox_fifo_init进行初始化。使用现有的存储空间。
    *   
	*/

/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"EventGPIO&EventVar example"
#define EXAMPLE_DATE	"2019-05-17"

Timer timer2(TIM2);
uint8_t buf[128];//此处必须是2的n次方
ebox_fifo *fifo;

uint8_t rx[100];
void t2it()
{

    static uint8_t i = 0;
    i++;
    ebox_fifo_put(fifo,&i,1);
}
void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);
    //event_io_1.begin(1);
    fifo = ebox_fifo_init(buf,128);//此处必须是2的n次方
    
    timer2.begin(10);
    timer2.attach(t2it);
    timer2.interrupt(ENABLE);
    timer2.start();
    UART.printf("\r\ntimer clock       = %dMhz", timer2.get_timer_source_clock() / 1000000);
    UART.printf("\r\nmax interrupt frq = %dKhz", timer2.get_max_frq() / 1000);
}
int len;
int main(void)
{
    setup();

    while(1)
    {
        len = ebox_fifo_get(fifo,rx,10);
        if(len > 0)
        {
            UART.print("len:");
            UART.println(len);
            UART.print("data:");
            for(int i = 0; i < len; i++)
                UART.println(rx[i]);
        }
        delay_ms(385);
    }
}
