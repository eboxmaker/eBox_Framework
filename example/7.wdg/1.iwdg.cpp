/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox


/*
一个独立看门狗的示例程序
*/
#include "ebox.h"
#include "bsp_ebox.h"
/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"independent wdg example"
#define EXAMPLE_DATE	"2018-08-08"

Iwdg dog;


void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME,EXAMPLE_DATE);

    UART.printf("reset !!!\r\n");
    dog.begin(26208);
}

int main(void)
{

    setup();

    while(1)
    {
        dog.feed();//在常见的应用中喂狗应该在定时器中断中定时喂狗
        UART.printf("running!\r\n");
        delay_ms(25000);
        //        delay_ms(1900);
        //        delay_ms(2000);
        //        delay_ms(3000);
    }
}




