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
#ifndef __MCU_H
#define __MCU_H




#ifdef __cplusplus
extern "C" {
#endif
#include "mcu_config.h"
#include "rcc.h"
#include "nvic.h"

typedef void (*callback_fun_type)(void);

    

#define NVIC_GROUP_CONFIG NVIC_PriorityGroup_2//以后NVIC_PriorityGroupConfig()函数不需要再被调用。更不能再以非NVIC_GROUP_CONFIG值填充调用



    void mcu_init(void);
    uint64_t mcu_micros(void);
    uint64_t mcu_millis( void );
    void mcu_delay_ms(uint32_t ms);
    void mcu_delay_us(uint32_t us);
    void set_systick_user_event_per_sec(u16 frq);
    void attach_systick_user_event(void (*callback_fun)(void));
#ifdef __cplusplus
}
#endif

#endif
