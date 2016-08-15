/**
  ******************************************************************************
  * @file    timer_one.cpp
  * @author  shentq
  * @version V1.2
  * @date    2016/08/14
  * @brief   
  ******************************************************************************
  * @attention
  *
  * No part of this software may be used for any commercial activities by any form 
  * or means, without the prior written consent of shentq. This specification is 
  * preliminary and is subject to change at any time without notice. shentq assumes
  * no responsibility for any errors contained herein.
  * <h2><center>&copy; Copyright 2015 shentq. All Rights Reserved.</center></h2>
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "timer_one.h"

extern callback_fun_type timx_cb_table[7][5];


TimerOne::TimerOne()
{


}
void TimerOne::begin(uint32_t frq)
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
void TimerOne::reset_frq(uint32_t frq)
{
    begin(frq);
    interrupt(ENABLE);
    start();
}

void TimerOne::interrupt(FunctionalState enable)
{
    TIM_ClearITPendingBit(TIM1 , TIM_FLAG_Update);
    TIM_ITConfig(TIM1, TIM_IT_Update, enable);
}

void TimerOne::start(void)
{
    TIM_Cmd(TIM1, ENABLE); //????
}

void TimerOne::stop(void)
{
    TIM_Cmd(TIM1, DISABLE); //????
}
void TimerOne::base_init(uint16_t period, uint16_t prescaler)
{

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;


    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    TIM_DeInit(TIM1);
    TIM_TimeBaseStructure.TIM_Prescaler = prescaler - 1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = period - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    ////�˴���ͨ�ö�ʱ����һ�� ���ƶ�ʱ��������ٴβ���һ���ж�
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0 ;

    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
    //����ARR�Ĵ����Ƿ�ʹ��Ӱ�ӼĴ���
    // TIM_ARRPreloadConfig(TIM1, ENABLE);

}
void TimerOne::set_reload(uint16_t Autoreload)
{
    TIM_SetAutoreload(TIM1, Autoreload);
}
void TimerOne::clear_count(void)
{
    TIM1->CNT = 0;
}
void TimerOne::attach_interrupt(void(*callback)(void))
{
    timx_cb_table[0][0] = callback;
}

