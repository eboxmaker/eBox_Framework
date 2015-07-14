/*
file   : core.c
author : shentq
version: V1.0
date   : 2015/7/5

Copyright (c) 2015, eBox by shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/


#include "common.h"

void 	Init_ADC1(void);

 __IO uint32_t millisSeconds;

void eBoxInit(void)
{
	SysTick_Config(9000);//  每隔 (nhz/9,000,000)s产生一次中断
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);//9Mhz的systemticks clock；
	Init_ADC1();
	
	NVIC_PriorityGroupConfig(NVIC_GROUP_CONFIG);

	//将pb4默认设置为IO口，仅用jtag
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	 GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);

}
uint32_t millis( void )
{
  return millisSeconds;
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
	if(count>8999)count =8999;
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
		millisSeconds++;//矫正毫秒计数
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
extern "C"{
void SysTick_Handler(void)
{

	millisSeconds++;

}
}
