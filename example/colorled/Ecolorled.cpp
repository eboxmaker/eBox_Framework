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

COLORLED led;
void setup()
{
	ebox_init();
	uart1.begin(9600);
		hsl.s = 0.5;
		hsl.l = 0.5;
	
		hsv.h = 2;
		hsv.s = 0.9;
		hsv.v = 0.5;

}
int main(void)
{
	setup();
	while(1)
	{	
		hsl.h += 1;
		if(hsl.h>360)hsl.h=0;		
//		hsl.saturation ++;
//		if(hsl.saturation>100)hsl.saturation=0;
//		hsl.luminance ++;
//		if(hsl.luminance>100)hsl.saturation=0;		
		hsv.h++;
		if(hsv.h>360)hsv.h = 0;		
//		hsv.s+=0.01;
//		if(hsv.s>1)hsv.s = 0;		
//		hsv.v+=0.01;
//		if(hsv.v>1)hsv.v = 0;		

		led.color_hsv(hsv);
//		led.color_hsl(hsl);
		
		delay_ms(10);

	}

}





