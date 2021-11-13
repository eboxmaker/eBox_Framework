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

/** @defgroup analog˽�к궨��
* @{
*/
// pin.id Ϊ8bit����4λ��ʶport 0-5,��4λΪio,0-15
#define GETEXTIPORT(a) ((uint32_t)a >>4 )
#define GETEXTILINE(a) ((uint32_t)1<<( a & 0x0f ))
#define GETPINNUMBER(a)(a & 0x0f)
/**
  * @}
  */
/** @defgroup extiȫ�ֱ���
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
 * @brief    Exti���캯����ʵ����һ������
 * @param    *pin: ָ�����ⲿ�ж�����PA0~PG15
 * @param    trigger: ���Ŵ����е�������������һ������ģʽ�е�һ�֣�
 *           - RISING: �����ش����ж�
 *           - FALLING: �½��ش����ж�
 *           - CHANGE: �����غ��½��ؾ������ж�
 * @return   NONE
 */
Exti::Exti(Gpio *pin)
{
    this->pin = pin;
    port_source = (uint32_t)pin->id >> 4;
    pin_source = pin->id & 0x0f;
    exti_line = 1 << pin_source;
    exti_irq_init(this->pin_source, (&Exti::_irq_handler), (uint32_t)this);

}

/**
 * @brief   ��ʼ���ⲿ�ж����ŵ�������Ϣ��
 * @param   NONE
 *
 * @return  NONE
 * @note    ��ʼ����Ĭ�Ͽ����жϣ�����û�������жϣ�
 *          ���Ե���interrupt(DISABLE)�ر��жϡ�
 */
void Exti::begin(PinMode_t mode, Mode_t extiMode)
{

    pin->mode((mode == INPUT) ? (INPUT_PU) : (mode));


    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    switch (extiMode)
    {
    case ModeIt:
        SET_BIT(EXTI->IMR, exti_line);
        CLEAR_BIT(EXTI->EMR, exti_line);
        break;
    case ModeEvent:
        SET_BIT(EXTI->EMR, exti_line);
        CLEAR_BIT(EXTI->IMR, exti_line);
        break;
    case ModeItEvent:

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
 * @brief   �ⲿ�ж����ŵ��ж�������ֹ���ƺ���
 * @param   enable: ������߽�ֹ�ж�
 *          - ENABLE: ������ⲿ�ж�
 *          - DISABLE: ��ֹ���ⲿ�ж�
 *
 * @return  NONE
 */
void Exti::interrupt(Trig_t trig,FunctionalState enable)
{

    EXTI_InitTypeDef EXTI_InitStructure;
    switch(trig)
    {
    case TrigNone:
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //?????????????????
        break;
    case TrigFall:
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½����ж�
        break; 
    case TrigRise:
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //���������ж�
        break;
    case TrigFallRise:
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //�������½��ض��ж�
        break;
    default:
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
 * @brief   ���жϵĻص������������жϺ����ִ��callback_fun()
 * @param   void (*fptr)(void)���ͺ�����ָ�롣
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
