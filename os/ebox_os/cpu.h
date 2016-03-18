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
    TASK_READY = 0,//����
    TASK_DELAY = 1,//��ʱ�ȴ�
    TASK_SUSPEND = 2,//����
} OS_TASK_STA;

typedef struct
{
    STACK_TypeDef *StkAddr;				 //����ջ��
    TICKS_TypeDef OSTCBDly;				 //������ʱʱ��
    OS_TASK_STA State;				     //����״̬

} OS_TCB, *OS_TCBP;

/******************ȫ�ֱ�������***************/

#define OS_TICKS_PER_SEC  1000      // ����һ���ڵ�ʱ�ӽ�����,����ϵͳʱ�ӽ�����ebox�ṩ����ֵ��Ч��Ĭ��1000
#define OS_TASKS		  3         // �趨�������������,�������������� 


extern INT32U OSTick;
extern INT32U cpu_sr;//���� PRIMASK;

extern  INT32U          OSRdyTbl;           // ���������б�
extern  PRIO_TypeDef    OSPrioCur;          // ��ǰ��������ȼ�
extern  PRIO_TypeDef    OSPrioHighRdy;      // ����Ҫ������������ȼ�
extern  OS_TCB          TCB[OS_TASKS + 1];  // ����������ƿ�TCB����

////////�����л���Ҫ�õ�����TCBָ��///////////////////
extern OS_TCBP OS_Tcb_CurP;
extern OS_TCBP OS_Tcb_HighRdyP;

///////////�ж���ؿ���///////////////////////////////////////////////////////////////////
INT32U OS_CPU_SR_Save(void);
void OS_CPU_SR_Restore(INT32U sr);

#define  OS_USE_CRITICAL    INT32U cpu_sr;
#define  OS_ENTER_CRITICAL()  {cpu_sr = OS_CPU_SR_Save();}//�˼���ֹͣһ���ж�
#define  OS_EXIT_CRITICAL()   {OS_CPU_SR_Restore(cpu_sr);}
////////////////////////////////////////////////////////////////////////////////////////
#define  OS_NO_TIME_SW()  {TIME_SW = 0;}//�˼��������ֹͣ��ʱ���ȣ�����ֹͣ��ʱ��
#define  OS_EN_TIME_SW()  {TIME_SW = 1;}
extern INT8U TIME_SW;
///////////////////////////////////////////////////////////////////////////////////////
extern OS_TCB TCB[OS_TASKS + 1];

void OSCtxSw(void);
void OSStart(void);

void CPU_task_create(TASK_TypeDef task, STACK_TypeDef *stk, PRIO_TypeDef t_Prio);
void cpu_start(void);


#endif
