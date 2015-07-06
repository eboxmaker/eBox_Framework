/*
file   : interrupts.c
author : shentq
version: V1.0
date   : 2015/7/5
brief  : binding interrupt callback function

Copyright (c) 2015, eBox by shentqlf@163.com. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentqlf@163.com.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentqlf@163.com assumes no responsibility for any errors contained herein.
*/

#include "ebox.h"

 __IO uint32_t TimingMillis;
 
callbackFun timOneCallbackTable[5];
callbackFun gTimxCallbackTable[TIM_NUM +1];
callbackFun extiCallbackTable[EXTI_LINE_NUM + 1];
callbackFun UARTCallbackTable[USART_NUM * 2];//支持串口的rx和tx中断
callbackFun rtcCallbackTable[3];//

void SysTick_Handler(void)
{

	TimingMillis++;

}

void RTC_IRQHandler(void)
{
	if (RTC_GetITStatus(RTC_IT_OW) != RESET)
	{	
		rtcCallbackTable[0]();
		RTC_ClearITPendingBit(RTC_IT_OW);
	}	
	if (RTC_GetITStatus(RTC_IT_ALR) != RESET)
	{	
		rtcCallbackTable[1]();
		RTC_ClearITPendingBit(RTC_IT_ALR);
	}	
	if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
	{	

		rtcCallbackTable[2]();
		RTC_ClearITPendingBit(RTC_IT_SEC);
	}
}

void TIM1_UP_IRQHandler(void)
{

 if(TIM_GetITStatus(TIM1 , TIM_IT_Update) == SET)
 {
  TIM_ClearITPendingBit(TIM1 , TIM_FLAG_Update);
		timOneCallbackTable[0]();
 }
}
void TIM2_IRQHandler(void)
{

 if(TIM_GetITStatus(TIM2 , TIM_IT_Update) == SET)
 {
  TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);
		gTimxCallbackTable[2]();
 }
}
void TIM3_IRQHandler(void)
{
 if(TIM_GetITStatus(TIM3 , TIM_IT_Update) == SET)
 {
  TIM_ClearITPendingBit(TIM3 , TIM_FLAG_Update);
		gTimxCallbackTable[3]();
 }
}
void TIM4_IRQHandler(void)
{
 if(TIM_GetITStatus(TIM4 , TIM_IT_Update) == SET)
 {
  TIM_ClearITPendingBit(TIM4 , TIM_FLAG_Update);
		gTimxCallbackTable[4]();
 }
}
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
void USART1_IRQHandler(void)
{
	
	if(USART_GetITStatus(USART1,USART_IT_RXNE)==SET)
	{
		UARTCallbackTable[0]();
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		return;
	}
	if(USART_GetITStatus(USART1,USART_IT_TXE)==SET)
	{
		UARTCallbackTable[1]();
		USART_ClearITPendingBit(USART1,USART_IT_TXE);
	}
}
void USART2_IRQHandler(void)
{
	
	if(USART_GetITStatus(USART2,USART_IT_RXNE)==SET)
	{
		UARTCallbackTable[2]();
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
		return;
	}	
	if(USART_GetITStatus(USART2,USART_IT_TXE)==SET)
	{
		UARTCallbackTable[3]();
		USART_ClearITPendingBit(USART2,USART_IT_TXE);
	}
}
void USART3_IRQHandler(void)
{
	
	if(USART_GetITStatus(USART3,USART_IT_RXNE)==SET)
	{
		UARTCallbackTable[4]();
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);
		return;
	}
	if(USART_GetITStatus(USART3,USART_IT_TXE)==SET)
	{
		UARTCallbackTable[5]();
		USART_ClearITPendingBit(USART3,USART_IT_TXE);
	}
}
