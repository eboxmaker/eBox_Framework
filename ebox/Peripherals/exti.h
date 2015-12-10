/*
file   : exti.h
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/
#ifndef __EXTI_H
#define __EXTI_H
#include "common.h"
/*
	1.提供一个io中断
	*注意：stm32一个中断线EXTI_Linex只能连接到一个port的GPIO_Pin_x，即设置PA0为中断源之后就不能设置PB0，PC0等为中断源
*/

//EXTITrigger_TypeDef类型值
//	EXTI_Trigger_Rising 
//	EXTI_Trigger_Falling  
//	EXTI_Trigger_Rising_Falling 
class EXTIx
{
    public:
        EXTIx(GPIO *exti_pin, EXTITrigger_TypeDef trigger);
        void begin();
        void attach_interrupt(void (*callback_fun)(void));
        void interrupt(FunctionalState enable);

    private:
        GPIO                *exti_pin;
        EXTITrigger_TypeDef trigger;
        uint8_t             port_source;
        uint8_t             pin_source;
        uint32_t            exti_line;
        uint8_t             irq;

        void init_info(GPIO *exti_pin);
};

#endif
