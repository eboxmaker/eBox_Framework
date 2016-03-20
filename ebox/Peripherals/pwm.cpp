/*
file   : pwm.cpp
author : shentq
version: V1.1
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/
#include "pwm.h"




#define TIMxCH1 0x01
#define TIMxCH2 0x02
#define TIMxCH3 0x03
#define TIMxCH4 0x04



PWM::PWM(GPIO *pwm_pin)
{
    //	if(isPwmPin(PWMpin))
    //	{
    this->pwm_pin = pwm_pin;
    //	}
}
void PWM::begin(uint32_t frq, uint16_t duty)
{
    this->duty = duty;

    init_info(pwm_pin);
    pwm_pin->mode(AF_PP);

    set_oc_polarity(1);
    set_frq(frq);
    set_duty(duty);
}
void PWM::base_init(uint16_t period, uint16_t prescaler)
{
    this->period = period;//更新period


    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    RCC_APB1PeriphClockCmd(rcc, ENABLE);
    TIM_TimeBaseStructure.TIM_Period = this->period - 1; //ARR
    TIM_TimeBaseStructure.TIM_Prescaler = prescaler - 1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //
    TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);

    TIM_ARRPreloadConfig(TIMx, ENABLE);

    TIM_Cmd(TIMx, ENABLE); //

}
void PWM::init_info(GPIO *pwm_pin)
{
    if(pwm_pin->port == GPIOA)
    {
        switch(pwm_pin->pin)
        {
        case GPIO_Pin_0:
            TIMx = TIM2;
            rcc = RCC_APB1Periph_TIM2;
            ch = TIMxCH1;//irq = TIM2_IRQn;
            break;
        case GPIO_Pin_1:
            TIMx = TIM2;
            rcc = RCC_APB1Periph_TIM2;
            ch = TIMxCH2;//irq = TIM2_IRQn;
            break;
        case GPIO_Pin_2:
            TIMx = TIM2;
            rcc = RCC_APB1Periph_TIM2;
            ch = TIMxCH3;//irq = TIM2_IRQn;
            break;
        case GPIO_Pin_3:
            TIMx = TIM2;
            rcc = RCC_APB1Periph_TIM2;
            ch = TIMxCH4;//irq = TIM2_IRQn;
            break;

        case GPIO_Pin_6:
            TIMx = TIM3;
            rcc = RCC_APB1Periph_TIM3;
            ch = TIMxCH1;//irq = TIM3_IRQn;
            break;
        case GPIO_Pin_7:
            TIMx = TIM3;
            rcc = RCC_APB1Periph_TIM3;
            ch = TIMxCH2;//irq = TIM3_IRQn;
            break;
        case GPIO_Pin_10:
            TIMx = TIM3;
            rcc = RCC_APB1Periph_TIM3;
            ch = TIMxCH3;//irq = TIM3_IRQn;
            break;
        case GPIO_Pin_11:
            TIMx = TIM3;
            rcc = RCC_APB1Periph_TIM3;
            ch = TIMxCH4;//irq = TIM3_IRQn;
            break;
        }
    }
    if(pwm_pin->port == GPIOB)
    {
        switch(pwm_pin->pin)
        {
        case GPIO_Pin_6:
            TIMx = TIM4;
            rcc = RCC_APB1Periph_TIM4;
            ch = TIMxCH1;//irq = TIM4_IRQn;
            break;
        case GPIO_Pin_7:
            TIMx = TIM4;
            rcc = RCC_APB1Periph_TIM4;
            ch = TIMxCH2;//irq = TIM4_IRQn;
            break;
        case GPIO_Pin_8:
            TIMx = TIM4;
            rcc = RCC_APB1Periph_TIM4;
            ch = TIMxCH3;//irq = TIM4_IRQn;
            break;
        case GPIO_Pin_9:
            TIMx = TIM4;
            rcc = RCC_APB1Periph_TIM4;
            ch = TIMxCH4;//irq = TIM4_IRQn;
            break;
        }
    }

}

void PWM::set_oc_polarity(uint8_t flag)
{

    if(flag == 1)
        this->oc_polarity = TIM_OCPolarity_High;
    else if(flag == 0)
        this->oc_polarity = TIM_OCPolarity_Low;
    set_duty(duty);

}
//pwm的频率 = 72M/72/1000;
//
void PWM::set_frq(uint32_t frq)
{
    uint32_t period  = 0;
    uint32_t prescaler = 1;
    if(frq >= 720000)frq = 720000;
    //千分之一精度分配方案
    for(; prescaler <= 0xffff; prescaler++)
    {
        period = 72000000 / prescaler / frq;
        if((0xffff >= period) && (period >= 1000))break;
    }
    if(prescaler == 65536)//上述算法分配失败
        //百分之一分配方案
        for(prescaler = 1; prescaler <= 0xffff; prescaler++)
        {
            period = 72000000 / prescaler / frq;
            if((0xffff >= period) && (period >= 100))break;
        }


    base_init(period, prescaler);
    set_duty(duty);

}

//duty:0-1000对应0%-100.0%
void PWM::set_duty(uint16_t duty)
{

    this->duty = duty;

    uint16_t pulse = 0;
    float percent;

    if(this->duty > 1000)
        this->duty = 1000;
    percent = this->duty / 1000.0;

    pulse = (uint16_t) (( percent  * period ));

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
//duty:0-1000对应0%-100.0%
void analog_write(GPIO *pwm_pin, uint16_t duty)
{
    //	if(isPwmPin(PWMpin))
    //	{
    PWM p(pwm_pin);
    p.begin(1000, duty);
    //p.SetFrq(1000,1);
    //			p.set_duty(duty);

    //	}
    //	else
    //	{
    //	;
    //	}
}

//////////////////////////////////////////////////////////////
uint8_t isPwmPin(uint8_t pin)
{

    return 0;
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
