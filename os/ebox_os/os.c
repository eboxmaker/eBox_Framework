/*
file   : os.c
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/


#include "os.h"
#include "mcu_core.h"


/******************����������Ϣ*******************/
#define StackSizeIdle   50		 // �����������ջ��С                 
static STACK_TypeDef   StackIdle[StackSizeIdle]; 				// ������������ջ
void idle_task(void);
/*****************************************************************************/

/*
+---------------------------------------------------------------------------------------+
|��������:  os_set_priod_ready                                                                |
|---------------------------------------------------------------------------------------|
|����ԭ��:  void os_set_priod_ready(prio)                                                     |
|��������:  �������                                                                    |
|��ڲ���:  prio �������ȼ�                                                             |
+---------------------------------------------------------------------------------------+
*/
#define os_set_priod_ready(Prio)    \
    {                         \
    OSRdyTbl |= 0x01<<Prio;   \
    }
/*
+---------------------------------------------------------------------------------------+
|�������ƣ�	os_delete_prio_ready																|
|---------------------------------------------------------------------------------------|
|����ԭ�ͣ�	void os_delete_prio_ready(prio)														|
|																						|
|�������ܣ�	����ɾ�� 															   		|
|																						|
|��ڲ�����	prio �������ȼ�														   		|
+---------------------------------------------------------------------------------------+
*/
#define os_delete_prio_ready(Prio)    \
    {                         \
    OSRdyTbl &= ~(0x01<<Prio);\
	}
/*
+---------------------------------------------------------------------------------------+
|��������:  os_task_create                                                             |
|---------------------------------------------------------------------------------------|
|����ԭ��:  void os_task_create(void (*p_Task)(void),.......)                          |
|��������:  ��������                                                                    |
|��ڲ���:  *Task����������ַ��*Stack������ջ��ָ�룻Prio:�������ȼ�                  |
+---------------------------------------------------------------------------------------+
*/
void os_task_create(void (*p_Task)(void), STACK_TypeDef *p_Stack, PRIO_TypeDef Prio)
{
    if(Prio <= OS_TASKS)
    {
        CPU_task_create(p_Task, p_Stack, Prio); //����ƽ̨���񴴽�����
        os_set_priod_ready(Prio);      			  // ������������еǼ�
    }
}

/*
+---------------------------------------------------------------------------------------+
|��������:  os_start                                                                  |
|---------------------------------------------------------------------------------------|
|����ԭ��:  void os_start(void)                                                       |
|��������:  ϵͳ����                                                                    |
+---------------------------------------------------------------------------------------+
*/
void os_start(void)
{
    INT8U i;
    os_task_create(idle_task, &StackIdle[StackSizeIdle - 1], OS_TASKS); //����IDLE����

    for( i = 0; (i < OS_TASKS) && (!(OSRdyTbl & (0x01 << i))); i++ ); //����������ȼ��㷨
    OSPrioCur = OSPrioHighRdy = i; // ����������ȼ�����
    OS_Tcb_HighRdyP = &TCB[OSPrioHighRdy];
    OS_EN_TIME_SW();
    cpu_start();                 // ����ƽ̨ϵͳ��������
}
/*
+---------------------------------------------------------------------------------------+
|��������:  os_task_suspend                                                            |
|---------------------------------------------------------------------------------------|
|����ԭ��:  void os_task_suspend(PRIO_TypeDef Prio)                                    |
|��������:  �������                                                                    |
|��ڲ���:  ������������ȼ�                                                            |
+---------------------------------------------------------------------------------------+
*/
void os_task_suspend(PRIO_TypeDef Prio)
{
    INT16U i;
    OS_ENTER_CRITICAL();
    TCB[Prio].OSTCBDly = 0;
    TCB[Prio].State = TASK_SUSPEND;
    os_delete_prio_ready(Prio);				// �������������ȥ����־λ
    OS_EXIT_CRITICAL();

    if(OSPrioCur == Prio)			// ��Ҫ���������Ϊ��ǰ����	���µ���
    {

        for( i = 0; (i < OS_TASKS) && (!(OSRdyTbl & (0x01 << i))); i++ ); //����������ȼ��㷨

        OSPrioHighRdy = i;
        OSPrioCur = i;
        OS_Tcb_HighRdyP = &TCB[i];
        OSCtxSw();

    }
}
/*
+---------------------------------------------------------------------------------------+
|�������ƣ�	os_task_resume                                                             |
|---------------------------------------------------------------------------------------|
|����ԭ��:  void os_task_resume(PRIO_TypeDef Prio)                                     |
|��������:  ����ָ�                                                                    |
|��ڲ���:  �ָ���������ȼ�                                                            |
+---------------------------------------------------------------------------------------+
*/
void os_task_resume(PRIO_TypeDef Prio)
{
    INT16U i;
    OS_ENTER_CRITICAL();
    os_set_priod_ready(Prio);				// ����������������ñ�־λ
    TCB[Prio].OSTCBDly = 0;			// ��ʱ���ʱ��Ϊ0,��ʱ��
    TCB[Prio].State = TASK_READY;			// ��ʱ���ʱ��Ϊ0,��ʱ��
    OS_EXIT_CRITICAL();

    if(OSPrioCur == Prio)			// ��Ҫ���������Ϊ��ǰ����	���µ���
    {
        for( i = 0; (i < OS_TASKS) && (!(OSRdyTbl & (0x01 << i))); i++ ); //����������ȼ��㷨

        OSPrioHighRdy = i;
        OSPrioCur = i;
        OS_Tcb_HighRdyP = &TCB[i];
        OSCtxSw();

    }

}

