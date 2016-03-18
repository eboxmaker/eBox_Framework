/*
file   : os.h
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/
#ifndef __XXOS_H
#define __XXOS_H

#ifdef __cplusplus
extern "C" {
#endif
#include "cpu.h"
void        os_init(void);
void        os_start(void);
void        os_task_suspend(PRIO_TypeDef Prio);
void        os_task_resume(PRIO_TypeDef Prio);
void        os_task_create(void (*p_Task)(void), STACK_TypeDef *p_Stack, PRIO_TypeDef Prio);
void        os_time_delay(TICKS_TypeDef ticks);

float       os_get_cpu_usage(void);
uint32_t    os_get_tick(void);
uint8_t     os_get_stack_max_usage(STACK_TypeDef *stack, INT16U size);

#ifdef __cplusplus
}
#endif

#endif
