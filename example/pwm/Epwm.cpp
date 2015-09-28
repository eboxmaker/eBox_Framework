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
USART uart1(USART1,&PA9,&PA10);

PWM pwm1(&PB8,1000);

//STM32 RUN IN eBox

void setup()
{
	eBoxInit();
	uart1.begin(9600);
}

float x;
uint16_t y;

int main(void)
{
	setup();
	
	while(1)
	{		 	
		x = x + PI*0.01;
		if(x >= PI)x=0;
		y = 2000 - (sin(x)+1)*1000;
		
		pwm1.setDuty(y);
		delay_ms(10);
	}

}




