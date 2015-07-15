/*
file   : exti.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright (c) 2015, eBox by shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/

#include "exti.h"

#define EXTI_LINE_NUM 16

callbackFun extiCallbackTable[EXTI_LINE_NUM];


//  trigger	:
// 				 	EXTI_Trigger_Rising 
//  				EXTI_Trigger_Falling  
//  				EXTI_Trigger_Rising_Falling 

EXTIx::EXTIx(GPIO* EXTIPin, EXTITrigger_TypeDef trigger)
{

	
//	_pin = pin;
//	_PortSource = pinToExti[_pin].portSource;
//	_PinSource  = pinToExti[_pin].pinSource;
//	_ExtiLine		= pinToExti[_pin].extiLine;
//	_irq				= pinToExti[_pin].irqch;

	EXTIPin->mode(INPUT);
	initInfo(EXTIPin);

	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	EXTI_InitTypeDef EXTI_InitStructure;
	/* EXTI line(PB0) mode config */
	GPIO_EXTILineConfig(PortSource, PinSource); 
  EXTI_InitStructure.EXTI_Line = ExtiLine;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = trigger; //下降沿中断
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure); 
	
	
	 NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_GROUP_CONFIG);//使用全局控制值
  
  /* 配置P[A|B|C|D|E]0为中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = irq;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
void EXTIx::initInfo(GPIO* EXTIPin)
{
	switch((uint32_t)EXTIPin->port)
	{
		case (uint32_t)GPIOA: PortSource = GPIO_PortSourceGPIOA;break;
		case (uint32_t)GPIOB: PortSource = GPIO_PortSourceGPIOB;break;
		case (uint32_t)GPIOC: PortSource = GPIO_PortSourceGPIOC;break;
		case (uint32_t)GPIOD: PortSource = GPIO_PortSourceGPIOD;break;
		case (uint32_t)GPIOE: PortSource = GPIO_PortSourceGPIOE;break;
		case (uint32_t)GPIOF: PortSource = GPIO_PortSourceGPIOF;break;
	}
	switch(EXTIPin->pin)
	{
		case GPIO_Pin_0:
			PinSource = GPIO_PinSource0;ExtiLine = EXTI_Line0;irq = EXTI0_IRQn;
			break;
		
		case GPIO_Pin_1:
			PinSource = GPIO_PinSource1;ExtiLine = EXTI_Line1;irq = EXTI1_IRQn;
			break;
		
		case GPIO_Pin_2:
			PinSource = GPIO_PinSource2;ExtiLine = EXTI_Line2;irq = EXTI2_IRQn;
			break;
		
		case GPIO_Pin_3:
			PinSource = GPIO_PinSource3;ExtiLine = EXTI_Line3;irq = EXTI3_IRQn;
			break;
		
		case GPIO_Pin_4:
			PinSource = GPIO_PinSource4;ExtiLine = EXTI_Line4;irq = EXTI4_IRQn;
			break;
		
		case GPIO_Pin_5:
			PinSource = GPIO_PinSource5;ExtiLine = EXTI_Line5;irq = EXTI9_5_IRQn;
			break;
		
		case GPIO_Pin_6:
			PinSource = GPIO_PinSource6;ExtiLine = EXTI_Line6;irq = EXTI9_5_IRQn;
			break;
		
		case GPIO_Pin_7:
			PinSource = GPIO_PinSource7;ExtiLine = EXTI_Line7;irq = EXTI9_5_IRQn;
			break;
		
		case GPIO_Pin_8:
			PinSource = GPIO_PinSource8;ExtiLine = EXTI_Line8;irq = EXTI9_5_IRQn;
			break;
		
		case GPIO_Pin_9:
			PinSource = GPIO_PinSource9;ExtiLine = EXTI_Line9;irq = EXTI9_5_IRQn;
			break;
		
		case GPIO_Pin_10:
			PinSource = GPIO_PinSource10;ExtiLine = EXTI_Line10;irq = EXTI15_10_IRQn;
			break;
		
		case GPIO_Pin_11:
			PinSource = GPIO_PinSource11;ExtiLine = EXTI_Line11;irq = EXTI15_10_IRQn;
			break;
		
		case GPIO_Pin_12:
			PinSource = GPIO_PinSource12;ExtiLine = EXTI_Line12;irq = EXTI15_10_IRQn;
			break;
		
		case GPIO_Pin_13:
			PinSource = GPIO_PinSource13;ExtiLine = EXTI_Line13;irq = EXTI15_10_IRQn;
			break;
		
		case GPIO_Pin_14:
			PinSource = GPIO_PinSource14;ExtiLine = EXTI_Line14;irq = EXTI15_10_IRQn;
			break;
		
		case GPIO_Pin_15:
			PinSource = GPIO_PinSource15;ExtiLine = EXTI_Line15;irq = EXTI15_10_IRQn;
			break;
		
	}

}

void EXTIx::attachInterrupt(void (*callbackFun)(void))
{
		extiCallbackTable[PinSource] = callbackFun;
}
extern "C" {
	
	void EXTI0_IRQHandler(void)
	{
		if(EXTI_GetITStatus(EXTI_Line0) != RESET) 
		{
			extiCallbackTable[0]();
			EXTI_ClearITPendingBit(EXTI_Line0);    
		}  	
	}
	void EXTI1_IRQHandler(void)
	{
		if(EXTI_GetITStatus(EXTI_Line1) != RESET)
		{
			extiCallbackTable[1]();

			EXTI_ClearITPendingBit(EXTI_Line1);    
		}  	
	}
	void EXTI2_IRQHandler(void)
	{
		if(EXTI_GetITStatus(EXTI_Line2) != RESET) 
		{

			extiCallbackTable[2]();

			EXTI_ClearITPendingBit(EXTI_Line2);    
		}  	
	}
	void EXTI3_IRQHandler(void)
	{
		if(EXTI_GetITStatus(EXTI_Line3) != RESET) 
		{
			extiCallbackTable[3]();
			EXTI_ClearITPendingBit(EXTI_Line3);     
		}  	
	}
	void EXTI4_IRQHandler(void)
	{
		if(EXTI_GetITStatus(EXTI_Line4) != RESET) 
		{
			extiCallbackTable[4]();
			EXTI_ClearITPendingBit(EXTI_Line4);     
		}  	
	}

	void EXTI9_5_IRQHandler(void)
	{
		if(EXTI_GetITStatus(EXTI_Line5) != RESET) 
		{
			extiCallbackTable[5]();
			EXTI_ClearITPendingBit(EXTI_Line5);     
		}   
		if(EXTI_GetITStatus(EXTI_Line6) != RESET) 
		{
			extiCallbackTable[6]();
			EXTI_ClearITPendingBit(EXTI_Line6);    
		}  
		if(EXTI_GetITStatus(EXTI_Line7) != RESET) 
		{
			extiCallbackTable[7]();
			EXTI_ClearITPendingBit(EXTI_Line7);     
		}   
		if(EXTI_GetITStatus(EXTI_Line8) != RESET) 
		{
			extiCallbackTable[8]();
			EXTI_ClearITPendingBit(EXTI_Line8);    
		}	
		if(EXTI_GetITStatus(EXTI_Line9) != RESET) 
		{
			extiCallbackTable[9]();
			EXTI_ClearITPendingBit(EXTI_Line9);    
		}   

	}

	void EXTI15_10_IRQHandler(void)
	{
		if(EXTI_GetITStatus(EXTI_Line10) != RESET) 
		{
			extiCallbackTable[10]();
			EXTI_ClearITPendingBit(EXTI_Line10);     
		}   
		if(EXTI_GetITStatus(EXTI_Line11) != RESET) 
		{
			extiCallbackTable[11]();
			EXTI_ClearITPendingBit(EXTI_Line11);     
		}  
		if(EXTI_GetITStatus(EXTI_Line12) != RESET) 
		{
			extiCallbackTable[12]();
			EXTI_ClearITPendingBit(EXTI_Line12);     
		}   
		if(EXTI_GetITStatus(EXTI_Line13) != RESET) 
		{
			extiCallbackTable[13]();
			EXTI_ClearITPendingBit(EXTI_Line13);     
		}	
		if(EXTI_GetITStatus(EXTI_Line14) != RESET) 
		{
			extiCallbackTable[14]();
			EXTI_ClearITPendingBit(EXTI_Line14);     
		}   
		if(EXTI_GetITStatus(EXTI_Line15) != RESET) 
		{
			extiCallbackTable[15]();
			EXTI_ClearITPendingBit(EXTI_Line15);    
		}   

	}

}
