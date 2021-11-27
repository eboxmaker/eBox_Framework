/**
  ******************************************************************************
  * @file    exti.cpp
  * @author  shentq
  * @version V2.1
  * @date    2016/08/14
  * @brief
  *         2018/10/2         LQM
  *         ���������أ��½��ذ󶨲�ͬ���жϻص�����
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

#if EBOX_DEBUG
// �Ƿ��ӡ������Ϣ, 1��ӡ,0����ӡ
#define debug 0
#endif

#if debug
#define  EXTI_DEBUG(...) DBG("[EXTI]  "),DBG(__VA_ARGS__)
#else
#define  EXTI_DEBUG(...)
#endif


// pin.id Ϊ8bit����4λ��ʶport 0-5,��4λΪio,0-15
#define GETEXTIPORT(a) ((uint32_t)a >>4 )
#define GETEXTILINE(a) ((uint32_t)1<<( a & 0x0f ))
#define GETPINNUMBER(a)(a & 0x0f)


// uint32_t pObj �����
typedef void (*exti_irq_handler)(uint32_t pObj);

static exti_irq_handler  exti_handler;   // ��������ָ�������ָ����ľ�̬��Ա
static uint32_t  exti_irq_ids[16];    	 // ��������ַ������̬��Աʶ����󣬲����ʶ������ͨ��Ա

void exti_irq_init(uint8_t index, exti_irq_handler handler, uint32_t id)
{
    exti_irq_ids[index] = id;       // ��������ַ
    exti_handler =  handler;        // ָ��ص�����
}


/**
 * @brief    Exti���캯����ʵ����һ������
 * @param    *pin: ָ�����ⲿ�ж�����PA0~PG15
 * @return   NONE
 */
Exti::Exti(Gpio *pin)
{
    _pin = pin;
    _extiLine = GETEXTILINE(_pin->id);
    exti_irq_init(GETPINNUMBER(_pin->id), (&Exti::_irq_handler), (uint32_t)this);
}
/**
 * @brief    Exti���캯����ʵ����һ������
 * @param    mode: gpioģʽ��type �ж����ͣ�IT��EVENT,IT_EVENT��Ĭ��Ϊ�ж�
 * @return   NONE
 */
void Exti::begin(PinMode_t mode, Mode_t extiMode)
{
    // f1ϵ�в�������Ϊ
    _pin->mode((mode == INPUT) ? (INPUT_PU) : (mode));

    //  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SYSCFG);
    GPIO_EXTILineConfig(GETEXTIPORT(_pin->id), GETPINNUMBER(_pin->id));

    EXTI_DEBUG("extiLine is %d , %d\n", _extiLine, GETEXTILINE(_pin->id));
    EXTI_DEBUG("pinNumber is %d\n", GETPINNUMBER(_pin->id));

    switch (extiMode)
    {
    case ModeIt:
        SET_BIT(EXTI->IMR, _extiLine);
        CLEAR_BIT(EXTI->EMR, _extiLine);
        break;
    case ModeEvent:
        SET_BIT(EXTI->EMR, _extiLine);
        CLEAR_BIT(EXTI->IMR, _extiLine);
        break;
    case ModeItEvent:
        SET_BIT(EXTI->EMR, _extiLine);
        SET_BIT(EXTI->IMR, _extiLine);
        break;
    default:
        SET_BIT(EXTI->IMR, _extiLine);
        CLEAR_BIT(EXTI->EMR, _extiLine);
        break;
    }
    nvic(ENABLE);
}

void Exti::nvic(FunctionalState enable, uint8_t preemption_priority, uint8_t sub_priority )
{
    nvic_dev_set_priority((uint32_t)_extiLine,0,0,0);
    if(enable != DISABLE)
        nvic_dev_enable((uint32_t)_extiLine,0);
    else
        nvic_dev_disable((uint32_t)_extiLine,0);
}


void Exti::interrupt(Trig_t trig, FunctionalState enable)
{

    switch (trig) // ʹ�ܴ�������
    {
    case TrigFall:
        bit_write(EXTI->FTSR,GETPINNUMBER(_pin->id),enable);
        break;
    case TrigRise:
        bit_write(EXTI->RTSR,GETPINNUMBER(_pin->id),enable);
        break;
    case TrigFallRise:
        bit_write(EXTI->FTSR,GETPINNUMBER(_pin->id),enable);
        bit_write(EXTI->RTSR,GETPINNUMBER(_pin->id),enable);
        break;
    default:
        break;
    }
}

void Exti::soft_triger()
{
    bit_set(EXTI->SWIER,GETPINNUMBER(_pin->id));
}

/**
 *@brief    EXTI ��̬��Ա���������ж��е��ã�����ִ����ػص�����
 *@param    uint32_t id �����ַ������ʶ�����IrqType irq_type �ж�����
 *@retval   NONE
*/
void Exti::_irq_handler(uint32_t pObj)
{
    Exti *handler = (Exti *)pObj; // ָ��ص�������ַ
    handler->_pirq.call();

}
/**
 * @brief   ���жϵĻص������������жϺ����ִ��callback_fun()
 * @param   void (*fptr)(void)���ͺ�����ָ�롣
 *
 * @return  NONE
 */
void Exti::attach(void (*fptr)(void))
{
    _pirq.attach(fptr);
}

extern "C" {

    void EXTI0_IRQHandler(void)
    {
        if (EXTI_GetITStatus(EXTI_Line0) != RESET)
        {
            exti_handler(exti_irq_ids[0]);
            EXTI_ClearITPendingBit(EXTI_Line0);
        }
    }
    void EXTI1_IRQHandler(void)
    {
        if (EXTI_GetITStatus(EXTI_Line1) != RESET)
        {
            exti_handler(exti_irq_ids[1]);
            EXTI_ClearITPendingBit(EXTI_Line1);
        }
    }
    void EXTI2_IRQHandler(void)
    {
        if (EXTI_GetITStatus(EXTI_Line2) != RESET)
        {
            exti_handler(exti_irq_ids[2]);
            EXTI_ClearITPendingBit(EXTI_Line2);
        }
    }
    void EXTI3_IRQHandler(void)
    {
        if (EXTI_GetITStatus(EXTI_Line3) != RESET)
        {
            exti_handler(exti_irq_ids[3]);
            EXTI_ClearITPendingBit(EXTI_Line3);
        }
    }
    void EXTI4_IRQHandler(void)
    {
        if (EXTI_GetITStatus(EXTI_Line4) != RESET)
        {
            exti_handler(exti_irq_ids[4]);
            EXTI_ClearITPendingBit(EXTI_Line4);
        }
    }

    void EXTI9_5_IRQHandler(void)
    {
        for (uint8_t i = 5; i <= 9; i++)
        {
            if (EXTI_GetITStatus(1 << i) != RESET)
            {
                exti_handler(exti_irq_ids[i]);
                EXTI_ClearITPendingBit(1 << i);
            }
        }
    }

    void EXTI15_10_IRQHandler(void)
    {
        for (uint8_t i = 10; i <= 15; i++)
        {
            if (EXTI_GetITStatus(1 << i) != RESET)
            {
                exti_handler(exti_irq_ids[i]);
                EXTI_ClearITPendingBit(1 << i);
            }
        }
    }

}
