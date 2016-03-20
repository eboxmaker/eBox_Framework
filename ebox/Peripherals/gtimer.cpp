/*
file   : gtimer.cpp
author : shentq
version: V1.1
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/
#include "gtimer.h"

//#define TIM_NUM 7

extern callback_fun_type timx_cb_table[7][5];

//////////////////////////////////////


TIM::TIM(TIM_TypeDef *TIMx)
{
    _TIMx = TIMx;
}

void TIM::begin(uint32_t frq)
{
    uint32_t _period  = 0;
    uint32_t _prescaler = 1;
    if(frq >= 1000000)frq = 1000000;
    for(; _prescaler <= 0xffff; _prescaler++)
    {
        _period = 72000000 / _prescaler / frq;
        if((0xffff >= _period))break;
    }

    base_init(_period, _prescaler);
}
void TIM::reset_frq(uint32_t frq)
{
    begin(frq);
    interrupt(ENABLE);
    start();
}
void TIM::attach_interrupt(void(*callback)(void))
{
    switch((uint32_t)_TIMx)
    {
    case (uint32_t)TIM2_BASE:
        timx_cb_table[1][0] = callback;
        break;
    case (uint32_t)TIM3_BASE:
        timx_cb_table[2][0] = callback;
        break;
    case (uint32_t)TIM4_BASE:
        timx_cb_table[3][0] = callback;
        break;
    case (uint32_t)TIM5_BASE:
        timx_cb_table[4][0] = callback;
        break;
    case (uint32_t)TIM6_BASE:
        timx_cb_table[5][0] = callback;
        break;
    case (uint32_t)TIM7_BASE:
        timx_cb_table[6][0] = callback;
        break;
    }
}
void TIM::interrupt(FunctionalState enable)
{
    TIM_ClearITPendingBit(_TIMx , TIM_FLAG_Update);//必须加，否则开启中断会立即产生一次中断
    TIM_ITConfig(_TIMx, TIM_IT_Update, enable);
}

void TIM::start(void)
{
    TIM_Cmd(_TIMx, ENABLE); //????
}

void TIM::stop(void)
{
    TIM_Cmd(_TIMx, DISABLE); //????
}
void TIM::base_init(uint16_t period, uint16_t prescaler)
{
    NVIC_InitTypeDef NVIC_InitStructure;


    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_DeInit(_TIMx);
    switch((uint32_t)_TIMx)
    {
    //		case (uint32_t)TIM1:
    //			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    //			NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;//
    //			break;
    case (uint32_t)TIM2_BASE:
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
        NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;//
        break;
    case (uint32_t)TIM3_BASE:
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
        NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;//
        break;
    case (uint32_t)TIM4_BASE:
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
        NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;//
        break;
#if defined (STM32F10X_HD)
    case (uint32_t)TIM5_BASE:
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
        NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;//
        break;
    case (uint32_t)TIM6_BASE:
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
        NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;//
        break;
    case (uint32_t)TIM7_BASE:
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
        NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;//
        break;
#endif
    }

    TIM_TimeBaseStructure.TIM_Period = period - 1; //ARR寄存器
    TIM_TimeBaseStructure.TIM_Prescaler = prescaler - 1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //单边斜坡
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

    TIM_TimeBaseInit(_TIMx, &TIM_TimeBaseStructure);
    TIM_ARRPreloadConfig(_TIMx, ENABLE);	//控制ARR寄存器是否使用影子寄存器

    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

}
void TIM::set_reload(uint16_t auto_reload)
{
    TIM_SetAutoreload(_TIMx, auto_reload);
}
void TIM::clear_count(void)
{
    _TIMx->CNT = 0;
}




