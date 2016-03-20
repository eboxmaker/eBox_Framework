/*
file   : exti.cpp
author : shentq
version: V1.1
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/

#include "exti.h"

#define EXTI_LINE_NUM 16

callback_fun_type exti_callback_table[EXTI_LINE_NUM];


//  trigger	:
// 				 	EXTI_Trigger_Rising
//  				EXTI_Trigger_Falling
//  				EXTI_Trigger_Rising_Falling

EXTIx::EXTIx(GPIO *exti_pin, EXTITrigger_TypeDef trigger)
{


    this->exti_pin = exti_pin;
    this->trigger = trigger;

}
void EXTIx::begin()
{
    init_info(exti_pin);

    exti_pin->mode(INPUT);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    EXTI_InitTypeDef EXTI_InitStructure;
    /* EXTI line(PB0) mode config */
    GPIO_EXTILineConfig(port_source, pin_source);
    EXTI_InitStructure.EXTI_Line = exti_line;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = trigger; //下降沿中断
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

}
void EXTIx::interrupt(FunctionalState enable)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Configure one bit for preemption priority */
    //  NVIC_PriorityGroupConfig(NVIC_GROUP_CONFIG);//使用全局控制值

    /* 配置P[A|B|C|D|E]0为中断源 */
    NVIC_InitStructure.NVIC_IRQChannel = irq;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = enable;
    NVIC_Init(&NVIC_InitStructure);

}

void EXTIx::init_info(GPIO *exti_pin)
{
    switch((uint32_t)exti_pin->port)
    {
    case (uint32_t)GPIOA_BASE:
        port_source = GPIO_PortSourceGPIOA;
        break;
    case (uint32_t)GPIOB_BASE:
        port_source = GPIO_PortSourceGPIOB;
        break;
    case (uint32_t)GPIOC_BASE:
        port_source = GPIO_PortSourceGPIOC;
        break;
    case (uint32_t)GPIOD_BASE:
        port_source = GPIO_PortSourceGPIOD;
        break;
    case (uint32_t)GPIOE_BASE:
        port_source = GPIO_PortSourceGPIOE;
        break;
    case (uint32_t)GPIOF_BASE:
        port_source = GPIO_PortSourceGPIOF;
        break;
    }
    switch(exti_pin->pin)
    {
    case GPIO_Pin_0:
        pin_source = GPIO_PinSource0;
        exti_line = EXTI_Line0;
        irq = EXTI0_IRQn;
        break;

    case GPIO_Pin_1:
        pin_source = GPIO_PinSource1;
        exti_line = EXTI_Line1;
        irq = EXTI1_IRQn;
        break;

    case GPIO_Pin_2:
        pin_source = GPIO_PinSource2;
        exti_line = EXTI_Line2;
        irq = EXTI2_IRQn;
        break;

    case GPIO_Pin_3:
        pin_source = GPIO_PinSource3;
        exti_line = EXTI_Line3;
        irq = EXTI3_IRQn;
        break;

    case GPIO_Pin_4:
        pin_source = GPIO_PinSource4;
        exti_line = EXTI_Line4;
        irq = EXTI4_IRQn;
        break;

    case GPIO_Pin_5:
        pin_source = GPIO_PinSource5;
        exti_line = EXTI_Line5;
        irq = EXTI9_5_IRQn;
        break;

    case GPIO_Pin_6:
        pin_source = GPIO_PinSource6;
        exti_line = EXTI_Line6;
        irq = EXTI9_5_IRQn;
        break;

    case GPIO_Pin_7:
        pin_source = GPIO_PinSource7;
        exti_line = EXTI_Line7;
        irq = EXTI9_5_IRQn;
        break;

    case GPIO_Pin_8:
        pin_source = GPIO_PinSource8;
        exti_line = EXTI_Line8;
        irq = EXTI9_5_IRQn;
        break;

    case GPIO_Pin_9:
        pin_source = GPIO_PinSource9;
        exti_line = EXTI_Line9;
        irq = EXTI9_5_IRQn;
        break;

    case GPIO_Pin_10:
        pin_source = GPIO_PinSource10;
        exti_line = EXTI_Line10;
        irq = EXTI15_10_IRQn;
        break;

    case GPIO_Pin_11:
        pin_source = GPIO_PinSource11;
        exti_line = EXTI_Line11;
        irq = EXTI15_10_IRQn;
        break;

    case GPIO_Pin_12:
        pin_source = GPIO_PinSource12;
        exti_line = EXTI_Line12;
        irq = EXTI15_10_IRQn;
        break;

    case GPIO_Pin_13:
        pin_source = GPIO_PinSource13;
        exti_line = EXTI_Line13;
        irq = EXTI15_10_IRQn;
        break;

    case GPIO_Pin_14:
        pin_source = GPIO_PinSource14;
        exti_line = EXTI_Line14;
        irq = EXTI15_10_IRQn;
        break;

    case GPIO_Pin_15:
        pin_source = GPIO_PinSource15;
        exti_line = EXTI_Line15;
        irq = EXTI15_10_IRQn;
        break;

    }

}

