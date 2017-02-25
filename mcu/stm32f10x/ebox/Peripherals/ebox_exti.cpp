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
  
/**
  * @}
  */
/** @defgroup extiȫ�ֱ���
  * @{
  */
static exti_irq_handler irq_handler;
static uint32_t exti_irq_ids[16];
int exti_irq_init(uint8_t index,exti_irq_handler handler,uint32_t id)
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
 *           - EXTI_Trigger_Rising: �����ش����ж�
 *           - EXTI_Trigger_Falling: �½��ش����ж�
 *           - EXTI_Trigger_Rising_Falling: �����غ��½��ؾ������ж�
 * @return   NONE
 */
Exti::Exti(Gpio *pin, EXTITrigger_TypeDef trigger)
{


    this->exti_pin = pin;
    this->trigger = trigger;

}

/**
 * @brief   ��ʼ���ⲿ�ж����ŵ�������Ϣ��
 * @param   NONE
 *          
 * @return  NONE
 * @note    ��ʼ����Ĭ�Ͽ����жϣ�����û�������жϣ�
 *          ���Ե���interrupt(DISABLE)�ر��жϡ�
 */ 
void Exti::begin()
{
    
	
    init_info(exti_pin);
    exti_irq_init(this->pin_source,(&Exti::_irq_handler),(uint32_t)this);

    exti_pin->mode(INPUT);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    EXTI_InitTypeDef EXTI_InitStructure;
    /* EXTI line(PB0) mode config */
    GPIO_EXTILineConfig(port_source, pin_source);
    EXTI_InitStructure.EXTI_Line = exti_line;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = trigger; //�½����ж�
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    interrupt(ENABLE);

}

/**
 * @brief   �ⲿ�ж����ŵ��ж�������ֹ���ƺ���
 * @param   enable: ������߽�ֹ�ж�
 *          - ENABLE: ������ⲿ�ж�   
 *          - DISABLE: ��ֹ���ⲿ�ж�   
 *          
 * @return  NONE
 */ 
void Exti::interrupt(FunctionalState enable, uint8_t preemption_priority, uint8_t sub_priority)
{
    if(preemption_priority > 4)preemption_priority = 4;
    if(sub_priority > 4)sub_priority = 4;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Configure one bit for preemption priority */
    //  NVIC_PriorityGroupConfig(NVIC_GROUP_CONFIG);//ʹ��ȫ�ֿ���ֵ

    NVIC_InitStructure.NVIC_IRQChannel = irq;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = preemption_priority;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = sub_priority;
    NVIC_InitStructure.NVIC_IRQChannelCmd = enable;
    NVIC_Init(&NVIC_InitStructure);

}

/**
 * @brief   ��ʼ����������Ӧ���жϿ���������
 * @param   *pin: �ⲿ�ж�����Ӧ������PA0~PG15
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

void Exti::_irq_handler( uint32_t id)
{ 
    Exti *handler = (Exti*)id;
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
