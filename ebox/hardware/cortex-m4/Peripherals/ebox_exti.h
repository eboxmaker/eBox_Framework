/**
  ******************************************************************************
  * @file    exti.h
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

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __EXTI_H
#define __EXTI_H
#include "ebox_core.h"
#include "mcu.h"
#include "FunctionPointer.h"

/*
	1.提供一个io中断
	*注意：stm32一个中断线EXTI_Linex只能连接到一个port的GPIO_Pin_x，即设置PA0为中断源之后就不能设置PB0，PC0等为中断源
    trigger: 引脚触发中的条件，可以是一下三种模式中的一种：
        - RISING: 上升沿触发中断
        - FALLING: 下降沿触发中断
        - CHANGE: 上升沿和下降沿均触发中断
 */


class Exti
{
public:
    // 触发类型
    enum Trigger_t
    {
        FALL = 0,	// 下降沿触发
        RISE,		// 上升沿触发
        CHANGE		// 上升沿下降沿
    };
    enum Exti_t
    {
        IT = 0,			// 中断
        EVENT,			// 事件
        IT_EVENT		// 中断&事件
    };

public:
    Exti(Gpio *exti_pin);
    void begin(PinMode_t mode = INPUT, Exti_t type = IT);
    void nvic(FunctionalState enable, uint8_t preemption_priority = 0, uint8_t sub_priority = 0);
    void interrupt(Trigger_t type, FunctionalState enable);

    static void _irq_handler( uint32_t id);
    void attach(void (*fptr)(void));
    template<typename T>
    void attach(T *tptr, void (T::*mptr)(void))
    {
        _irq.attach(tptr, mptr);
    }

private:
    Gpio                *pin;
    uint8_t             port_source;
    uint8_t             pin_source;
    uint32_t            exti_line;

protected:
    FunctionPointer _irq;
};
#ifdef __cplusplus
extern "C" {
#endif


typedef void (*exti_irq_handler)(uint32_t id);

int exti_irq_init(uint8_t index, exti_irq_handler handler, uint32_t id);

#ifdef __cplusplus
}
#endif

#endif
