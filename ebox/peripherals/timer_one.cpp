/*-
file   : timerone.cpp
author : shentq
version: V1.1
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/
#include "timer_one.h"

extern callback_fun_type timx_cb_table[7][5];


TIMERONE::TIMERONE()
{


}
void TIMERONE::begin(uint32_t frq)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    uint32_t _period  = 0;
    uint32_t _prescaler = 1;

    if(frq >= 1000000)frq = 1000000;

    for(; _prescaler <= 0xffff; _prescaler++)
    {
        _period = 72000000 / _prescaler / frq;
        if((0xffff >= _period))break;
    }

    base_init(_period, _prescaler);

    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;//
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);


    interrupt(DISABLE);
    stop();
}
void TIMERONE::reset_frq(uint32_t frq)
{
    begin(frq);
    interrupt(ENABLE);
    start();
}

void TIMERONE::interrupt(FunctionalState enable)
{
    TIM_ClearITPendingBit(TIM1 , TIM_FLAG_Update);
    TIM_ITConfig(TIM1, TIM_IT_Update, enable);
}

void TIMERONE::start(void)
{
    TIM_Cmd(TIM1, ENABLE); //????
}

void TIMERONE::stop(void)
{
    TIM_Cmd(TIM1, DISABLE); //????
}
void TIMERONE::base_init(uint16_t period, uint16_t prescaler)
{

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;


    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    TIM_DeInit(TIM1);
    TIM_TimeBaseStructure.TIM_Prescaler = prescaler - 1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = period - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    ////此处和通用定时器不一样 控制定时器溢出多少次产生一次中断
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0 ;

    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
    //控制ARR寄存器是否使用影子寄存器
    // TIM_ARRPreloadConfig(TIM1, ENABLE);

}
void TIMERONE::set_reload(uint16_t Autoreload)
{
    TIM_SetAutoreload(TIM1, Autoreload);
}
void TIMERONE::clear_count(void)
{
    TIM1->CNT = 0;
}
void TIMERONE::attach_interrupt(void(*callback)(void))
{
    timx_cb_table[0][0] = callback;
}

