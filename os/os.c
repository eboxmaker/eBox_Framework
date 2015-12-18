
#include "os.h"
extern float cpu;

/******************空闲任务信息*******************/
#define StackSizeIdle   50		 // 定义空闲任务栈大小                 
static STACK_TypeDef   StackIdle[StackSizeIdle]; 				// 建立空闲任务栈
void Idle_Task(void);
/*****************************************************************************/

/*
+---------------------------------------------------------------------------------------+
|函数名称:  OSSetPrioRdy                                                                |
|---------------------------------------------------------------------------------------|
|函数原型:  void OSSetPrioRdy(prio)                                                     |
|函数功能:  任务就绪                                                                    |
|入口参数:  prio 任务优先级                                                             |
+---------------------------------------------------------------------------------------+
*/
#define OSSetPrioRdy(Prio)    \
    {                         \
    OSRdyTbl |= 0x01<<Prio;   \
    }		                                          
/*
+---------------------------------------------------------------------------------------+
|函数名称：	OSDelPrioRdy																|
|---------------------------------------------------------------------------------------|
|函数原型：	void OSDelPrioRdy(prio)														|
|																						|
|函数功能：	任务删除 															   		|
|																						|
|入口参数：	prio 任务优先级														   		|
+---------------------------------------------------------------------------------------+
*/							          
#define OSDelPrioRdy(Prio)    \
    {                         \
    OSRdyTbl &= ~(0x01<<Prio);\
	}												   
/*
+---------------------------------------------------------------------------------------+
|函数名称:  OS_TaskCreate                                                             |
|---------------------------------------------------------------------------------------|
|函数原型:  void OS_TaskCreate(void (*p_Task)(void),.......)                          |
|函数功能:  建立任务                                                                    |
|入口参数:  *Task：任务函数地址；*Stack：任务栈顶指针；Prio:任务优先级                  |
+---------------------------------------------------------------------------------------+
*/
void OS_TaskCreate(void (*p_Task)(void),STACK_TypeDef *p_Stack, PRIO_TypeDef Prio)
{
    if(Prio <= OS_TASKS)
    {
			CPU_TaskCreate(p_Task,p_Stack,Prio);  //具体平台任务创建函数  	
			OSSetPrioRdy(Prio);      			  // 在任务就绪表中登记	
    }
}

/*
+---------------------------------------------------------------------------------------+
|函数名称:  OS_Start                                                                  |
|---------------------------------------------------------------------------------------|
|函数原型:  void OS_Start(void)                                                       |
|函数功能:  系统启动                                                                    |
+---------------------------------------------------------------------------------------+
*/
void OS_Start(void)
{
		INT32U i;
	  OS_TaskCreate(Idle_Task,&StackIdle[StackSizeIdle-1],OS_TASKS);//创建IDLE任务
	
//		for( i = 0;(i < OS_TASKS) && (!(OSRdyTbl & (0x01<<i)));i++ );//查找最高优先级算法	
//    OSPrioCur = OSPrioHighRdy=i;   // 运行最高优先级任务
    OSPrioCur = OSPrioHighRdy=OS_TASKS;   // 运行一次空闲任务，用于初始化CPU使用率统计信息
		OS_Tcb_HighRdyP=&TCB[OSPrioHighRdy];
		OS_EN_TIMELY_SW();
    CPU_OSStart();                 // 具体平台系统启动函数								
}
/*
+---------------------------------------------------------------------------------------+
|函数名称:  OS_TaskSuspend                                                            |
|---------------------------------------------------------------------------------------|
|函数原型:  void OS_TaskSuspend(PRIO_TypeDef Prio)                                    |
|函数功能:  任务挂起                                                                    |
|入口参数:  挂起任务的优先级                                                            |
+---------------------------------------------------------------------------------------+
*/
void OS_TaskSuspend(PRIO_TypeDef Prio)
{
	INT16U i;
	OS_ENTER_CRITICAL();
	TCB[Prio].OSTCBDly = 0;
	TCB[Prio].State = TASK_SUSPEND;
	OSDelPrioRdy(Prio);				// 从任务就绪表上去除标志位
	OS_EXIT_CRITICAL();
		
	if(OSPrioCur == Prio)			// 当要挂起的任务为当前任务	重新调度		
	{

	  for( i = 0;(i < OS_TASKS) && (!(OSRdyTbl & (0x01<<i)));i++ );//查找最高优先级算法
		
		OSPrioHighRdy = i;	
		OSPrioCur = i;
		OS_Tcb_HighRdyP=&TCB[i];
		OSCtxSw();

	}	
}
/*
+---------------------------------------------------------------------------------------+
|函数名称：	OS_TaskResume                                                             |
|---------------------------------------------------------------------------------------|
|函数原型:  void OS_TaskResume(PRIO_TypeDef Prio)                                     |
|函数功能:  任务恢复                                                                    |
|入口参数:  恢复任务的优先级                                                            |
+---------------------------------------------------------------------------------------+
*/
void OS_TaskResume(PRIO_TypeDef Prio)
{
	INT16U i;
	OS_ENTER_CRITICAL();
	OSSetPrioRdy(Prio);				// 从任务就绪表上重置标志位	
    TCB[Prio].OSTCBDly = 0;			// 将时间计时设为0,延时到	
    TCB[Prio].State = TASK_READY;			// 将时间计时设为0,延时到	
	OS_EXIT_CRITICAL();
	
	if(OSPrioCur == Prio)			// 当要挂起的任务为当前任务	重新调度		
	{
	  for( i = 0;(i < OS_TASKS) && (!(OSRdyTbl & (0x01<<i)));i++ );//查找最高优先级算法
		
		OSPrioHighRdy = i;	
		OSPrioCur = i;
		OS_Tcb_HighRdyP=&TCB[i];
		OSCtxSw();

	}	
	
}

