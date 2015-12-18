
#include "os.h"
extern float cpu;

/******************����������Ϣ*******************/
#define StackSizeIdle   50		 // �����������ջ��С                 
static STACK_TypeDef   StackIdle[StackSizeIdle]; 				// ������������ջ
void Idle_Task(void);
/*****************************************************************************/

/*
+---------------------------------------------------------------------------------------+
|��������:  OSSetPrioRdy                                                                |
|---------------------------------------------------------------------------------------|
|����ԭ��:  void OSSetPrioRdy(prio)                                                     |
|��������:  �������                                                                    |
|��ڲ���:  prio �������ȼ�                                                             |
+---------------------------------------------------------------------------------------+
*/
#define OSSetPrioRdy(Prio)    \
    {                         \
    OSRdyTbl |= 0x01<<Prio;   \
    }		                                          
/*
+---------------------------------------------------------------------------------------+
|�������ƣ�	OSDelPrioRdy																|
|---------------------------------------------------------------------------------------|
|����ԭ�ͣ�	void OSDelPrioRdy(prio)														|
|																						|
|�������ܣ�	����ɾ�� 															   		|
|																						|
|��ڲ�����	prio �������ȼ�														   		|
+---------------------------------------------------------------------------------------+
*/							          
#define OSDelPrioRdy(Prio)    \
    {                         \
    OSRdyTbl &= ~(0x01<<Prio);\
	}												   
/*
+---------------------------------------------------------------------------------------+
|��������:  OS_TaskCreate                                                             |
|---------------------------------------------------------------------------------------|
|����ԭ��:  void OS_TaskCreate(void (*p_Task)(void),.......)                          |
|��������:  ��������                                                                    |
|��ڲ���:  *Task����������ַ��*Stack������ջ��ָ�룻Prio:�������ȼ�                  |
+---------------------------------------------------------------------------------------+
*/
void OS_TaskCreate(void (*p_Task)(void),STACK_TypeDef *p_Stack, PRIO_TypeDef Prio)
{
    if(Prio <= OS_TASKS)
    {
			CPU_TaskCreate(p_Task,p_Stack,Prio);  //����ƽ̨���񴴽�����  	
			OSSetPrioRdy(Prio);      			  // ������������еǼ�	
    }
}

/*
+---------------------------------------------------------------------------------------+
|��������:  OS_Start                                                                  |
|---------------------------------------------------------------------------------------|
|����ԭ��:  void OS_Start(void)                                                       |
|��������:  ϵͳ����                                                                    |
+---------------------------------------------------------------------------------------+
*/
void OS_Start(void)
{
		INT32U i;
	  OS_TaskCreate(Idle_Task,&StackIdle[StackSizeIdle-1],OS_TASKS);//����IDLE����
	
//		for( i = 0;(i < OS_TASKS) && (!(OSRdyTbl & (0x01<<i)));i++ );//����������ȼ��㷨	
//    OSPrioCur = OSPrioHighRdy=i;   // ����������ȼ�����
    OSPrioCur = OSPrioHighRdy=OS_TASKS;   // ����һ�ο����������ڳ�ʼ��CPUʹ����ͳ����Ϣ
		OS_Tcb_HighRdyP=&TCB[OSPrioHighRdy];
		OS_EN_TIMELY_SW();
    CPU_OSStart();                 // ����ƽ̨ϵͳ��������								
}
/*
+---------------------------------------------------------------------------------------+
|��������:  OS_TaskSuspend                                                            |
|---------------------------------------------------------------------------------------|
|����ԭ��:  void OS_TaskSuspend(PRIO_TypeDef Prio)                                    |
|��������:  �������                                                                    |
|��ڲ���:  ������������ȼ�                                                            |
+---------------------------------------------------------------------------------------+
*/
void OS_TaskSuspend(PRIO_TypeDef Prio)
{
	INT16U i;
	OS_ENTER_CRITICAL();
	TCB[Prio].OSTCBDly = 0;
	TCB[Prio].State = TASK_SUSPEND;
	OSDelPrioRdy(Prio);				// �������������ȥ����־λ
	OS_EXIT_CRITICAL();
		
	if(OSPrioCur == Prio)			// ��Ҫ���������Ϊ��ǰ����	���µ���		
	{

	  for( i = 0;(i < OS_TASKS) && (!(OSRdyTbl & (0x01<<i)));i++ );//����������ȼ��㷨
		
		OSPrioHighRdy = i;	
		OSPrioCur = i;
		OS_Tcb_HighRdyP=&TCB[i];
		OSCtxSw();

	}	
}
/*
+---------------------------------------------------------------------------------------+
|�������ƣ�	OS_TaskResume                                                             |
|---------------------------------------------------------------------------------------|
|����ԭ��:  void OS_TaskResume(PRIO_TypeDef Prio)                                     |
|��������:  ����ָ�                                                                    |
|��ڲ���:  �ָ���������ȼ�                                                            |
+---------------------------------------------------------------------------------------+
*/
void OS_TaskResume(PRIO_TypeDef Prio)
{
	INT16U i;
	OS_ENTER_CRITICAL();
	OSSetPrioRdy(Prio);				// ����������������ñ�־λ	
    TCB[Prio].OSTCBDly = 0;			// ��ʱ���ʱ��Ϊ0,��ʱ��	
    TCB[Prio].State = TASK_READY;			// ��ʱ���ʱ��Ϊ0,��ʱ��	
	OS_EXIT_CRITICAL();
	
	if(OSPrioCur == Prio)			// ��Ҫ���������Ϊ��ǰ����	���µ���		
	{
	  for( i = 0;(i < OS_TASKS) && (!(OSRdyTbl & (0x01<<i)));i++ );//����������ȼ��㷨
		
		OSPrioHighRdy = i;	
		OSPrioCur = i;
		OS_Tcb_HighRdyP=&TCB[i];
		OSCtxSw();

	}	
	
}