/*
+---------------------------------------------------------------------------------------+
|��������:  os_time_delay                                                              |
|---------------------------------------------------------------------------------------|
|����ԭ��: 	void os_time_delay(TICKS_TypeDef ticks)                                    |
|��������:  ������ʱ                                                                    |
|��ڲ���:  ��ʱ��ʱ�䣬����ȴ�ʱ�ӽ��ĵĸ���                                          |
|�й�˵��:  ��ʱ�����ó���TICKS_TypeDef�ķ�Χ                                           |
+---------------------------------------------------------------------------------------+
*/
void os_time_delay(TICKS_TypeDef ticks)
{
    INT32U i = 0;
    if(ticks)                           //�����Ҫ��ʱ
    {
        OS_ENTER_CRITICAL();
        os_delete_prio_ready(OSPrioCur);					// ������Ӿ�������ɾȥ
        TCB[OSPrioCur].OSTCBDly = ticks;	// ����������ʱ������
        TCB[OSPrioCur].State = TASK_DELAY;	// ��������״̬Ϊ
        OS_EXIT_CRITICAL();

        for( i = 0; (i < OS_TASKS) && (!(OSRdyTbl & (0x01 << i))); i++ ); //����������ȼ��㷨
        OSPrioHighRdy = i;
        OSPrioCur = i;
        OS_Tcb_HighRdyP = &TCB[i];					//�����ȼ���ߵ������ջָ�봫�ݸ�OS_Tcb_HighRdyP
        OSCtxSw();												// �������

    }
}
/*
+---------------------------------------------------------------------------------------+
|��������:  os_time_sw                                                                 |
|---------------------------------------------------------------------------------------|
|����ԭ��:  void os_time_sw(void)                                                      |
|��������:  ��systick�жϵ���                                                           |
|�й�˵��:  1.����Ϊ��Ҫ��ʱ��������м�ʱ
|						2.ʵ�ֶ�ʱ�л�����                                                |
+---------------------------------------------------------------------------------------+
*/

void os_time_sw(void)
{
    INT32U i;

    if(TIME_SW == 1)
    {
        for(i = 0; i < OS_TASKS; i++)			// ˢ�¸�����ʱ��
        {
            if(TCB[i].State == TASK_DELAY )
            {
                TCB[i].OSTCBDly --;
                if(TCB[i].OSTCBDly == 0)		// ������ʱ�ӵ�ʱ,�������ɶ�ʱ����ʱ�Ĳ���
                {
                    TCB[OSPrioCur].State = TASK_READY;	// ��������״̬Ϊ
                    os_set_priod_ready(i);		    //�޸���������� ʹ���������������
                }
            }
        }

        for( i = 0; (i < OS_TASKS) && (!(OSRdyTbl & (0x01 << i))); i++ ); //����������ȼ��㷨
        OSPrioHighRdy = i;
        OSPrioCur = i;
        OS_Tcb_HighRdyP = &TCB[i];
        OSCtxSw();
    }
}

