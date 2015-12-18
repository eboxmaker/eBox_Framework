#ifndef __XXOS_H
#define __XXOS_H

#ifdef __cplusplus
 extern "C"{
#endif
#include "cpu.h"

void OS_Init(void);
void OS_Start(void);
void OS_TaskSuspend(PRIO_TypeDef Prio);
void OS_TaskResume(PRIO_TypeDef Prio);
void OS_TaskCreate(void (*p_Task)(void),STACK_TypeDef *p_Stack,PRIO_TypeDef Prio);
void OS_DelayTimes(TICKS_TypeDef ticks);
float OS_GetCPU(void);
INT8U OS_GetStackMaxUsage(STACK_TypeDef* stack,INT16U size);

#ifdef __cplusplus
 }
#endif

#endif
