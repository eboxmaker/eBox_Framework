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
char buf[16];
int main(void)
{
    setup();
    for(int i = 0; i < 16;i++)buf[i] = '1';
    while(1)
    {
        //uart1.printf("hello World !\r\n");
        uart1.printf_length(buf,1);
        delay_ms(1000);
    }
}




