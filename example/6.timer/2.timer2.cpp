/**
  ******************************************************************************
  * @file    .cpp
  * @author  shentq
  * @version V1.2
  * @date    2016/08/14
  * @brief
  ******************************************************************************
  * @attention
  *
  * No part of this software may be used for any commercial activities by any form
  * or means, without the prior written consent of shentq. This specification is
  * preliminary and is subject to change at any time without notice. shentq assumes
  * no responsibility for any errors contained herein.
  * <h2><center>&copy; Copyright 2015 shentq. All Rights Reserved.</center></h2>
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/

#include "ebox.h"
#include "bsp_ebox.h"

/**
  *	1	此例程演示了timer定时中断，使用tim2
  *	2	定时器频率为1hz，每秒中断1次
  * 3   表现为LED闪烁
  */

/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"Timer interrupt example"
#define EXAMPLE_DATE	"2018-08-08"

Timer timer2(TIM2);

void t2it()
{
    LED1.toggle();
}
class Test
{
public:
    void event()
    {
        LED1.toggle();
    }
};
Test test;

void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);

    LED1.mode(OUTPUT_PP);

    timer2.begin(1);
    timer2.attach(t2it);
    //    timer2.attach(&test,&Test::event);
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

    }


}








