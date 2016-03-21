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
#include "core.h"


/******************空闲任务信息*******************/
#define StackSizeIdle   50		 // 定义空闲任务栈大小                 
static STACK_TypeDef   StackIdle[StackSizeIdle]; 				// 建立空闲任务栈
void idle_task(void);
/*****************************************************************************/

/*
+---------------------------------------------------------------------------------------+
|函数名称:  os_set_priod_ready                                                                |
|---------------------------------------------------------------------------------------|
|函数原型:  void os_set_priod_ready(prio)                                                     |
|函数功能:  任务就绪                                                                    |
|入口参数:  prio 任务优先级                                                             |
+---------------------------------------------------------------------------------------+
*/
#define os_set_priod_ready(Prio)    \
    {                         \
    OSRdyTbl |= 0x01<<Prio;   \
    }
/*
+---------------------------------------------------------------------------------------+
|函数名称：	os_delete_prio_ready																|
|---------------------------------------------------------------------------------------|
|函数原型：	void os_delete_prio_ready(prio)														|
|																						|
|函数功能：	任务删除 															   		|
|																						|
|入口参数：	prio 任务优先级														   		|
+---------------------------------------------------------------------------------------+
*/
#define os_delete_prio_ready(Prio)    \
    {                         \
    OSRdyTbl &= ~(0x01<<Prio);\
	}
/*
+---------------------------------------------------------------------------------------+
|函数名称:  os_task_create                                                             |
|---------------------------------------------------------------------------------------|
|函数原型:  void os_task_create(void (*p_Task)(void),.......)                          |
|函数功能:  建立任务                                                                    |
|入口参数:  *Task：任务函数地址；*Stack：任务栈顶指针；Prio:任务优先级                  |
+---------------------------------------------------------------------------------------+
*/
void os_task_create(void (*p_Task)(void), STACK_TypeDef *p_Stack, PRIO_TypeDef Prio)
{
    if(Prio <= OS_TASKS)
    {
        CPU_task_create(p_Task, p_Stack, Prio); //具体平台任务创建函数
        os_set_priod_ready(Prio);      			  // 在任务就绪表中登记
    }
}

/*
+---------------------------------------------------------------------------------------+
|函数名称:  os_start                                                                  |
|---------------------------------------------------------------------------------------|
|函数原型:  void os_start(void)                                                       |
|函数功能:  系统启动                                                                    |
+---------------------------------------------------------------------------------------+
*/
void os_start(void)
{
    INT8U i;
    os_task_create(idle_task, &StackIdle[StackSizeIdle - 1], OS_TASKS); //创建IDLE任务

    for( i = 0; (i < OS_TASKS) && (!(OSRdyTbl & (0x01 << i))); i++ ); //查找最高优先级算法
    OSPrioCur = OSPrioHighRdy = i; // 运行最高优先级任务
    OS_Tcb_HighRdyP = &TCB[OSPrioHighRdy];
    OS_EN_TIME_SW();
    cpu_start();                 // 具体平台系统启动函数
}
/*
+---------------------------------------------------------------------------------------+
|函数名称:  os_task_suspend                                                            |
|---------------------------------------------------------------------------------------|
|函数原型:  void os_task_suspend(PRIO_TypeDef Prio)                                    |
|函数功能:  任务挂起                                                                    |
|入口参数:  挂起任务的优先级                                                            |
+---------------------------------------------------------------------------------------+
*/
void os_task_suspend(PRIO_TypeDef Prio)
{
    INT16U i;
    OS_ENTER_CRITICAL();
    TCB[Prio].OSTCBDly = 0;
    TCB[Prio].State = TASK_SUSPEND;
    os_delete_prio_ready(Prio);				// 从任务就绪表上去除标志位
    OS_EXIT_CRITICAL();

    if(OSPrioCur == Prio)			// 当要挂起的任务为当前任务	重新调度
    {

        for( i = 0; (i < OS_TASKS) && (!(OSRdyTbl & (0x01 << i))); i++ ); //查找最高优先级算法

        OSPrioHighRdy = i;
        OSPrioCur = i;
        OS_Tcb_HighRdyP = &TCB[i];
        OSCtxSw();

    }
}
/*
+---------------------------------------------------------------------------------------+
|函数名称：	os_task_resume                                                             |
|---------------------------------------------------------------------------------------|
|函数原型:  void os_task_resume(PRIO_TypeDef Prio)                                     |
|函数功能:  任务恢复                                                                    |
|入口参数:  恢复任务的优先级                                                            |
+---------------------------------------------------------------------------------------+
*/
void os_task_resume(PRIO_TypeDef Prio)
{
    INT16U i;
    OS_ENTER_CRITICAL();
    os_set_priod_ready(Prio);				// 从任务就绪表上重置标志位
    TCB[Prio].OSTCBDly = 0;			// 将时间计时设为0,延时到
    TCB[Prio].State = TASK_READY;			// 将时间计时设为0,延时到
    OS_EXIT_CRITICAL();

    if(OSPrioCur == Prio)			// 当要挂起的任务为当前任务	重新调度
    {
        for( i = 0; (i < OS_TASKS) && (!(OSRdyTbl & (0x01 << i))); i++ ); //查找最高优先级算法

        OSPrioHighRdy = i;
        OSPrioCur = i;
        OS_Tcb_HighRdyP = &TCB[i];
        OSCtxSw();

    }

}

