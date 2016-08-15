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

Iwdg dog;


void setup()
{
    ebox_init();
    uart1.begin(9600);
    uart1.printf("reset !!!\r\n");
    dog.begin(2000);
}

int main(void)
{

    setup();

    while(1)
    {
        dog.feed();//在常见的应用中喂狗应该在定时器中断中定时喂狗
        uart1.printf("running!\r\n");
        delay_ms(1000);
        //        delay_ms(1900);
        //        delay_ms(2000);
        //        delay_ms(3000);
    }
}




