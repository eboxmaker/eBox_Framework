/*
file   : ebox.c
author : shentq
version: V1.0
date   : 2015/7/5
brief  : This file provides time service , analog,digital,interrupt functions , and so on.

Copyright (c) 2015, eBox by shentqlf@163.com. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentqlf@163.com.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentqlf@163.com assumes no responsibility for any errors contained herein.
*/


#include "ebox.h"

#ifdef __cplusplus
extern "C" {
#endif

extern  __IO uint32_t TimingMillis;

void eBoxInit(void)
{
	SysTick_Config(9000);//  每隔 (nhz/9,000,000)s产生一次中断
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);//9Mhz的systemticks clock；
	Init_ADC1();
}
uint32_t millis( void )
{
  return TimingMillis;
}

uint32_t micros( void )
{
	return 0;
}

void delay_ms(uint32_t ms)
{	 	
	uint32_t end = millis() + ms;
  uint32_t systick = SysTick->VAL;

	while (millis() < end) {
		;
		}
	while(SysTick->VAL > systick)
		{
			;
		}
}   


    								   
void delay_us(uint16_t us)
{		 
	uint32_t systick = SysTick->VAL;
	uint16_t count = (us)*9;
		if(count == 0)return;
	noInterrupts();
	if(systick < count)
	{
		count  = (8995  + systick - count);///
		while(SysTick->VAL <= count)
		{
			;
		}
		if(count == 0)
			count = 1;

		while(SysTick->VAL > count)
		{
			;
		}
	}
	else
	{
		count  =  systick - count;
		if(count == 0)
			count = 1; 
		while(SysTick->VAL > count)
		{
			;
		}
	}
	Interrupts();
}
void delayus(uint32_t us)
{	 		
	int i;
	 for(i=0; i<11*us; i++){
							;
		}
} 


#ifdef __cplusplus
}
#endif
