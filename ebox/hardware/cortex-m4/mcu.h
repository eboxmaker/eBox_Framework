/*
file   : common.h
author : shentq
version: V1.1
date   : 2016/4/6

Copyright 2015 shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/
#ifndef __MCU_CORE_H
#define __MCU_CORE_H
#ifdef __cplusplus
extern "C" {
#endif
#include "stm32f4xx.h"
#include "ebox_type.h"

    
typedef void (*callback_fun_type)(void);

#define interrupts() 		__enable_irq()//允许所有中断
#define no_interrupts() 	__disable_irq()//禁止所有中断
    



extern cpu_t mcu;


    void mcu_init(void);
    uint64_t mcu_micros(void);
    uint64_t mcu_millis( void );
    void mcu_delay_ms(uint64_t ms);
    void mcu_delay_us(uint64_t us);
    void set_systick_user_event_per_sec(u16 frq);
    void attach_systick_user_event(void (*callback_fun)(void));
#ifdef __cplusplus
}
#endif

#endif
