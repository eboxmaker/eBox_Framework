/*
file   : core.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/


#include "common.h"
#include "ebox.h"

void 	Init_ADC1(void);

 __IO uint32_t millisSeconds;
 
#if USE_OS 
	#include "cpu.h"
	#define SysTickOverFlowValue (9000000/OS_TICKS_PER_SEC)//SysTickOverFlowValue取值范围（100-9000），主频为72Mhz的情况下

	void eBoxInit(void)
	{
		Init_ADC1();
		
		NVIC_PriorityGroupConfig(NVIC_GROUP_CONFIG);

		//将pb4默认设置为IO口，仅用jtag
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
		 GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);

	}

#else

	#define SysTickOverFlowValue 9000//此值取值范围（100-9000），主频为72Mhz的情况下

	void eBoxInit(void)
	{
		SysTick_Config(SysTickOverFlowValue);//  每隔 (nhz/9,000,000)s产生一次中断
		SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);//9Mhz的systemticks clock；
		Init_ADC1();
		
		NVIC_PriorityGroupConfig(NVIC_GROUP_CONFIG);

		//将pb4默认设置为IO口，仅用jtag
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
		 GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);

	}
#endif

uint32_t millis( void )
{
  return millisSeconds;
}

void delay_ms(uint32_t ms)
{	 	
	uint32_t end = millis() + ms*(9000/SysTickOverFlowValue);
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
	uint16_t count;
	if(count == 0)return;
	
	count = us * 9;
	if(count>SysTickOverFlowValue - 1)count = SysTickOverFlowValue-1;
	noInterrupts();
	if(systick < count)
	{
		count  = ((SysTickOverFlowValue-5)  + systick - count);///
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

#if USE_OS

#else
extern "C"{
void SysTick_Handler(void)
{

	millisSeconds++;

}
}
#endif