uint32_t os_get_tick()
{
    return OSTick;
}
void os_time_tick()
{
    OS_ENTER_CRITICAL();
    OSTick++;
    os_time_sw();
    OS_EXIT_CRITICAL();

}
/*
+---------------------------------------------------------------------------------------+
|��������:  os_init                                                                   |
|---------------------------------------------------------------------------------------|
|����ԭ��:  void os_init(void)                                                        |
|��������:  ��ʼ��ϵͳȫ�ֱ���                                                          |
+---------------------------------------------------------------------------------------+
*/
void os_init(void)
{
    OSRdyTbl = 0;
    OSPrioCur = OSPrioHighRdy = OS_TASKS;
    set_systick_user_event_per_sec(1000);
    attach_systick_user_event(os_time_tick);//��ֲ����ebox������ʱ����Ҫ������os_time_tick����systick�жϣ���������ɾ��
    OS_NO_TIME_SW();

}

/*
+---------------------------------------------------------------------------------------+
|��������:  IdleTask                                                                    |
|---------------------------------------------------------------------------------------|
|����ԭ��:  void IdleTask(void)                                                         |
|��������:  ��������                                                                    |
|�й�˵��:  �������������Զ���ھ���״̬                                                |
+---------------------------------------------------------------------------------------+
*/
void os_idle_hook(void);
void os_idle_hook_1(void);

void idle_task(void)
{
    while(1)
    {
        os_idle_hook();
    }
}


///////����CPUʹ����//////////////////////////////////////
#define CPU_USAGE_CALC_TICK    1000

static  uint32_t totalCount = 0 ;
static	uint32_t count = 0;
static float cpu_usage = 0;
//cpuʹ���ʵ����ֵ���㷽��
//����ʹ�ò���ϵͳ������¼���û���κ��û���������µļ�������
//Ȼ��ͳ��ʹ�ò���ϵͳ�Ŀ�������ʱ��
//������cpuʹ����
void os_idle_hook(void)
{

    uint32_t tick;

    if(totalCount == 0)//ֻ�ڿ����ĵ�һ�ν���ִ��
    {
        OS_NO_TIME_SW();//ֹͣ������
        tick = os_get_tick();
        while(os_get_tick() - tick < CPU_USAGE_CALC_TICK)
        {
            totalCount++;
        }
        OS_EN_TIME_SW();//����������
    }

    count = 0;

    tick = os_get_tick();
    while(os_get_tick() - tick < CPU_USAGE_CALC_TICK)
    {
        count++;

    }
    if(count < totalCount)
    {
        count = totalCount - count;
        cpu_usage = (float)(count * 1.0 / (float)totalCount) * 100;
    }

}
//cpuʹ���ʵľ���ֵ���㷽��
//�ȼ���cpu�ڲ�ʹ�ò���ϵͳ�ļ�������
//Ȼ��ͳ��ʹ�ò���ϵͳ�Ŀ�������ʱ��
//������cpuʹ����
void os_idle_hook_1(void)
{

    uint32_t tick;



    count = 0;

    tick = os_get_tick();
    while(os_get_tick() - tick < 1000)
    {
        count++;

    }
    if(count < get_cpu_calculate_per_sec())
    {
        count = get_cpu_calculate_per_sec() - count;
        cpu_usage = (float)(count * 100.0 / (float)get_cpu_calculate_per_sec());
    }

}


float os_get_cpu_usage(void)
{
    return cpu_usage;
}


uint8_t os_get_stack_max_usage(STACK_TypeDef *stack, INT16U size)
{
    INT16U i;
    INT8U percent;
    for(i = 0; i < size; i++)
    {
        if(stack[i] != 0)
            break;
    }

    i = size - i;

    percent = i * 100 / size;
    return percent;
}

