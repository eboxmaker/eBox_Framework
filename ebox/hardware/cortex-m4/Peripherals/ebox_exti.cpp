/**
  ******************************************************************************
  * @file    exti.cpp
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
#include "ebox_exti.h"
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
// pin.id 为8bit，高4位标识port 0-5,低4位为io,0-15
#define GETEXTIPORT(a) ((uint32_t)a >>4 )
#define GETEXTILINE(a) ((uint32_t)1<<( a & 0x0f ))
#define GETPINNUMBER(a)(a & 0x0f)
/**
  * @}
  */
/** @defgroup exti全局变量
  * @{
  */
static exti_irq_handler irq_handler;
static uint32_t exti_irq_ids[16];
int exti_irq_init(uint8_t index, exti_irq_handler handler, uint32_t id)
{
    exti_irq_ids[index] = id;
    irq_handler =  handler;
    return 0;
}



/**
  * @}
  */



/**
 * @brief    Exti构造函数，实例化一个对象
 * @param    *pin: 指定的外部中断引脚PA0~PG15
 * @param    trigger: 引脚触发中的条件，可以是一下三种模式中的一种：
 *           - RISING: 上升沿触发中断
 *           - FALLING: 下降沿触发中断
 *           - CHANGE: 上升沿和下降沿均触发中断
 * @return   NONE
 */
Exti::Exti(Gpio *pin)
{
    this->pin = pin;
    exti_line = 1 << pin_source;
    exti_irq_init(this->pin_source, (&Exti::_irq_handler), (uint32_t)this);

}

/**
 * @brief   初始化外部中断引脚的配置信息。
 * @param   NONE
 *
 * @return  NONE
 * @note    初始化会默认开启中断，如果用户想禁用中断，
 *          可以调用interrupt(DISABLE)关闭中断。
 */
void Exti::begin(PinMode_t mode, Exti_t type)
{

    pin->mode((mode == INPUT) ? (INPUT_PU) : (mode));

//    port_source = (uint32_t)pin->id >> 4;
//    pin_source = pin->id & 0x0f;
//    GPIO_EXTILineConfig(GETEXTIPORT(pin->id), GETPINNUMBER(pin->id));

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    switch (type)
    {
    case IT:
        SET_BIT(EXTI->IMR, exti_line);
        CLEAR_BIT(EXTI->EMR, exti_line);
        break;
    case EVENT:
        SET_BIT(EXTI->EMR, exti_line);
        CLEAR_BIT(EXTI->IMR, exti_line);
        break;
    case IT_EVENT:

        SET_BIT(EXTI->EMR, exti_line);
        SET_BIT(EXTI->IMR, exti_line);
        break;
    default:
        SET_BIT(EXTI->IMR, exti_line);
        CLEAR_BIT(EXTI->EMR, exti_line);
        break;
    }
    nvic(ENABLE, 0, 0);

}

void Exti::nvic(FunctionalState enable, uint8_t preemption_priority, uint8_t sub_priority )
{
    nvic_dev_set_priority((uint32_t)exti_line, 0, 0, 0);
    if(enable != DISABLE)
        nvic_dev_enable((uint32_t)exti_line, 0);
    else
        nvic_dev_disable((uint32_t)exti_line, 0);
}

/**
 * @brief   外部中断引脚的中断允许、禁止控制函数
 * @param   enable: 允许或者禁止中断
 *          - ENABLE: 允许该外部中断
 *          - DISABLE: 禁止该外部中断
 *
 * @return  NONE
 */
void Exti::interrupt(Trigger_t type,FunctionalState enable)
{

    EXTI_InitTypeDef EXTI_InitStructure;
    switch(type)
    {
    case RISE:
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //上升沿沿中断
        break;
    case FALL:
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿中断
        break;
    case CHANGE:
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //上升，下降沿都中断
        break;
    }
    SYSCFG_EXTILineConfig(port_source, pin_source);
    EXTI_InitStructure.EXTI_Line = exti_line;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_LineCmd = enable;
    EXTI_Init(&EXTI_InitStructure);

}

