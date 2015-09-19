/*
file   : timerone.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/
#include "timerone.h"


callbackFun timOneCallbackTable[1];

TIMERONE::TIMERONE()
{


}
void TIMERONE::begin(uint32_t period,uint32_t prescaler)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	baseInit(period,prescaler);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;//
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;// 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;// 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	
	interrupt(DISABLE);
	stop();
}
void TIMERONE::interrupt(FunctionalState enable)
{
 TIM_ClearITPendingBit(TIM1 , TIM_FLAG_Update);
 TIM_ITConfig(TIM1,TIM_IT_Update,enable);

}

void TIMERONE::start(void)
{
	 TIM_Cmd(TIM1, ENABLE); //????
}

void TIMERONE::stop(void)
{
	 TIM_Cmd(TIM1, DISABLE); //????
}
void TIMERONE::baseInit(uint16_t period,uint16_t prescaler)
{
	
 TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	
	TIM_DeInit(TIM1);
  TIM_TimeBaseStructure.TIM_Prescaler = prescaler-1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = period-1;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	////此处和通用定时器不一样 控制定时器溢出多少次产生一次中断
  TIM_TimeBaseStructure.TIM_RepetitionCounter=0 ;

  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	//控制ARR寄存器是否使用影子寄存器
// TIM_ARRPreloadConfig(TIM1, ENABLE);
	
}
void TIMERONE::setReload(uint16_t Autoreload)
{
	TIM_SetAutoreload(TIM1,Autoreload);
}
void TIMERONE::clearCount(void)
{
	TIM1->CNT = 0;
}
void TIMERONE::attachInterrupt(void(*callback)(void))
{
	 timOneCallbackTable[0] = callback;
}
extern "C" {
	void TIM1_UP_IRQHandler(void)
	{

	 if(TIM_GetITStatus(TIM1 , TIM_IT_Update) == SET)
	 {
		TIM_ClearITPendingBit(TIM1 , TIM_FLAG_Update);
			timOneCallbackTable[0]();
	 }
	}
}
