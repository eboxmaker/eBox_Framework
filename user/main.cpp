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
#include "color_convert.h"

PWM g(&PA0,1000);
PWM r(&PA1,1000);
PWM b(&PA2,1000);
COLOR_HSL hsl;
COLOR_RGB rgb;
COLOR_HSV hsv;

//STM32 RUN IN eBox

void setup()
{
	ebox_init();
	uart1.begin(9600);
	hsl.h = 0;//色相
	hsl.s = 1;//饱和度
	hsl.l = 0.5;//亮度
	
	hsv.h = 93;//色相
	hsv.s = 0.92;//饱和度
	hsv.v = 0.94;//亮度
	
	rgb.r = 76;
	rgb.g = 57;
	rgb.b = 53;
}
int main(void)
{
	setup();
	
	while(1)
	{	
//		hsl.hue += 1;
//		if(hsl.hue>360)hsl.hue=0;		
//		hsl.saturation ++;
//		if(hsl.saturation>100)hsl.saturation=0;
//		hsl.luminance ++;
//		if(hsl.luminance>100)hsl.saturation=0;		
//		HSLtoRGB(&hsl,&rgb);
		
		hsv.h++;
		if(hsv.h>360)hsv.h = 0;		
//		hsv.s+=0.01;
//		if(hsv.s>1)hsv.s = 0;		
//		hsv.v+=0.01;
//		if(hsv.v>1)hsv.v = 0;		
			HSV_to_RGB(hsv,rgb);
//		RGB_to_HSV(rgb,hsv);
				r.set_duty(rgb.r*3);
				g.set_duty(rgb.g*3);
				b.set_duty(rgb.b*3);
				delay_ms(10);
			
	
		
	}

}




