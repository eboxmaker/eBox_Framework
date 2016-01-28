/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox


#include "ebox.h"
void setup()
{
    ebox_init();
    uart1.begin(9600);	
}
int main(void)
{
    setup();
    while(1)
    {
        uart1.printf("random=%d\r\n",random());
        uart1.printf("random=%d\r\n",random(100));
        uart1.printf("random=%d\r\n",random(100,65535));
        delay_ms(1000);
    }
}




