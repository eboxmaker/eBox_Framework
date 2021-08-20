/**
  ******************************************************************************
  * @file    pwm.cpp
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
#include "ebox_pulse.h"


Pulse::Pulse(Gpio *pwm_pin)
{
    //	if(isPwmPin(PWMpin))
    //	{
    this->pin = pwm_pin;
    //	}
}
void Pulse::begin(uint16_t percent)
{
    this->percent = percent;
    init_info(pin);
    pin->mode(AF_PP);
    set_oc_polarity(1);
    //set_pulse(percent);
    base_init(cfg.max_counter,cfg.prescaler);
    oc_mode_init();

}
void Pulse::start()
{
    TIM_Cmd(TIMx, ENABLE); //
}
void Pulse::stop()
{
    TIM_Cmd(TIMx, DISABLE); //
}
void Pulse::base_init(uint16_t period, uint16_t prescaler)
{

    rcc_clock_cmd((uint32_t)TIMx, ENABLE);


    if(TIMx == TIM1 ||  TIMx == TIM8 )
    {
        TIM_CtrlPWMOutputs(TIMx, ENABLE);
    }

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Period = period; //ARR
    TIM_TimeBaseStructure.TIM_Prescaler = prescaler - 1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //
//    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);

    TIM_ARRPreloadConfig(TIMx, ENABLE);
    TIM_Cmd(TIMx, DISABLE); //

}
void Pulse::set_oc_polarity(uint8_t flag)
{

    if(flag == 1)
        this->oc_polarity = TIM_OCPolarity_High;
    else if(flag == 0)
        this->oc_polarity = TIM_OCPolarity_Low;
}

//duty:0-1000对应0%-100.0%
void Pulse::oc_mode_init()
{

    TIM_OCInitTypeDef  TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
//    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_Pulse = cfg.delay_counter;
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

    TIM_SelectOnePulseMode(TIM2, TIM_OPMode_Single);//选择定时器为单脉冲模式

}

//duty:0-1000对应0%-100.0%
void Pulse::set_delay(uint16_t counter)
{
    
    
    cfg.delay_counter = counter;
    
    
    switch(ch)
    {
    case TIMxCH1:
        TIMx->CCR1 = cfg.delay_counter;
        break;
    case TIMxCH2:
        TIMx->CCR2 = cfg.delay_counter;
        break;
    case TIMxCH3:
        TIMx->CCR3 = cfg.delay_counter;
        break;
    case TIMxCH4:
        TIMx->CCR4 = cfg.delay_counter;
        break;
    }

}
void Pulse::set_reload(uint16_t value)
{
    cfg.max_counter = value;
}

float Pulse::get_delay_us()
{
    uint32_t clock = get_timer_source_clock() / cfg.prescaler;
        
    float delay_time =  cfg.delay_counter / (float)clock * 1000000;
    return delay_time;
}
//duty:0-1000对应0%-100.0%
void Pulse::set_phase_front(uint16_t percent)
{
    uint16_t diff = cfg.max_counter - cfg.delay_counter;
    uint16_t ccr;
    
    ccr  = cfg.delay_counter + diff - diff * (percent / 1000.0);
    
    
    switch(ch)
    {
    case TIMxCH1:
        TIMx->CCR1 = ccr;
        break;
    case TIMxCH2:
        TIMx->CCR2 = ccr;
        break;
    case TIMxCH3:
        TIMx->CCR3 = ccr;
        break;
    case TIMxCH4:
        TIMx->CCR4 = ccr;
        break;
    }

}

void Pulse::set_phase_after(uint16_t percent)
{
    limit(percent,(uint16_t)0,(uint16_t)1000);

    uint16_t diff = cfg.max_counter - cfg.delay_counter;
    
    arr = (uint16_t) (percent * diff / 1000.0) + cfg.delay_counter;
    
    TIMx->ARR = arr;

}


uint32_t Pulse::get_timer_source_clock()
{
    uint32_t temp = 0;
    uint32_t timer_clock = 0x00;

    if ((uint32_t)this->TIMx == TIM1_BASE)
    {
        timer_clock = cpu.clock.pclk2;
    }
    else
    {
        temp = RCC->CFGR;
        if(temp & 0x00000400)//检测PCLK是否进行过分频，如果进行过分频则定时器的频率为PCLK1的两倍
            timer_clock = cpu.clock.pclk1 * 2;
        else
            timer_clock = cpu.clock.pclk1 ;
    }
    return timer_clock;
}

void Pulse::enable_pin()
{
    pin->mode(AF_PP);
}
void Pulse::disable_pin()
{
    pin->mode(INPUT);
}
void Pulse::end()
{

}
//uint8_t isPinNeedRemap(uint8_t pin)
//{
//	int i;

//	for( i = 0; i<255; i++)
//	{
//		if(pinTOTimx[i].pin == pin)
//
//			return pinTOTimx[i].needremap;
//	}
//		return 0XFF;
//}
void Pulse::init_info(Gpio *pwm_pin)
{
    switch((uint8_t)pwm_pin->id)
    {
    //TIM2
    case PA0_ID:
        TIMx = TIM2;
        ch = TIMxCH1;//irq = TIM2_IRQn;
        break;
    case PA1_ID:
        TIMx = TIM2;
        ch = TIMxCH2;//irq = TIM2_IRQn;
        break;
    case PA2_ID:
        TIMx = TIM2;
        ch = TIMxCH3;//irq = TIM2_IRQn;
        break;
    case PA3_ID:
        TIMx = TIM2;
        ch = TIMxCH4;//irq = TIM2_IRQn;
        break;

    //TIM3
    case PA6_ID:
        TIMx = TIM3;
        ch = TIMxCH1;//irq = TIM3_IRQn;
        break;
    case PA7_ID:
        TIMx = TIM3;
        ch = TIMxCH2;//irq = TIM3_IRQn;
        break;

    //TIM1
    case PA8_ID:
        TIMx = TIM1;
        ch = TIMxCH1;//irq = TIM4_IRQn;
        break;
    case PA9_ID:
    case PE10_ID:
        GPIO_PinRemapConfig(GPIO_FullRemap_TIM1, ENABLE);
        TIMx = TIM1;
        ch = TIMxCH2;//irq = TIM4_IRQn;
        break;
    case PA10_ID:
    case PE12_ID:
        TIMx = TIM1;
        ch = TIMxCH3;//irq = TIM4_IRQn;
        break;
    case PA11_ID:
        TIMx = TIM1;
        ch = TIMxCH4;//irq = TIM4_IRQn;
        break;

    //TIM4
    case PB6_ID:
        TIMx = TIM4;
        ch = TIMxCH1;//irq = TIM4_IRQn;
        break;
    case PB7_ID:
        TIMx = TIM4;
        ch = TIMxCH2;//irq = TIM4_IRQn;
        break;
    case PB8_ID:
        TIMx = TIM4;
        ch = TIMxCH3;//irq = TIM4_IRQn;
        break;
    case PB9_ID:
        TIMx = TIM4;
        ch = TIMxCH4;//irq = TIM4_IRQn;
        break;

    //TIM3
    case PB0_ID:
        TIMx = TIM3;
        ch = TIMxCH3;//irq = TIM3_IRQn;
        break;
    case PB1_ID:
        TIMx = TIM3;
        ch = TIMxCH4;//irq = TIM3_IRQn;
        break;
    }
}

