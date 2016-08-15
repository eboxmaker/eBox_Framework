/**
  ******************************************************************************
  * @file    exti.cpp
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
#include "exti.h"

/** @defgroup exti 
  * @brief exti driver modules
  * @{
  */

/**
  * @}
  */
  
  /** @defgroup analog私有宏定义
  * @{
  */  
  
#define EXTI_LINE_NUM 16

/**
  * @}
  */
/** @defgroup exti全局变量
  * @{
  */
  
callback_fun_type exti_callback_table[EXTI_LINE_NUM];
  
/**
  * @}
  */



/**
 * @brief    Exti构造函数，实例化一个对象
 * @param    *pin: 指定的外部中断引脚PA0~PG15
 * @param    trigger: 引脚触发中的条件，可以是一下三种模式中的一种：
 *           - EXTI_Trigger_Rising: 上升沿触发中断
 *           - EXTI_Trigger_Falling: 下降沿触发中断
 *           - EXTI_Trigger_Rising_Falling: 上升沿和下降沿均触发中断
 * @return   NONE
 */
Exti::Exti(Gpio *pin, EXTITrigger_TypeDef trigger)
{


    this->exti_pin = pin;
    this->trigger = trigger;

}

/**
 * @brief   初始化外部中断引脚的配置信息。
 * @param   NONE
 *          
 * @return  NONE
 * @note    初始化会默认开启中断，如果用户想禁用中断，
 *          可以调用interrupt(DISABLE)关闭中断。
 */ 
void Exti::begin()
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
    interrupt(ENABLE);

}

/**
 * @brief   外部中断引脚的中断允许、禁止控制函数
 * @param   enable: 允许或者禁止中断
 *          - ENABLE: 允许该外部中断   
 *          - DISABLE: 禁止该外部中断   
 *          
 * @return  NONE
 */ 
void Exti::interrupt(FunctionalState enable)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Configure one bit for preemption priority */
    //  NVIC_PriorityGroupConfig(NVIC_GROUP_CONFIG);//使用全局控制值

    NVIC_InitStructure.NVIC_IRQChannel = irq;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = enable;
    NVIC_Init(&NVIC_InitStructure);

}

/**
 * @brief   初始化引脚所对应的中断控制器参数
 * @param   *pin: 外部中断所对应的引脚PA0~PG15
 *          
 * @return  NONE
 */ 
void Exti::init_info(Gpio *pin)
{
    switch((uint32_t)pin->id>>8)
    {
    case (uint32_t)0:
        port_source = GPIO_PortSourceGPIOA;
        break;
    case (uint32_t)1:
        port_source = GPIO_PortSourceGPIOB;
        break;
    case (uint32_t)2:
        port_source = GPIO_PortSourceGPIOC;
        break;
    case (uint32_t)3:
        port_source = GPIO_PortSourceGPIOD;
        break;
    case (uint32_t)4:
        port_source = GPIO_PortSourceGPIOE;
        break;
    case (uint32_t)5:
        port_source = GPIO_PortSourceGPIOF;
        break;
    }
    switch(pin->id&0x0f)
    {
    case 0:
        pin_source = GPIO_PinSource0;
        exti_line = EXTI_Line0;
        irq = EXTI0_IRQn;
        break;

    case 1:
        pin_source = GPIO_PinSource1;
        exti_line = EXTI_Line1;
        irq = EXTI1_IRQn;
        break;

    case 2:
        pin_source = GPIO_PinSource2;
        exti_line = EXTI_Line2;
        irq = EXTI2_IRQn;
        break;

    case 3:
        pin_source = GPIO_PinSource3;
        exti_line = EXTI_Line3;
        irq = EXTI3_IRQn;
        break;

    case 4:
        pin_source = GPIO_PinSource4;
        exti_line = EXTI_Line4;
        irq = EXTI4_IRQn;
        break;

    case 5:
        pin_source = GPIO_PinSource5;
        exti_line = EXTI_Line5;
        irq = EXTI9_5_IRQn;
        break;

    case 6:
        pin_source = GPIO_PinSource6;
        exti_line = EXTI_Line6;
        irq = EXTI9_5_IRQn;
        break;

    case 7:
        pin_source = GPIO_PinSource7;
        exti_line = EXTI_Line7;
        irq = EXTI9_5_IRQn;
        break;

    case 8:
        pin_source = GPIO_PinSource8;
        exti_line = EXTI_Line8;
        irq = EXTI9_5_IRQn;
        break;

    case 9:
        pin_source = GPIO_PinSource9;
        exti_line = EXTI_Line9;
        irq = EXTI9_5_IRQn;
        break;

    case 10:
        pin_source = GPIO_PinSource10;
        exti_line = EXTI_Line10;
        irq = EXTI15_10_IRQn;
        break;

    case 11:
        pin_source = GPIO_PinSource11;
        exti_line = EXTI_Line11;
        irq = EXTI15_10_IRQn;
        break;

    case 12:
        pin_source = GPIO_PinSource12;
        exti_line = EXTI_Line12;
        irq = EXTI15_10_IRQn;
        break;

    case 13:
        pin_source = GPIO_PinSource13;
        exti_line = EXTI_Line13;
        irq = EXTI15_10_IRQn;
        break;

    case 14:
        pin_source = GPIO_PinSource14;
        exti_line = EXTI_Line14;
        irq = EXTI15_10_IRQn;
        break;

    case 15:
        pin_source = GPIO_PinSource15;
        exti_line = EXTI_Line15;
        irq = EXTI15_10_IRQn;
        break;

    }

}

/**
 * @brief   绑定中断的回调函数，产生中断后程序将执行callback_fun()
 * @param   callback_fun: void (*callback_fun)(void)类型函数的指针。
 *          
 * @return  NONE
 */ 
void Exti::attach_interrupt(void (*callback_fun)(void))
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
