/**
  ******************************************************************************
  * @file    timer.cpp
  * @author  shentq
  * @version V2.1
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


Timer::Timer(TIM_Module *TIMx)
{
    _TIMx = TIMx;
}

void Timer::begin(uint32_t frq)
{

    uint32_t _period  = 0;
    uint32_t _prescaler = 1;
    uint8_t index;

//    if(frq >= get_max_frq())frq = get_max_frq();//控制最大频率
    for(; _prescaler <= 0xffff; _prescaler++)
    {
        _period = get_timer_source_clock() / _prescaler / frq;
        if((0xffff >= _period))break;
    }

    switch((uint32_t)_TIMx)
    {
    case (uint32_t)TIM1_BASE:
        index = TIM1_IT_Update;
        break;
//    case (uint32_t)TIM2_BASE:
//        index = TIM2_IT_Update;
//        break;
    case (uint32_t)TIM3_BASE:
        index = TIM3_IT_Update;
        break;
//    case (uint32_t)TIM4_BASE:
//        index = TIM4_IT_Update;
//        break;
//    case (uint32_t)TIM5_BASE:
//        index = TIM5_IT_Update;
//        break;
    case (uint32_t)TIM6_BASE:
        index = TIM6_IT_Update;
        break;
//    case (uint32_t)TIM7_BASE:
//        index = TIM7_IT_Update;
//        break;
    case (uint32_t)TIM8_BASE:
        index = TIM8_IT_Update;
        break;
    }
    tim_irq_init(index, (&Timer::_irq_handler), (uint32_t)this);

    rcc_clock_cmd((uint32_t)_TIMx, ENABLE);

    base_init(_period, _prescaler);
    nvic(ENABLE, 0, 0);
    interrupt(DISABLE);
}
void Timer::reset_frq(uint32_t frq)
{
    begin(frq);
    start();
}
void Timer::nvic(FunctionalState enable, uint8_t preemption_priority, uint8_t sub_priority)
{
//    nvic_dev_set_priority((uint32_t)_TIMx, 0, preemption_priority, sub_priority);
//    if(enable != DISABLE)
//        nvic_dev_enable((uint32_t)_TIMx, 0);
//    else
//        nvic_dev_disable((uint32_t)_TIMx, 0);
    NVIC_InitType NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel          = dev_to_irqn((uint32_t)_TIMx,0);
    NVIC_InitStructure.NVIC_IRQChannelPriority  = preemption_priority;
    NVIC_InitStructure.NVIC_IRQChannelCmd       = enable;
    NVIC_Init(&NVIC_InitStructure);
}

void Timer::interrupt(FunctionalState enable)
{
    TIM_ClrIntPendingBit(_TIMx, TIM_FLAG_UPDATE); //必须加，否则开启中断会立即产生一次中断
    TIM_ConfigInt(_TIMx, TIM_INT_UPDATE, enable);
    
}

void Timer::start(void)
{
    TIM_Enable(_TIMx, ENABLE);
}

void Timer::stop(void)
{
    TIM_Enable(_TIMx, DISABLE);
}
void Timer::base_init(uint16_t period, uint16_t prescaler)
{
    TIM_TimeBaseInitType TIM_TimeBaseStructure;
    TIM_DeInit(_TIMx);


    TIM_ConfigInternalClk(_TIMx);
    
    //此处和通用定时器不一样 控制定时器溢出多少次产生一次中断
    if(_TIMx == TIM1)
        TIM_TimeBaseStructure.RepetCnt = 0 ;

    TIM_TimeBaseStructure.Period = period - 1; //ARR寄存器
    TIM_TimeBaseStructure.Prescaler = prescaler - 1;
    TIM_TimeBaseStructure.CntMode = TIM_CNT_MODE_UP; //单边斜坡
    TIM_TimeBaseStructure.ClkDiv = TIM_CLK_DIV1;

    TIM_InitTimeBase(_TIMx, &TIM_TimeBaseStructure);
    
    TIM_ConfigArPreload(_TIMx, ENABLE);	//控制ARR寄存器是否使用影子寄存器
    

}
void Timer::set_reload(uint16_t auto_reload)
{
    TIM_SetAutoReload(_TIMx, auto_reload);
}
void Timer::clear_count(void)
{
    _TIMx->CNT = 0;
}

uint32_t Timer::get_timer_source_clock()
{
    uint32_t temp = 0;
    uint32_t timer_clock = 0x00;

    if ((uint32_t)this->_TIMx == TIM1_BASE)
    {
        timer_clock = cpu.clock.pclk2;
    }
    else
    {
        temp = RCC->CFG;
        if(temp & 0x00000400)//检测PCLK是否进行过分频，如果进行过分频则定时器的频率为PCLK1的两倍
            timer_clock = cpu.clock.pclk1 * 2;
        else
            timer_clock = cpu.clock.pclk1 ;
    }
    return timer_clock;
}
uint32_t Timer::get_max_frq()
{
    return get_timer_source_clock();

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




