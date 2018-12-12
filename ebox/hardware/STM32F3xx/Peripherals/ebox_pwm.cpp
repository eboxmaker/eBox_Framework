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
#include "ebox_pwm.h"



Pwm::Pwm(Gpio *pwm_pin, TIM_TypeDef *timer, uint8_t ch)
{
    this->pwm_pin = pwm_pin;
    this->TIMx = timer;
    this->ch = ch;
    switch((uint32_t)timer)
    {
    case TIM1_BASE:
        af_timer_x = GPIO_AF_TIM1;
        break;
    case TIM2_BASE:
        af_timer_x = GPIO_AF_TIM2;
        break;
    case TIM3_BASE:
        af_timer_x = GPIO_AF_TIM3;
        break;
    case TIM4_BASE:

        af_timer_x = GPIO_AF_TIM4;
        break;
    case TIM5_BASE:

        af_timer_x = GPIO_AF_TIM5;
        break;
    case TIM8_BASE:

        af_timer_x = GPIO_AF_TIM8;
        break;
    case TIM9_BASE:

        af_timer_x = GPIO_AF_TIM9;
        break;
    case TIM10_BASE:

        af_timer_x = GPIO_AF_TIM10;
        break;
    case TIM11_BASE:

        af_timer_x = GPIO_AF_TIM11;
        break;
    default :
        break;
    }

}

void Pwm::begin(uint32_t frq, uint16_t duty)
{
    this->duty = duty;

    pwm_pin->mode(AF_PP, af_timer_x);


    rcc_clock_cmd((uint32_t)TIMx, ENABLE);
    set_oc_polarity(1);
    set_frq(frq);
    _set_duty(duty);
}

void Pwm::set_frq(uint32_t frq)
{
    uint32_t _period  = 0;
    uint32_t _prescaler = 1;

    if(frq >= get_max_frq())//控制频率，保证其有1%精度
        frq = get_max_frq();

    //千分之一精度分配方案
    for(; _prescaler <= 0xffff; _prescaler++)
    {
        _period = get_timer_source_clock() / _prescaler / frq;
        if((0xffff >= _period) && (_period >= 1000))
        {
            accuracy = 1;
            break;
        }
    }

    if(_prescaler == 65536)//上述算法分配失败
    {
        //百分之一分配方案
        for(_prescaler = 1; _prescaler <= 0xffff; _prescaler++)
        {
            _period = get_timer_source_clock() / _prescaler / frq;
            if((0xffff >= _period) && (_period >= 100))
            {
                accuracy = 2;
                break;
            }
        }
    }
    this->period = _period;


    base_init(this->period, _prescaler);
    _set_duty(duty);

}
//duty:0-1000对应0%-100.0%
void Pwm::set_duty(uint16_t duty)
{
    this->duty = duty;

    uint16_t pulse = 0;
    float percent;

    if(this->duty > 1000)
        this->duty = 1000;
    percent = this->duty / 1000.0;

    pulse = (uint16_t) (( percent  * this->period ));

    switch(ch)
    {
    case TIMxCH1:
        TIMx->CCR1 = pulse;
        break;
    case TIMxCH2:
        TIMx->CCR2 = pulse;
        break;
    case TIMxCH3:
        TIMx->CCR3 = pulse;
        break;
    case TIMxCH4:
        TIMx->CCR4 = pulse;
        break;
    }

}
void Pwm::set_oc_polarity(uint8_t flag)
{

    if(flag == 1)
        this->oc_polarity = TIM_OCPolarity_High;
    else if(flag == 0)
        this->oc_polarity = TIM_OCPolarity_Low;
    _set_duty(duty);

}

//duty:0-1000对应0%-100.0%
void Pwm::_set_duty(uint16_t duty)
{

    this->duty = duty;

    uint16_t pulse = 0;
    float percent;

    if(this->duty > 1000)
        this->duty = 1000;
    percent = this->duty / 1000.0;

    pulse = (uint16_t) (( percent  * this->period ));

    TIM_OCInitTypeDef  TIM_OCInitStructure;

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = oc_polarity;
    TIM_OCInitStructure.TIM_Pulse = pulse;
    switch(ch)
    {
    case TIMxCH1:
        TIM_OC1Init(TIMx, &TIM_OCInitStructure);
        break;
    case TIMxCH2:
        TIM_OC2Init(TIMx, &TIM_OCInitStructure);
        break;
    case TIMxCH3:
        TIM_OC3Init(TIMx, &TIM_OCInitStructure);
        break;
    case TIMxCH4:
        TIM_OC4Init(TIMx, &TIM_OCInitStructure);
        break;
    }
}

void Pwm::base_init(uint32_t _period, uint32_t _prescaler)
{
    this->period = _period;//更新period

    if( TIMx == TIM1 ||
            TIMx == TIM8 ||
            TIMx == TIM9 ||
            TIMx == TIM10 ||
            TIMx == TIM11 )
    {
        TIM_CtrlPWMOutputs(TIMx, ENABLE);
    }
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    TIM_TimeBaseStructure.TIM_Period = this->period - 1; //ARR
    TIM_TimeBaseStructure.TIM_Prescaler = _prescaler - 1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 1 ;
    TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);

    TIM_ARRPreloadConfig(TIMx, ENABLE);
    TIM_Cmd(TIMx, ENABLE); //

}

uint32_t Pwm::get_timer_source_clock()
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

uint32_t Pwm::get_max_frq()
{
    return get_timer_source_clock() / 100;

}

float Pwm::get_accuracy()
{

    switch(accuracy)
    {
    case 0:
        return 0;
    case 1:
        return 0.001;
    case 2:
        return 0.01;

    }
    return 0.001;


}

