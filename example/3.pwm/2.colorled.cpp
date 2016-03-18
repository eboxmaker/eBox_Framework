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
#include "colorled.h"

COLOR_HSL hsl;
COLOR_RGB rgb;
COLOR_HSV hsv;

COLORLED led(&PB7, &PB8, &PB9);

void setup()
{
    ebox_init();
    uart1.begin(9600);
    led.begin();
    hsl.s = 0.5;
    hsl.l = 0.5;

    hsv.h = 2;
    hsv.s = 0.5;
    hsv.v = 0.5;
}

float x;
uint16_t y;

int main(void)
{
    setup();

    while(1)
    {
        hsl.h += 1;
        if(hsl.h > 360)hsl.h = 0;
        led.color_hsl(hsl);
        //        led.color_rgb(0,0,255);
        //        led.color_rgb(0,255,0);
        //        led.color_rgb(255,0,0);
        delay_ms(10);

        //		hsv.h++;
        //		if(hsv.h>360)hsv.h = 0;
        //		led.color_hsv(hsv);
        //		delay_ms(10);
    }

}