/*
+---------------------------------------------------------------------------------------+
|函数名称:  OS_TimeDelay                                                              |
|---------------------------------------------------------------------------------------|
|函数原型: 	void OS_TimeDelay(TICKS_TypeDef ticks)                                    |
|函数功能:  任务延时                                                                    |
|入口参数:  延时的时间，任务等待时钟节拍的个数                                          |
|有关说明:  延时数不得超出TICKS_TypeDef的范围                                           |
+---------------------------------------------------------------------------------------+
*/
void OS_DelayTimes(TICKS_TypeDef ticks)
{
	INT32U i=0;
	if(ticks)                           //如果需要延时         
	{
		OS_ENTER_CRITICAL();
		OSDelPrioRdy(OSPrioCur);					// 把任务从就绪表中删去 
		TCB[OSPrioCur].OSTCBDly = ticks;	// 设置任务延时节拍数   
		TCB[OSPrioCur].State = TASK_DELAY;	// 设置任务状态为
		OS_EXIT_CRITICAL();
		
		for( i = 0;(i < OS_TASKS) && (!(OSRdyTbl & (0x01<<i)));i++ );//查找最高优先级算法
		OSPrioHighRdy = i;	
		OSPrioCur = i;
		OS_Tcb_HighRdyP=&TCB[i];					//将优先级最高的任务堆栈指针传递给OS_Tcb_HighRdyP
		OSCtxSw();												// 任务调度
		
	}
}
/*
+---------------------------------------------------------------------------------------+
|函数名称:  OSTimelySw                                                                 |
|---------------------------------------------------------------------------------------|
|函数原型:  void OSTimelySw(void)                                                      |
|函数功能:  被systick中断调用                                                           |
|有关说明:  1.用来为需要延时的任务进行计时
|						2.实现定时切换任务                                                |
+---------------------------------------------------------------------------------------+
*/

void OSTimelySw(void)
{
	INT32U i;

	if(TIMELY_SW == 1)
	{
		for(i = 0; i < OS_TASKS; i++)			// 刷新各任务时钟 
		{
			if(TCB[i].State == TASK_DELAY )
			{
				TCB[i].OSTCBDly --;
				if(TCB[i].OSTCBDly == 0)		// 当任务时钟到时,必须是由定时器减时的才行
				{
					TCB[OSPrioCur].State = TASK_READY;	// 设置任务状态为
					OSSetPrioRdy(i);		    //修改任务就绪表 使任务可以重新运行		
				}
			}
		}
		
		for( i = 0;(i < OS_TASKS) && (!(OSRdyTbl & (0x01<<i)));i++ );//查找最高优先级算法
		OSPrioHighRdy = i;	
		OSPrioCur = i;
		OS_Tcb_HighRdyP=&TCB[i];
		OSCtxSw();
	}
}

INT32U OS_GetTick()
{
	return OSTick;
}
/*
+---------------------------------------------------------------------------------------+
|函数名称:  RAYS_Init                                                                   |
|---------------------------------------------------------------------------------------|
|函数原型:  void RAYS_Init(void)                                                        |
|函数功能:  初始化系统全局变量                                                          |
+---------------------------------------------------------------------------------------+
*/
void OS_Init(void)
{
	OSRdyTbl = 0;
	OSPrioCur = OSPrioHighRdy = OS_TASKS;
	SysTickInit();
	OS_NO_TIMELY_SW();

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
void OS_Idlehook(void);

void Idle_Task(void)
{	
	while(1)
	{
	 OS_Idlehook();
	}
}


///////计算CPU使用率//////////////////////////////////////
#define CPU_USAGE_CALC_TICK    1000

static	uint32_t totalCount= 0 ;
static	uint32_t count = 0;	
static float cpuUsage = 0;

void OS_Idlehook(void)
{
 
	uint32_t tick;

	if(totalCount == 0)//只在开机的第一次进入执行
	{
		OS_NO_TIMELY_SW();//停止调度器
		tick = OS_GetTick();
		while(OS_GetTick() - tick < CPU_USAGE_CALC_TICK)
		{
			totalCount++;
		}
		OS_EN_TIMELY_SW();//开启调度器
	}
	
	count = 0;
	
	tick = OS_GetTick();
	while(OS_GetTick() - tick < CPU_USAGE_CALC_TICK)
	{
		count++;
		
	}
	if(count < totalCount)
	{
		count = totalCount - count;
		cpuUsage = (float)(count*100.0/(float)totalCount);
	}

}

float OS_GetCPU(void)
{
	return cpuUsage;
}


INT8U OS_GetStackMaxUsage(STACK_TypeDef* stack,INT16U size)
{
	INT16U i;
	INT8U percent;
	for(i = 0;i < size; i++)
	{
		if(stack[i] != 0)
			break;
	}
	
	i = size - i;
	
	percent = i*100/size;
	return percent;
}

