/**
  ******************************************************************************
  * @file   : *.cpp
  * @author : shentq
  * @version: V1.2
  * @date   : 2016/08/14

  * @brief   ebox application example .
  *
  * Copyright 2016 shentq. All Rights Reserved.         
  ******************************************************************************
 */




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
    uart1.begin(115200);
    led.begin();
    hsl.h = 1;
    hsl.s = 1;
    hsl.l = 0.5;

    hsv.h = 2;
    hsv.s = 1;
    hsv.v = 0.5;
}

float x;
uint16_t y;

int main(void)
{
    setup();

    while(1)
    {
//方法1：        
    led.color_rgb(0,0,255);
    delay_ms(100);
    led.color_rgb(0,255,0);
    delay_ms(100);
    led.color_rgb(255,0,0);
    delay_ms(100);
        
//方法2：
//        hsv.h++;
//        if(hsv.h>360)hsv.h = 0;
//        led.color_hsv(hsv);
//        delay_ms(10);

//方法3：
//        hsl.h++;
//        if(hsl.h > 360)hsl.h = 0;
//        led.color_hsl(hsl);
//        delay_ms(10);
        
        


    }

}