/*
+---------------------------------------------------------------------------------------+
|��������:  OS_TimeDelay                                                              |
|---------------------------------------------------------------------------------------|
|����ԭ��: 	void OS_TimeDelay(TICKS_TypeDef ticks)                                    |
|��������:  ������ʱ                                                                    |
|��ڲ���:  ��ʱ��ʱ�䣬����ȴ�ʱ�ӽ��ĵĸ���                                          |
|�й�˵��:  ��ʱ�����ó���TICKS_TypeDef�ķ�Χ                                           |
+---------------------------------------------------------------------------------------+
*/
void OS_DelayTimes(TICKS_TypeDef ticks)
{
	INT32U i=0;
	if(ticks)                           //�����Ҫ��ʱ         
	{
		OS_ENTER_CRITICAL();
		OSDelPrioRdy(OSPrioCur);					// ������Ӿ�������ɾȥ 
		TCB[OSPrioCur].OSTCBDly = ticks;	// ����������ʱ������   
		TCB[OSPrioCur].State = TASK_DELAY;	// ��������״̬Ϊ
		OS_EXIT_CRITICAL();
		
		for( i = 0;(i < OS_TASKS) && (!(OSRdyTbl & (0x01<<i)));i++ );//����������ȼ��㷨
		OSPrioHighRdy = i;	
		OSPrioCur = i;
		OS_Tcb_HighRdyP=&TCB[i];					//�����ȼ���ߵ������ջָ�봫�ݸ�OS_Tcb_HighRdyP
		OSCtxSw();												// �������
		
	}
}
/*
+---------------------------------------------------------------------------------------+
|��������:  OSTimelySw                                                                 |
|---------------------------------------------------------------------------------------|
|����ԭ��:  void OSTimelySw(void)                                                      |
|��������:  ��systick�жϵ���                                                           |
|�й�˵��:  1.����Ϊ��Ҫ��ʱ��������м�ʱ
|						2.ʵ�ֶ�ʱ�л�����                                                |
+---------------------------------------------------------------------------------------+
*/

void OSTimelySw(void)
{
	INT32U i;

	if(TIMELY_SW == 1)
	{
		for(i = 0; i < OS_TASKS; i++)			// ˢ�¸�����ʱ�� 
		{
			if(TCB[i].State == TASK_DELAY )
			{
				TCB[i].OSTCBDly --;
				if(TCB[i].OSTCBDly == 0)		// ������ʱ�ӵ�ʱ,�������ɶ�ʱ����ʱ�Ĳ���
				{
					TCB[OSPrioCur].State = TASK_READY;	// ��������״̬Ϊ
					OSSetPrioRdy(i);		    //�޸���������� ʹ���������������		
				}
			}
		}
		
		for( i = 0;(i < OS_TASKS) && (!(OSRdyTbl & (0x01<<i)));i++ );//����������ȼ��㷨
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
|��������:  RAYS_Init                                                                   |
|---------------------------------------------------------------------------------------|
|����ԭ��:  void RAYS_Init(void)                                                        |
|��������:  ��ʼ��ϵͳȫ�ֱ���                                                          |
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
|��������:  IdleTask                                                                    |
|---------------------------------------------------------------------------------------|
|����ԭ��:  void IdleTask(void)                                                         |
|��������:  ��������                                                                    |
|�й�˵��:  �������������Զ���ھ���״̬                                                |
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


///////����CPUʹ����//////////////////////////////////////
#define CPU_USAGE_CALC_TICK    1000

static	uint32_t totalCount= 0 ;
static	uint32_t count = 0;	
static float cpuUsage = 0;

void OS_Idlehook(void)
{
 
	uint32_t tick;

	if(totalCount == 0)//ֻ�ڿ����ĵ�һ�ν���ִ��
	{
		OS_NO_TIMELY_SW();//ֹͣ������
		tick = OS_GetTick();
		while(OS_GetTick() - tick < CPU_USAGE_CALC_TICK)
		{
			totalCount++;
		}
		OS_EN_TIMELY_SW();//����������
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

