#include "cpu.h"
#include "core.h"
OS_TCBP OS_Tcb_CurP; 
OS_TCBP OS_Tcb_HighRdyP;

/******************全局变量定义***************/
	INT32U OSTick;
	INT32U cpu_sr;//保存 PRIMASK;
	INT8U TIMELY_SW = 1;

	
  INT32U   			OSRdyTbl;     							// 就绪任务列表
  PRIO_TypeDef	OSPrioCur;		            	// 当前任务的优先级
  PRIO_TypeDef  OSPrioHighRdy	;           	// 即将要运行任务的优先级	
  OS_TCB TCB[OS_TASKS + 1];			// 定义任务控制块TCB数组

void CPU_TaskCreate(TASK_TypeDef task,STACK_TypeDef *stk,PRIO_TypeDef t_Prio)
{
    STACK_TypeDef  *p_stk;
    p_stk      = stk;
    p_stk      = (STACK_TypeDef *)((STACK_TypeDef)(p_stk) & 0xFFFFFFF8u);
    
    *(--p_stk) = (STACK_TypeDef)0x01000000uL;                          //xPSR
    *(--p_stk) = (STACK_TypeDef)task;                                  // Entry Point
    *(--p_stk) = (STACK_TypeDef)0x14141414uL;                          // R14 (LR)
    *(--p_stk) = (STACK_TypeDef)0x12121212uL;                          // R12
    *(--p_stk) = (STACK_TypeDef)0x03030303uL;                          // R3
    *(--p_stk) = (STACK_TypeDef)0x02020202uL;                          // R2
    *(--p_stk) = (STACK_TypeDef)0x01010101uL;                          // R1
    *(--p_stk) = (STACK_TypeDef)0x00000000u;                           // R0
    
    *(--p_stk) = (STACK_TypeDef)0x11111111uL;                          // R11
    *(--p_stk) = (STACK_TypeDef)0x10101010uL;                          // R10
    *(--p_stk) = (STACK_TypeDef)0x09090909uL;                          // R9
    *(--p_stk) = (STACK_TypeDef)0x08080808uL;                          // R8
    *(--p_stk) = (STACK_TypeDef)0x07070707uL;                          // R7
    *(--p_stk) = (STACK_TypeDef)0x06060606uL;                          // R6
    *(--p_stk) = (STACK_TypeDef)0x05050505uL;                          // R5
    *(--p_stk) = (STACK_TypeDef)0x04040404uL;                          // R4
    
    TCB[t_Prio].StkAddr=p_stk;
		TCB[t_Prio].OSTCBDly = 0;
		TCB[t_Prio].State = TASK_READY;
}

void CPU_OSStart(void)
 {
	 
	 OSStart();

 }
 
extern void OSTimelySw(void);//os.c中的定时调度函数
extern __IO uint32_t millis_seconds;
 
void CPU_ticks()
{
    OS_ENTER_CRITICAL();
        OSTick++;
        OSTimelySw();
    OS_EXIT_CRITICAL();

}

void SysTickInit()
{
    
    attch_sys_ticks_interrupt(CPU_ticks);
	
}