/*
+---------------------------------------------------------------------------------------+
|函数名称:  os_time_delay                                                              |
|---------------------------------------------------------------------------------------|
|函数原型: 	void os_time_delay(TICKS_TypeDef ticks)                                    |
|函数功能:  任务延时                                                                    |
|入口参数:  延时的时间，任务等待时钟节拍的个数                                          |
|有关说明:  延时数不得超出TICKS_TypeDef的范围                                           |
+---------------------------------------------------------------------------------------+
*/
void os_time_delay(TICKS_TypeDef ticks)
{
    INT32U i = 0;
    if(ticks)                           //如果需要延时
    {
        OS_ENTER_CRITICAL();
        os_delete_prio_ready(OSPrioCur);					// 把任务从就绪表中删去
        TCB[OSPrioCur].OSTCBDly = ticks;	// 设置任务延时节拍数
        TCB[OSPrioCur].State = TASK_DELAY;	// 设置任务状态为
        OS_EXIT_CRITICAL();

        for( i = 0; (i < OS_TASKS) && (!(OSRdyTbl & (0x01 << i))); i++ ); //查找最高优先级算法
        OSPrioHighRdy = i;
        OSPrioCur = i;
        OS_Tcb_HighRdyP = &TCB[i];					//将优先级最高的任务堆栈指针传递给OS_Tcb_HighRdyP
        OSCtxSw();												// 任务调度

    }
}
/*
+---------------------------------------------------------------------------------------+
|函数名称:  os_time_sw                                                                 |
|---------------------------------------------------------------------------------------|
|函数原型:  void os_time_sw(void)                                                      |
|函数功能:  被systick中断调用                                                           |
|有关说明:  1.用来为需要延时的任务进行计时
|						2.实现定时切换任务                                                |
+---------------------------------------------------------------------------------------+
*/

void os_time_sw(void)
{
    INT32U i;

    if(TIME_SW == 1)
    {
        for(i = 0; i < OS_TASKS; i++)			// 刷新各任务时钟
        {
            if(TCB[i].State == TASK_DELAY )
            {
                TCB[i].OSTCBDly --;
                if(TCB[i].OSTCBDly == 0)		// 当任务时钟到时,必须是由定时器减时的才行
                {
                    TCB[OSPrioCur].State = TASK_READY;	// 设置任务状态为
                    os_set_priod_ready(i);		    //修改任务就绪表 使任务可以重新运行
                }
            }
        }

        for( i = 0; (i < OS_TASKS) && (!(OSRdyTbl & (0x01 << i))); i++ ); //查找最高优先级算法
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
|函数名称:  os_init                                                                   |
|---------------------------------------------------------------------------------------|
|函数原型:  void os_init(void)                                                        |
|函数功能:  初始化系统全局变量                                                          |
+---------------------------------------------------------------------------------------+
*/
void os_init(void)
{
    OSRdyTbl = 0;
    OSPrioCur = OSPrioHighRdy = OS_TASKS;
    set_systick_user_event_per_sec(1000);
    attach_systick_user_event(os_time_tick);//移植到非ebox环境的时候需要处理。将os_time_tick放入systick中断，并将此行删除
    OS_NO_TIME_SW();

}

/*
+---------------------------------------------------------------------------------------+
|函数名称:  IdleTask                                                                    |
|---------------------------------------------------------------------------------------|
|函数原型:  void IdleTask(void)                                                         |
|函数功能:  空闲任务                                                                    |
|有关说明:  空闲任务必须永远处于就绪状态                                                |
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


///////计算CPU使用率//////////////////////////////////////
#define CPU_USAGE_CALC_TICK    1000

static  uint32_t totalCount = 0 ;
static	uint32_t count = 0;
static float cpu_usage = 0;
//cpu使用率的相对值计算方法
//在已使用操作系统的情况下计算没有任何用户任务情况下的计算能力
//然后统计使用操作系统的空闲任务时间
//最后计算cpu使用率
void os_idle_hook(void)
{

    uint32_t tick;

    if(totalCount == 0)//只在开机的第一次进入执行
    {
        OS_NO_TIME_SW();//停止调度器
        tick = os_get_tick();
        while(os_get_tick() - tick < CPU_USAGE_CALC_TICK)
        {
            totalCount++;
        }
        OS_EN_TIME_SW();//开启调度器
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
//cpu使用率的绝对值计算方法
//先计算cpu在不使用操作系统的计算能力
//然后统计使用操作系统的空闲任务时间
//最后计算cpu使用率
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