void EXTIx::attach_interrupt(void (*callback_fun)(void))
{
    exti_callback_table[pin_source] = callback_fun;
}
extern "C" {

    void EXTI0_IRQHandler(void)
    {
        if(EXTI_GetITStatus(EXTI_Line0) != RESET)
        {
            exti_callback_table[0]();
            EXTI_ClearITPendingBit(EXTI_Line0);
        }
    }
    void EXTI1_IRQHandler(void)
    {
        if(EXTI_GetITStatus(EXTI_Line1) != RESET)
        {
            exti_callback_table[1]();

            EXTI_ClearITPendingBit(EXTI_Line1);
        }
    }
    void EXTI2_IRQHandler(void)
    {
        if(EXTI_GetITStatus(EXTI_Line2) != RESET)
        {

            exti_callback_table[2]();

            EXTI_ClearITPendingBit(EXTI_Line2);
        }
    }
    void EXTI3_IRQHandler(void)
    {
        if(EXTI_GetITStatus(EXTI_Line3) != RESET)
        {
            exti_callback_table[3]();
            EXTI_ClearITPendingBit(EXTI_Line3);
        }
    }
    void EXTI4_IRQHandler(void)
    {
        if(EXTI_GetITStatus(EXTI_Line4) != RESET)
        {
            exti_callback_table[4]();
            EXTI_ClearITPendingBit(EXTI_Line4);
        }
    }

    void EXTI9_5_IRQHandler(void)
    {
        if(EXTI_GetITStatus(EXTI_Line5) != RESET)
        {
            exti_callback_table[5]();
            EXTI_ClearITPendingBit(EXTI_Line5);
        }
        if(EXTI_GetITStatus(EXTI_Line6) != RESET)
        {
            exti_callback_table[6]();
            EXTI_ClearITPendingBit(EXTI_Line6);
        }
        if(EXTI_GetITStatus(EXTI_Line7) != RESET)
        {
            exti_callback_table[7]();
            EXTI_ClearITPendingBit(EXTI_Line7);
        }
        if(EXTI_GetITStatus(EXTI_Line8) != RESET)
        {
            exti_callback_table[8]();
            EXTI_ClearITPendingBit(EXTI_Line8);
        }
        if(EXTI_GetITStatus(EXTI_Line9) != RESET)
        {
            exti_callback_table[9]();
            EXTI_ClearITPendingBit(EXTI_Line9);
        }

    }

    void EXTI15_10_IRQHandler(void)
    {
        if(EXTI_GetITStatus(EXTI_Line10) != RESET)
        {
            exti_callback_table[10]();
            EXTI_ClearITPendingBit(EXTI_Line10);
        }
        if(EXTI_GetITStatus(EXTI_Line11) != RESET)
        {
            exti_callback_table[11]();
            EXTI_ClearITPendingBit(EXTI_Line11);
        }
        if(EXTI_GetITStatus(EXTI_Line12) != RESET)
        {
            exti_callback_table[12]();
            EXTI_ClearITPendingBit(EXTI_Line12);
        }
        if(EXTI_GetITStatus(EXTI_Line13) != RESET)
        {
            exti_callback_table[13]();
            EXTI_ClearITPendingBit(EXTI_Line13);
        }
        if(EXTI_GetITStatus(EXTI_Line14) != RESET)
        {
            exti_callback_table[14]();
            EXTI_ClearITPendingBit(EXTI_Line14);
        }
        if(EXTI_GetITStatus(EXTI_Line15) != RESET)
        {
            exti_callback_table[15]();
            EXTI_ClearITPendingBit(EXTI_Line15);
        }

    }

}
