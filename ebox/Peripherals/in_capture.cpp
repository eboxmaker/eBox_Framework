/*
file   : in_capture.cpp
author : shentq
version: V1.1
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/

#include "in_capture.h"
extern callback_fun_type timx_cb_table[7][5];
extern uint16_t t2_overflow_times ;
extern uint16_t t3_overflow_times ;
extern uint16_t t4_overflow_times ;


IN_CAPTURE::IN_CAPTURE(GPIO *capture_pin)
{
    this->capture_pin = capture_pin;
    this->period = 0xffff;
    this->prescaler = 72;
    this->polarity = TIM_ICPOLARITY_FALLING;
}
void IN_CAPTURE::begin(uint16_t prescaler)
{
    capture_pin->mode(INPUT_PU);
    init_info(this->capture_pin);

    this->prescaler = prescaler;
    base_init(this->period, this->prescaler);

    switch(ch)
    {
    case TIM_Channel_1:
        _get_capture = TIM_GetCapture1;
        _set_polarity = TIM_OC1PolarityConfig;
        break;
    case TIM_Channel_2:
        _get_capture = TIM_GetCapture2;
        _set_polarity = TIM_OC2PolarityConfig;
        break;
    case TIM_Channel_3:
        _get_capture = TIM_GetCapture3;
        _set_polarity = TIM_OC3PolarityConfig;
        break;
    case TIM_Channel_4:
        _get_capture = TIM_GetCapture4;
        _set_polarity = TIM_OC4PolarityConfig;
        break;
    }
    switch((uint32_t)TIMx)
    {
    case (uint32_t)TIM2_BASE:
        overflow_times = &t2_overflow_times;
        break;
    case (uint32_t)TIM3_BASE:
        overflow_times = &t3_overflow_times;
        break;
    case (uint32_t)TIM4_BASE:
        overflow_times = &t4_overflow_times;
        break;

    }

}
void IN_CAPTURE::base_init(uint16_t period, uint16_t prescaler)
{
    this->period = period;//更新period
    this->prescaler = prescaler;//更新prescaler

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIMx_ICInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;


    switch((uint32_t)this->TIMx)
    {
    //		case (uint32_t)TIM1_BASE:
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
    }

    TIM_TimeBaseStructure.TIM_Period = this->period - 1; //ARR
    TIM_TimeBaseStructure.TIM_Prescaler = this->prescaler - 1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //
    TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);

    TIM_ARRPreloadConfig(TIMx, ENABLE);


    //初始化TIMx输入捕获参数 通道2
    TIMx_ICInitStructure.TIM_Channel = ch; //CC1S=01 	选择输入端 IC1映射到TI1上
    TIMx_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;	//上升沿捕获
    TIMx_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
    TIMx_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	  //配置输入分频,不分频
    TIMx_ICInitStructure.TIM_ICFilter = 0x00;	  //IC1F=0000 配置输入滤波器 不滤波
    TIM_ICInit(this->TIMx, &TIMx_ICInitStructure);
    TIM_ClearFlag(this->TIMx, TIM_FLAG_Update);


    //中断分组初始化
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级1级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级0级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure);   //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

    switch(ch)
    {
    case TIM_Channel_1:
        TIM_ClearFlag(this->TIMx, TIM_FLAG_CC1);
        TIM_ITConfig(this->TIMx,  TIM_IT_CC1 | TIM_IT_Update, ENABLE);   //使能TIMx的通道2捕获中断
        break;
    case TIM_Channel_2:
        TIM_ClearFlag(this->TIMx, TIM_FLAG_CC2);
        TIM_ITConfig(this->TIMx,  TIM_IT_CC2 | TIM_IT_Update, ENABLE);   //使能TIMx的通道2捕获中断
        break;
    case TIM_Channel_3:
        TIM_ClearFlag(this->TIMx, TIM_FLAG_CC3);
        TIM_ITConfig(this->TIMx,  TIM_IT_CC3 | TIM_IT_Update, ENABLE);   //使能TIMx的通道2捕获中断
        break;
    case TIM_Channel_4:
        TIM_ClearFlag(this->TIMx, TIM_FLAG_CC4);
        TIM_ITConfig(this->TIMx,  TIM_IT_CC4 | TIM_IT_Update, ENABLE);   //使能TIMx的通道2捕获中断
        break;
    }

    TIM_Cmd(TIMx, ENABLE); //

}
void IN_CAPTURE::init_info(GPIO *capture_pin)
{
    if(this->capture_pin->port == GPIOA)
    {
        switch(this->capture_pin->pin)
        {
        case GPIO_Pin_0:
            TIMx = TIM2;
            ch = TIM_Channel_1;//irq = TIM2_IRQn;
            break;
        case GPIO_Pin_1:
            TIMx = TIM2;
            ch = TIM_Channel_2;//irq = TIM2_IRQn;
            break;
        case GPIO_Pin_2:
            TIMx = TIM2;
            ch = TIM_Channel_3;//irq = TIM2_IRQn;
            break;
        case GPIO_Pin_3:
            TIMx = TIM2;
            ch = TIM_Channel_4;//irq = TIM2_IRQn;
            break;

        case GPIO_Pin_6:
            TIMx = TIM3;
            ch = TIM_Channel_1;//irq = TIM3_IRQn;
            break;
        case GPIO_Pin_7:
            TIMx = TIM3;
            ch = TIM_Channel_2;//irq = TIM3_IRQn;
            break;
            //            case GPIO_Pin_10:
            //				TIMx = TIM3;ch = TIM_Channel_3;//irq = TIM3_IRQn;
            //				break;
            //			case GPIO_Pin_11:
            //				TIMx = TIM3;ch = TIM_Channel_4;//irq = TIM3_IRQn;
            //				break;

        }
    }
    if(this->capture_pin->port == GPIOB)
    {
        switch(this->capture_pin->pin)
        {
        case GPIO_Pin_0:
            TIMx = TIM3;
            ch = TIM_Channel_3;//irq = TIM3_IRQn;
            break;
        case GPIO_Pin_1:
            TIMx = TIM3;
            ch = TIM_Channel_4;//irq = TIM3_IRQn;
            break;

        case GPIO_Pin_6:
            TIMx = TIM4;
            ch = TIM_Channel_1;//irq = TIM4_IRQn;
            break;
        case GPIO_Pin_7:
            TIMx = TIM4;
            ch = TIM_Channel_2;//irq = TIM4_IRQn;
            break;
        case GPIO_Pin_8:
            TIMx = TIM4;
            ch = TIM_Channel_3;//irq = TIM4_IRQn;
            break;
        case GPIO_Pin_9:
            TIMx = TIM4;
            ch = TIM_Channel_4;//irq = TIM4_IRQn;
            break;
        }
    }

}

void IN_CAPTURE::set_polarity_falling()
{
    this->polarity = TIM_ICPOLARITY_FALLING;
    _set_polarity(this->TIMx, this->polarity);//设置为下降沿捕获

}
void IN_CAPTURE::set_polarity_rising()
{
    this->polarity = TIM_ICPOLARITY_RISING;
    _set_polarity(this->TIMx, this->polarity);//设置为下降沿捕获

}

uint32_t IN_CAPTURE::get_capture()
{
    uint32_t capture = 0;
    uint32_t now = 0;
    now = _get_capture( this->TIMx ) + (*overflow_times) * this->period;  //get capture value
    if(now > last_value)
        capture = now - last_value;
    else
        capture = 0xffffffff + now - last_value;
    last_value = now;
    return capture;
}
float IN_CAPTURE::get_zone_time_us()
{
    return get_capture()/(72.0/this->prescaler);
}

void IN_CAPTURE::set_count(uint16_t count)
{
    TIM_SetCounter(this->TIMx, count); //reset couter

}

void IN_CAPTURE::attch_update_interrupt(void(*callback)(void))
{
    switch((uint32_t)this->TIMx)
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
    default:
        break;
    }

}

void IN_CAPTURE::attch_ic_interrupt(void(*callback)(void))
{
    switch((uint32_t)this->TIMx)
    {
    case (uint32_t)TIM2_BASE:
        switch(ch)
        {
        case TIM_Channel_1:
            timx_cb_table[1][1] = callback;
            break;
        case TIM_Channel_2:
            timx_cb_table[1][2] = callback;
            break;
        case TIM_Channel_3:
            timx_cb_table[1][3] = callback;
            break;
        case TIM_Channel_4:
            timx_cb_table[1][4] = callback;
            break;
        default:
            break;
        }
        break;
    case (uint32_t)TIM3_BASE:
        switch(ch)
        {
        case TIM_Channel_1:
            timx_cb_table[2][1] = callback;
            break;
        case TIM_Channel_2:
            timx_cb_table[2][2] = callback;
            break;
        case TIM_Channel_3:
            timx_cb_table[2][3] = callback;
            break;
        case TIM_Channel_4:
            timx_cb_table[2][4] = callback;
            break;
        default:
            break;
        }
        break;
    case (uint32_t)TIM4_BASE:
        switch(ch)
        {
        case TIM_Channel_1:
            timx_cb_table[3][1] = callback;
            break;
        case TIM_Channel_2:
            timx_cb_table[3][2] = callback;
            break;
        case TIM_Channel_3:
            timx_cb_table[3][3] = callback;
            break;
        case TIM_Channel_4:
            timx_cb_table[3][4] = callback;
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}

