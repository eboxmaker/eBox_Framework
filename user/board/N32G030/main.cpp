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
#include "oled_ssd1306_128x32.h"


/**
  *	1	此例程演示了timer定时中断，使用tim1
  *	2	定时器频率为1hz，每秒中断1次
  * 3   表现为LED闪烁
  */

/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"Timer interrupt example"
#define EXAMPLE_DATE	"2018-08-08"

Timer timer1(TIM6);
OledSSD1306_128x32 oled(&mcuI2c1);
//OledSSD1306_128x32 oled(&sI2c1);

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
    LED1.mode(OUTPUT_PP);

    timer1.begin(1);
    //    timer1.attach(t2it);
    timer1.attach(&test, &Test::event);
    timer1.interrupt(ENABLE);
    timer1.start();
    oled.begin(I2c::K400);
    oled.fill_screen(0xff);
    oled.flush();

}


int main(void)
{
    setup();
    while(1)
    {
        LED1.toggle();
        delay_ms(1000);
    }


}








