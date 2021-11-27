/**
  ******************************************************************************
  * @file    exti.cpp
  * @author  shentq
  * @version V2.1
  * @date    2016/08/14
  * @brief
  *         2018/10/2         LQM
  *         允许上升沿，下降沿绑定不同的中断回调函数
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
// 是否打印调试信息, 1打印,0不打印
#define debug 0
#endif

#if debug
#define  EXTI_DEBUG(...) DBG("[EXTI]  "),DBG(__VA_ARGS__)
#else
#define  EXTI_DEBUG(...)
#endif


// pin.id 为8bit，高4位标识port 0-5,低4位为io,0-15
#define GETEXTIPORT(a) ((uint32_t)a >>4 )
#define GETEXTILINE(a) ((uint32_t)1<<( a & 0x0f ))
#define GETPINNUMBER(a)(a & 0x0f)


// uint32_t pObj 类对象
typedef void (*exti_irq_handler)(uint32_t pObj);

static exti_irq_handler  exti_handler;   // 声明函数指针变量，指向类的静态成员
static uint32_t  exti_irq_ids[16];    	 // 保存对象地址，供静态成员识别对象，并访问对象的普通成员

void exti_irq_init(uint8_t index, exti_irq_handler handler, uint32_t id)
{
    exti_irq_ids[index] = id;       // 保存对象地址
    exti_handler =  handler;        // 指向回调函数
}


/**
 * @brief    Exti构造函数，实例化一个对象
 * @param    *pin: 指定的外部中断引脚PA0~PG15
 * @return   NONE
 */
Exti::Exti(Gpio *pin)
{
    _pin = pin;
    _extiLine = GETEXTILINE(_pin->id);
    exti_irq_init(GETPINNUMBER(_pin->id), (&Exti::_irq_handler), (uint32_t)this);
}
/**
 * @brief    Exti构造函数，实例化一个对象
 * @param    mode: gpio模式，type 中断类型，IT，EVENT,IT_EVENT。默认为中断
 * @return   NONE
 */
void Exti::begin(PinMode_t mode, Mode_t extiMode)
{
    // f1系列不能设置为
    _pin->mode((mode == INPUT) ? (INPUT_PU) : (mode));

    //  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SYSCFG);
    GPIO_ConfigEXTILine(GETEXTIPORT(_pin->id), GETPINNUMBER(_pin->id));

    EXTI_DEBUG("extiLine is %d , %d\n", _extiLine, GETEXTILINE(_pin->id));
    EXTI_DEBUG("pinNumber is %d\n", GETPINNUMBER(_pin->id));

    switch (extiMode)
    {
    case ModeIt:
        SET_BIT(EXTI->IMASK, _extiLine);
        CLEAR_BIT(EXTI->EMASK, _extiLine);
        break;
    case ModeEvent:
        SET_BIT(EXTI->EMASK, _extiLine);
        CLEAR_BIT(EXTI->IMASK, _extiLine);
        break;
    case ModeItEvent:
        SET_BIT(EXTI->EMASK, _extiLine);
        SET_BIT(EXTI->IMASK, _extiLine);
        break;
    default:
        SET_BIT(EXTI->IMASK, _extiLine);
        CLEAR_BIT(EXTI->EMASK, _extiLine);
        break;
    }
    nvic(ENABLE);
}

void Exti::nvic(FunctionalState enable, uint8_t preemption_priority, uint8_t sub_priority )
{
//    nvic_dev_set_priority((uint32_t)_extiLine,0,0,0);
//    if(enable != DISABLE)
//        nvic_dev_enable((uint32_t)_extiLine,0);
//    else
//        nvic_dev_disable((uint32_t)_extiLine,0);
    NVIC_InitType NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel          = dev_to_irqn((uint32_t)_extiLine,0);
    NVIC_InitStructure.NVIC_IRQChannelPriority  = preemption_priority;
    NVIC_InitStructure.NVIC_IRQChannelCmd       = enable;
    NVIC_Init(&NVIC_InitStructure);
}


void Exti::interrupt(Trig_t trig, FunctionalState enable)
{

    switch (trig) // 使能触发类型
    {
    case TrigFall:
        bit_write(EXTI->FT_CFG,GETPINNUMBER(_pin->id),enable);
        break;
    case TrigRise:
        bit_write(EXTI->RT_CFG,GETPINNUMBER(_pin->id),enable);
        break;
    case TrigFallRise:
        bit_write(EXTI->FT_CFG,GETPINNUMBER(_pin->id),enable);
        bit_write(EXTI->RT_CFG,GETPINNUMBER(_pin->id),enable);
        break;
    default:
        break;
    }
}

void Exti::soft_triger()
{
    bit_set(EXTI->SWIE,GETPINNUMBER(_pin->id));
}

/**
 *@brief    EXTI 静态成员函数，在中断中调用，解析执行相关回调函数
 *@param    uint32_t id 对象地址，用来识别对象；IrqType irq_type 中断类型
 *@retval   NONE
*/
void Exti::_irq_handler(uint32_t pObj)
{
    Exti *handler = (Exti *)pObj; // 指向回调函数地址
    handler->_pirq.call();

}
/**
 * @brief   绑定中断的回调函数，产生中断后程序将执行callback_fun()
 * @param   void (*fptr)(void)类型函数的指针。
 *
 * @return  NONE
 */
void Exti::attach(void (*fptr)(void))
{
    _pirq.attach(fptr);
}

extern "C" {

    void EXTI0_1_IRQHandler(void)
    {
        if (EXTI_GetITStatus(EXTI_LINE0) != RESET)
        {
            exti_handler(exti_irq_ids[0]);
            EXTI_ClrITPendBit(EXTI_LINE0);
        }
        if (EXTI_GetITStatus(EXTI_LINE1) != RESET)
        {
            exti_handler(exti_irq_ids[1]);
            EXTI_ClrITPendBit(EXTI_LINE1);
        }
    }
    void EXTI2_3_IRQHandler(void)
    {
        if (EXTI_GetITStatus(EXTI_LINE2) != RESET)
        {
            exti_handler(exti_irq_ids[2]);
            EXTI_ClrITPendBit(EXTI_LINE2);
        }
        if (EXTI_GetITStatus(EXTI_LINE3) != RESET)
        {
            exti_handler(exti_irq_ids[3]);
            EXTI_ClrITPendBit(EXTI_LINE3);
        }
    }
   

    void EXTI4_15_IRQHandler(void)
    {
        for (uint8_t i = 4; i <= 15; i++)
        {
            if (EXTI_GetITStatus(1 << i) != RESET)
            {
                exti_handler(exti_irq_ids[i]);
                EXTI_ClrITPendBit(1 << i);
            }
        }
    }


}
