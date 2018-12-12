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
#include "ebox_timer_it.h"


//////////////////////////////////////


Timer::Timer(TIM_TypeDef *TIMx)
{
    this->TIMx = TIMx;
}

void Timer::begin(uint32_t frq)
{
    uint32_t _period  = 0;
    uint32_t _prescaler = 1;
    uint8_t index;

    if(frq >= get_max_frq())frq = get_max_frq();//控制最大频率
    for(; _prescaler <= 0xffff; _prescaler++)
    {
        _period = get_timer_source_clock() / _prescaler / frq;
        if((0xffff >= _period))break;
    }


    switch((uint32_t)TIMx)
    {
    case (uint32_t)TIM1_BASE:
        index = TIM1_IT_Update;
        break;
    case (uint32_t)TIM2_BASE:
        index = TIM2_IT_Update;
        break;
    case (uint32_t)TIM3_BASE:
        index = TIM3_IT_Update;
        break;
    case (uint32_t)TIM4_BASE:
        index = TIM4_IT_Update;
        break;
    case (uint32_t)TIM5_BASE:
        index = TIM5_IT_Update;
        break;
    case (uint32_t)TIM6_BASE:
        index = TIM6_IT_Update;
        break;
    case (uint32_t)TIM7_BASE:
        index = TIM7_IT_Update;
        break;
    case (uint32_t)TIM8_BASE:
        index = TIM8_IT_Update;
        break;
    case (uint32_t)TIM9_BASE:
        index = TIM9_IT_Update;
        break;
    case (uint32_t)TIM10_BASE:
        index = TIM10_IT_Update;
        break;
    }

    tim_irq_init(index, (&Timer::_irq_handler), (uint32_t)this);

    base_init(_period, _prescaler);
    nvic(ENABLE, 0, 0);
    interrupt(ENABLE);

}
void Timer::reset_frq(uint32_t frq)
{
    begin(frq);
    start();
}
void Timer::nvic(FunctionalState enable, uint8_t preemption_priority, uint8_t sub_priority)
{
    nvic_dev_set_priority((uint32_t)TIMx, 0, preemption_priority, sub_priority);
    if(enable == ENABLE)
        nvic_dev_enable((uint32_t)TIMx, 0);
    else
        nvic_dev_disable((uint32_t)TIMx, 0);
}
void Timer::interrupt(FunctionalState enable)
{
    TIM_ClearITPendingBit(TIMx, TIM_FLAG_Update); //必须加，否则开启中断会立即产生一次中断

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

    if ((uint32_t)this->TIMx == TIM1_BASE ||
            (uint32_t)this->TIMx == TIM8_BASE ||
            (uint32_t)this->TIMx == TIM9_BASE ||
            (uint32_t)this->TIMx == TIM10_BASE ||
            (uint32_t)this->TIMx == TIM11_BASE )
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
    return get_timer_source_clock() / 400;

}


void Timer::base_init(uint32_t _period, uint32_t _prescaler)
{


    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_DeInit(TIMx);

    rcc_clock_cmd((uint32_t)TIMx, ENABLE);

    //此处和通用定时器不一样 控制定时器溢出多少次产生一次中断
    //    if(TIMx == TIM1 || TIMx == TIM8)
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 1 ;

    TIM_TimeBaseStructure.TIM_Period = _period - 1; //ARR寄存器
    TIM_TimeBaseStructure.TIM_Prescaler = _prescaler - 1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //单边斜坡
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

    TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);
    TIM_ARRPreloadConfig(TIMx, ENABLE);	//控制ARR寄存器是否使用影子寄存器

}


void Timer::_irq_handler( uint32_t id)
{
    Timer *handler = (Timer *)id;
    handler->_irq.call();

}


void Timer::attach(void (*fptr)(void))
{
    if (fptr)
    {
        _irq.attach(fptr);
    }
}
