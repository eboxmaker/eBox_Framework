/*
file   : cpu.h
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/
#ifndef __CPU_H
#define __CPU_H

#include "stm32f10x.h"

typedef unsigned char  BOOLEAN;
typedef unsigned char  INT8U;
typedef signed   char  INT8S;
typedef unsigned int   INT16U;
typedef signed   int   INT16S;
typedef unsigned long  INT32U;
typedef signed   long  INT32S;

typedef unsigned long  STACK_TypeDef;
typedef unsigned long  PRIO_TypeDef;
typedef unsigned int   TICKS_TypeDef;
typedef void (*TASK_TypeDef)(void);

typedef enum OS_TASK_STA
{
    TASK_READY = 0,//就绪
    TASK_DELAY = 1,//延时等待
    TASK_SUSPEND = 2,//挂起
} OS_TASK_STA;

typedef struct
{
    STACK_TypeDef *StkAddr;				 //任务栈顶
    TICKS_TypeDef OSTCBDly;				 //任务延时时钟
    OS_TASK_STA State;				     //任务状态

} OS_TCB, *OS_TCBP;

/******************全局变量声明***************/

#define OS_TICKS_PER_SEC  1000      // 设置一秒内的时钟节拍数,由于系统时钟节拍由ebox提供，此值无效，默认1000
#define OS_TASKS		  3         // 设定运行任务的数量,不包含空闲任务 


extern INT32U OSTick;
extern INT32U cpu_sr;//保存 PRIMASK;

extern  INT32U          OSRdyTbl;           // 就绪任务列表
extern  PRIO_TypeDef    OSPrioCur;          // 当前任务的优先级
extern  PRIO_TypeDef    OSPrioHighRdy;      // 即将要运行任务的优先级
extern  OS_TCB          TCB[OS_TASKS + 1];  // 定义任务控制块TCB数组

////////任务切换需要用的两个TCB指针///////////////////
extern OS_TCBP OS_Tcb_CurP;
extern OS_TCBP OS_Tcb_HighRdyP;

///////////中断相关控制///////////////////////////////////////////////////////////////////
INT32U OS_CPU_SR_Save(void);
void OS_CPU_SR_Restore(INT32U sr);

#define  OS_USE_CRITICAL    INT32U cpu_sr;
#define  OS_ENTER_CRITICAL()  {cpu_sr = OS_CPU_SR_Save();}//此级别停止一切中断
#define  OS_EXIT_CRITICAL()   {OS_CPU_SR_Restore(cpu_sr);}
////////////////////////////////////////////////////////////////////////////////////////
#define  OS_NO_TIME_SW()  {TIME_SW = 0;}//此级别仅限于停止定时调度，但不停止定时器
#define  OS_EN_TIME_SW()  {TIME_SW = 1;}
extern INT8U TIME_SW;
///////////////////////////////////////////////////////////////////////////////////////
extern OS_TCB TCB[OS_TASKS + 1];

void OSCtxSw(void);
void OSStart(void);

void CPU_task_create(TASK_TypeDef task, STACK_TypeDef *stk, PRIO_TypeDef t_Prio);
void cpu_start(void);


#endif
