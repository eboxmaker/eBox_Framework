/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox


#include "ebox.h"
#include "math.h"
#include "ws2812.h"

WS2812 led(&PB0);


void setup()
{
    ebox_init();
    uart1.begin(9600);
    led.begin();

}
int main(void)
{
    setup();
    while(1)
    {
        led.rainbow_Loop();

    }

}