void Exti::_irq_handler( uint32_t id)
{
    Exti *handler = (Exti *)id;
    handler->_irq.call();
}
/**
 * @brief   绑定中断的回调函数，产生中断后程序将执行callback_fun()
 * @param   void (*fptr)(void)类型函数的指针。
 *
 * @return  NONE
 */
void Exti::attach(void (*fptr)(void))
{
    if (fptr)
    {
        _irq.attach(fptr);
    }
}


extern "C" {

    void EXTI0_IRQHandler(void)
    {
        if(EXTI_GetITStatus(EXTI_Line0) != RESET)
        {
            irq_handler(exti_irq_ids[0]);
            EXTI_ClearITPendingBit(EXTI_Line0);
        }
    }
    void EXTI1_IRQHandler(void)
    {
        if(EXTI_GetITStatus(EXTI_Line1) != RESET)
        {
            irq_handler(exti_irq_ids[1]);
            EXTI_ClearITPendingBit(EXTI_Line1);
        }
    }
    void EXTI2_IRQHandler(void)
    {
        if(EXTI_GetITStatus(EXTI_Line2) != RESET)
        {
            irq_handler(exti_irq_ids[2]);
            EXTI_ClearITPendingBit(EXTI_Line2);
        }
    }
    void EXTI3_IRQHandler(void)
    {
        if(EXTI_GetITStatus(EXTI_Line3) != RESET)
        {
            irq_handler(exti_irq_ids[3]);
            EXTI_ClearITPendingBit(EXTI_Line3);
        }
    }
    void EXTI4_IRQHandler(void)
    {
        if(EXTI_GetITStatus(EXTI_Line4) != RESET)
        {
            irq_handler(exti_irq_ids[4]);
            EXTI_ClearITPendingBit(EXTI_Line4);
        }
    }

    void EXTI9_5_IRQHandler(void)
    {
        if(EXTI_GetITStatus(EXTI_Line5) != RESET)
        {
            irq_handler(exti_irq_ids[5]);
            EXTI_ClearITPendingBit(EXTI_Line5);
        }
        if(EXTI_GetITStatus(EXTI_Line6) != RESET)
        {
            irq_handler(exti_irq_ids[6]);
            EXTI_ClearITPendingBit(EXTI_Line6);
        }
        if(EXTI_GetITStatus(EXTI_Line7) != RESET)
        {
            irq_handler(exti_irq_ids[7]);
            EXTI_ClearITPendingBit(EXTI_Line7);
        }
        if(EXTI_GetITStatus(EXTI_Line8) != RESET)
        {
            irq_handler(exti_irq_ids[8]);
            EXTI_ClearITPendingBit(EXTI_Line8);
        }
        if(EXTI_GetITStatus(EXTI_Line9) != RESET)
        {
            irq_handler(exti_irq_ids[9]);
            EXTI_ClearITPendingBit(EXTI_Line9);
        }

    }

    void EXTI15_10_IRQHandler(void)
    {
        if(EXTI_GetITStatus(EXTI_Line10) != RESET)
        {
            irq_handler(exti_irq_ids[10]);
            EXTI_ClearITPendingBit(EXTI_Line10);
        }
        if(EXTI_GetITStatus(EXTI_Line11) != RESET)
        {
            irq_handler(exti_irq_ids[11]);
            EXTI_ClearITPendingBit(EXTI_Line11);
        }
        if(EXTI_GetITStatus(EXTI_Line12) != RESET)
        {
            irq_handler(exti_irq_ids[12]);
            EXTI_ClearITPendingBit(EXTI_Line12);
        }
        if(EXTI_GetITStatus(EXTI_Line13) != RESET)
        {
            irq_handler(exti_irq_ids[13]);
            EXTI_ClearITPendingBit(EXTI_Line13);
        }
        if(EXTI_GetITStatus(EXTI_Line14) != RESET)
        {
            irq_handler(exti_irq_ids[14]);
            EXTI_ClearITPendingBit(EXTI_Line14);
        }
        if(EXTI_GetITStatus(EXTI_Line15) != RESET)
        {
            irq_handler(exti_irq_ids[15]);
            EXTI_ClearITPendingBit(EXTI_Line15);
        }

    }

}
