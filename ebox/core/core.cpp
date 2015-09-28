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
	#include "os.h"
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
	#if USE_OS 
		OS_TimeDelay(ms);
	#else
		uint32_t end = millis() + ms*(9000/SysTickOverFlowValue);
		uint32_t systick = SysTick->VAL;

		while (millis() < end) {
			;
			}
		while(SysTick->VAL > systick)
			{
				;
			}

	#endif
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

extern "C"{
	
#if USE_OS

#else
	
	void SysTick_Handler(void)//systick中断
	{

		millisSeconds++;

	}

	void PendSV_Handler(void)//操作系统中切换任务使用的中断
	{
	}

#endif

//STM32其他中断////////////////////////////
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
}
