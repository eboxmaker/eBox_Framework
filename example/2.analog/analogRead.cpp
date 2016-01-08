/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox
#include "ebox.h"
int16_t x;
void setup()
{
    ebox_init();
    uart1.begin(9600);
    PA7.mode(AIN);
}

int main(void)
{
    setup();
    while(1)
    {
        x = analog_read(&PA7);
        uart1.printf("hex = %05d\r\n",x);
        x = analog_read_voltage(&PA7);
        uart1.printf("val = %04dmv\r\n",x);
        uart1.printf("==============\r\n",x);
        delay_ms(1000);
    }
}




