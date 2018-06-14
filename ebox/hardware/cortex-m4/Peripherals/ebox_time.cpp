/**
  ******************************************************************************
  * @file    pwm.cpp
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
#include "ebox_timer.h"


extern callback_fun_type timx_cb_table[7][5];

//////////////////////////////////////


Timer::Timer(TIM_TypeDef *TIMx)
{
    this->TIMx = TIMx;
}

void Timer::begin(uint32_t frq)
{
    uint32_t _period  = 0;
    uint32_t _prescaler = 1;
    
    if(frq >= get_max_frq())frq = get_max_frq();//控制最大频率
    for(; _prescaler <= 0xffff; _prescaler++)
    {
        _period = get_timer_source_clock() / _prescaler / frq;
        if((0xffff >= _period))break;
    }
    
    base_init(_period, _prescaler);
}
void Timer::reset_frq(uint32_t frq)
{
    begin(frq);
    interrupt(ENABLE);
    start();
}
void Timer::interrupt(FunctionalState enable)
{
    TIM_ClearITPendingBit(TIMx , TIM_FLAG_Update);//必须加，否则开启中断会立即产生一次中断
    TIM_ITConfig(TIMx, TIM_IT_Update, enable);
}

void Timer::start(void)
{
    TIM_Cmd(TIMx, ENABLE); //
}

void Timer::stop(void)
{
    TIM_Cmd(TIMx, DISABLE); //
}
void Timer::base_init(uint32_t _period, uint32_t _prescaler)
{
    NVIC_InitTypeDef NVIC_InitStructure;


    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_DeInit(TIMx);
    switch((uint32_t)TIMx)
    {
    case (uint32_t)TIM1:
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
        //此处和通用定时器不一样 控制定时器溢出多少次产生一次中断
        TIM_TimeBaseStructure.TIM_RepetitionCounter = 0 ;
        NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;//
        break;
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
    case (uint32_t)TIM5_BASE:
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
        NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;//
        break;
    case (uint32_t)TIM6_BASE:
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
        NVIC_InitStructure.NVIC_IRQChannel = TIM6_DAC_IRQn;//
        break;
    case (uint32_t)TIM7_BASE:
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
        NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;//
        break;
    case (uint32_t)TIM8_BASE:
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
        NVIC_InitStructure.NVIC_IRQChannel = TIM8_UP_TIM13_IRQn;//
        break;
    case (uint32_t)TIM9_BASE:
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);
        NVIC_InitStructure.NVIC_IRQChannel = TIM1_BRK_TIM9_IRQn;//
        break;
    case (uint32_t)TIM10_BASE:
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE);
        NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;//
        break;
    case (uint32_t)TIM11_BASE:
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11, ENABLE);
        NVIC_InitStructure.NVIC_IRQChannel = TIM1_TRG_COM_TIM11_IRQn;//
        break;
    case (uint32_t)TIM12_BASE:
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE);
        NVIC_InitStructure.NVIC_IRQChannel = TIM8_BRK_TIM12_IRQn;//
        break;
    case (uint32_t)TIM13_BASE:
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13, ENABLE);
        NVIC_InitStructure.NVIC_IRQChannel = TIM8_UP_TIM13_IRQn;//
        break;
    case (uint32_t)TIM14_BASE:
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
        NVIC_InitStructure.NVIC_IRQChannel = TIM8_TRG_COM_TIM14_IRQn;//
        break;

    }

    TIM_TimeBaseStructure.TIM_Period = _period - 1; //ARR寄存器
    TIM_TimeBaseStructure.TIM_Prescaler = _prescaler - 1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //单边斜坡
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

    TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);
    TIM_ARRPreloadConfig(TIMx, ENABLE);	//控制ARR寄存器是否使用影子寄存器

    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

}
void Timer::attach_interrupt(void(*callback)(void))
{
    switch((uint32_t)TIMx)
    {
    case (uint32_t)TIM1_BASE:
        timx_cb_table[0][0] = callback;
        break;
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
    case (uint32_t)TIM8_BASE:
        timx_cb_table[7][0] = callback;
        break;
    case (uint32_t)TIM9_BASE:
        timx_cb_table[8][0] = callback;
        break;
    case (uint32_t)TIM10_BASE:
        timx_cb_table[9][0] = callback;
        break;
    case (uint32_t)TIM11_BASE:
        timx_cb_table[10][0] = callback;
        break;
    case (uint32_t)TIM12_BASE:
        timx_cb_table[11][0] = callback;
        break;
    case (uint32_t)TIM13_BASE:
        timx_cb_table[12][0] = callback;
        break;
    case (uint32_t)TIM14_BASE:
        timx_cb_table[13][0] = callback;
        break;

    }
}

void Timer::set_reload(uint16_t auto_reload)
{
    TIMx->ARR = auto_reload;
}
void Timer::clear_count(void)
{
    TIMx->CNT = 0;
}
uint32_t Timer::get_timer_source_clock()
{
    uint32_t temp = 0;
    uint32_t timer_clock = 0x00;
    
    if ((uint32_t)this->TIMx == TIM1_BASE)
    {
        timer_clock = cpu.clock.pclk2;
    }
    else
    {
        temp = RCC->CFGR & (0x7 << 10);
        if(temp  == 0)//检测PCLK是否进行过分频，如果进行过分频则定时器的频率为PCLK1的两倍
            timer_clock = cpu.clock.pclk1 ;
        else
            timer_clock = cpu.clock.pclk1 * 2;
    }
    return timer_clock;
}

uint32_t Timer::get_max_frq()
{
    return get_timer_source_clock()/400;

}




