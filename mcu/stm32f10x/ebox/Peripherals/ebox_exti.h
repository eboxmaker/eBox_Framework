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
#include "ebox_common.h"
#include "FunctionPointer.h"

/*
	1.提供一个io中断
	*注意：stm32一个中断线EXTI_Linex只能连接到一个port的GPIO_Pin_x，即设置PA0为中断源之后就不能设置PB0，PC0等为中断源
*/

//EXTITrigger_TypeDef类型值
//	EXTI_Trigger_Rising
//	EXTI_Trigger_Falling
//	EXTI_Trigger_Rising_Falling
class Exti
{
public:
    Exti(Gpio *exti_pin, EXTITrigger_TypeDef trigger);
    void begin();
    void attach_interrupt(void (*callback_fun)(void));
    void interrupt(FunctionalState enable, uint8_t preemption_priority = 0, uint8_t sub_priority = 0);

    static void _irq_handler( uint32_t id);
    void attach(void (*fptr)(void));
    template<typename T>
    void attach(T* tptr, void (T::*mptr)(void)) {
        _irq.attach(tptr, mptr);
    }

private:
    Gpio                *exti_pin;
    EXTITrigger_TypeDef trigger;
    uint8_t             port_source;
    uint8_t             pin_source;
    uint32_t            exti_line;
    uint8_t             irq;

    void init_info(Gpio *exti_pin);
protected:
    FunctionPointer _irq;
};
#ifdef __cplusplus
extern "C" {
#endif


typedef void (*exti_irq_handler)(uint32_t id);

int exti_irq_init(uint8_t index,exti_irq_handler handler,uint32_t id);
	
#ifdef __cplusplus
}
#endif

#endif
