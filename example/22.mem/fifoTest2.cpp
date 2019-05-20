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
	*   使用ebox_fifo_alloc进行初始化。使用动态内存申请空间。注意使用完
    *   要进行释放
	*/

/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"EventGPIO&EventVar example"
#define EXAMPLE_DATE	"2019-05-17"

Timer timer2(TIM2);
ebox_fifo *fifo;

uint8_t rx[100];
uint8_t source[100];

void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);
    //event_io_1.begin(1);
    
    fifo = ebox_fifo_alloc(128);//此处必须是2的n次方
    for(int i = 0; i < 100; i++)
        source[i] = i;
    ebox_fifo_put(fifo,source,100);

}
int len;
int main(void)
{
    setup();

    while(1)
    {
        len = ebox_fifo_get(fifo,rx,100);
        if(len > 0)
        {
            UART.print("len:");
            UART.println(len);
            UART.println("data:");
            for(int i = 0; i < len; i++)
                UART.println(rx[i]);
        }
        ebox_fifo_free(fifo);
    }
}
