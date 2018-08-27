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
#include "ebox_pwm.h"
#define PWM_DEBUG 0
#if PWM_DEBUG
    #include "ebox.h"
#endif




Pwm::Pwm(Gpio *pwm_pin)
{
    //	if(isPwmPin(PWMpin))
    //	{
    this->pin = pwm_pin;
    //	}
}
void Pwm::begin(uint32_t frq, uint16_t duty)
{
    this->duty = duty;

    init_info(pin);
    pin->mode(AF_PP);

    set_oc_polarity(1);
    set_frq(frq);
    _set_duty(duty);
}
void Pwm::base_init(uint16_t period, uint16_t prescaler)
{
    this->period = period;//����period

    rcc_clock_cmd((uint32_t)TIMx,ENABLE);


    if(TIMx == TIM1 ||  TIMx == TIM8 )
    {
        TIM_CtrlPWMOutputs(TIMx,ENABLE); 
    }
        
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Period = this->period - 1; //ARR
    TIM_TimeBaseStructure.TIM_Prescaler = prescaler - 1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);

    TIM_ARRPreloadConfig(TIMx, ENABLE);
    TIM_Cmd(TIMx, ENABLE); //

}
void Pwm::init_info(Gpio *pwm_pin)
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
        TIMx = TIM1;
        ch = TIMxCH2;//irq = TIM4_IRQn;
        break;
    case PA10_ID:
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

void Pwm::set_oc_polarity(uint8_t flag)
{

    if(flag == 1)
        this->oc_polarity = TIM_OCPolarity_High;
    else if(flag == 0)
        this->oc_polarity = TIM_OCPolarity_Low;
    _set_duty(duty);

}
void Pwm::set_frq(uint32_t frq)
{
    uint32_t period  = 0;
    uint32_t prescaler = 1;
    
    if(frq >= get_max_frq())//����Ƶ�ʣ���֤����1%����
        frq = get_max_frq();

    
    //ǧ��֮һ���ȷ��䷽��
    for(; prescaler <= 0xffff; prescaler++)
    {
        period = get_timer_source_clock() / prescaler / frq;
        if(period <= 65535)
        {
            if((0xffff >= period) && (period >= 1000))
            {
                accuracy = 1;
                break;
            }
        }
    }
    
    if(prescaler == 65536)//�����㷨����ʧ��
    {
        //�ٷ�֮һ���䷽��
        for(prescaler = 1; prescaler <= 0xffff; prescaler++)
        {
            period = get_timer_source_clock() / prescaler / frq;
            if(period <= 65535)
            {
                if((0xffff >= period) && (period >= 100))
                {
                    accuracy = 2;
                    break;
                }         
            }
        }
    } 
    if(prescaler == 65536)//�����㷨����ʧ��
    {
        //�ٷ�֮�����䷽��
        for(prescaler = 1; prescaler <= 0xffff; prescaler++)
        {
            period = get_timer_source_clock() / prescaler / frq;
            if(period <= 65535)
            {
                if((0xffff >= period) && (period >= 50))
                {
                    accuracy = 3;
                    break;
                }   
            }            
        }
    }     
    base_init(period, prescaler);
    _set_duty(duty);

}
//duty:0-1000��Ӧ0%-100.0%
void Pwm::_set_duty(uint16_t duty)
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
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputState_Disable;
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

//duty:0-1000��Ӧ0%-100.0%
void Pwm::set_duty(uint16_t duty)
{
    this->duty = duty;

    uint16_t pulse = 0;
    float percent;

    if(this->duty > 1000)
        this->duty = 1000;
    percent = this->duty / 1000.0;

    pulse = (uint16_t) (( percent  * period ));

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



uint32_t Pwm::get_timer_source_clock()
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
        if(temp & 0x00000400)//���PCLK�Ƿ���й���Ƶ��������й���Ƶ��ʱ����Ƶ��ΪPCLK1������
            timer_clock = cpu.clock.pclk1 * 2;
        else
            timer_clock = cpu.clock.pclk1 ;
    }
    return timer_clock;
}
uint32_t Pwm::get_max_frq()
{
    return get_timer_source_clock()/50;

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
        case 3:
            return 0.02;

    }
    return 0.001;


}
void Pwm::enable_pin()
{
    pin->mode(AF_PP);
}
void Pwm::disable_pin()
{
    pin->mode(INPUT);
}
void Pwm::end()
{
    
}
//duty:0-1000��Ӧ0%-100.0%
void analog_write(Gpio *pwm_pin, uint16_t duty)
{
    Pwm p(pwm_pin);
    p.begin(1000, duty);
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
