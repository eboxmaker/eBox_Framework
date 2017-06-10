/*
 ************************************************************************************************************************
 *                                                      uC/OS-III
 *                                                 The Real-Time Kernel
 *
 *                                  (c) Copyright 2009-2012; Micrium, Inc.; Weston, FL
 *                           All rights reserved.  Protected by international copyright laws.
 *
 *                                                    CORE FUNCTIONS
 *
 * File    : OS_CORE.C
 * By      : JJL
 * Version : V3.03.01
 *
 * LICENSING TERMS:
 * ---------------
 *           uC/OS-III is provided in source form for FREE short-term evaluation, for educational use or
 *           for peaceful research.  If you plan or intend to use uC/OS-III in a commercial application/
 *           product then, you need to contact Micrium to properly license uC/OS-III for its use in your
 *           application/product.   We provide ALL the source code for your convenience and to help you
 *           experience uC/OS-III.  The fact that the source is provided does NOT mean that you can use
 *           it commercially without paying a licensing fee.
 *
 *           Knowledge of the source code may NOT be used to develop a similar product.
 *
 *           Please help us continue to provide the embedded community with the finest software available.
 *           Your honesty is greatly appreciated.
 *
 *           You can contact us at www.micrium.com, or by phone at +1 (954) 217-2036.
 ************************************************************************************************************************
 */

#define  MICRIUM_SOURCE
#include <os.h>

#ifdef VSC_INCLUDE_SOURCE_FILE_NAMES
const CPU_CHAR  *os_core__c = "$Id: $";
#endif





/*
 ************************************************************************************************************************
 *                                                    INITIALIZATION
 *
 * Description: This function is used to initialize the internals of uC/OS-III and MUST be called prior to
 *              creating any uC/OS-III object and, prior to calling OS_Start().
 *
 * Arguments  : p_err         is a pointer to a variable that will contain an error code returned by this function.
 *
 *                                OS_ERR_NONE    Initialization was successful
 *                                Other          Other OS_ERR_xxx depending on the sub-functions called by OSInit().
 * Returns    : none
 ************************************************************************************************************************
 */

void  OSInit (OS_ERR *p_err)
{
    CPU_STK      *p_stk;
    CPU_STK_SIZE size;



#ifdef OS_SAFETY_CRITICAL
    if (p_err == (OS_ERR *)0) {
        OS_SAFETY_CRITICAL_EXCEPTION();
        return;
    }
#endif

    OSInitHook();                                           /* Call port specific initialization code                 */

    OSIntNestingCtr                 = (OS_NESTING_CTR)0;    /* Clear the interrupt nesting counter                    */

    OSRunning                       =  OS_STATE_OS_STOPPED; /* Indicate that multitasking not started                 */

    OSSchedLockNestingCtr           = (OS_NESTING_CTR)0;    /* Clear the scheduling lock counter                      */

    OSTCBCurPtr                     = (OS_TCB *)0;          /* Initialize OS_TCB pointers to a known state            */
    OSTCBHighRdyPtr                 = (OS_TCB *)0;

    OSPrioCur                       = (OS_PRIO)0;           /* Initialize priority variables to a known state         */
    OSPrioHighRdy                   = (OS_PRIO)0;
    OSPrioSaved                     = (OS_PRIO)0;

#if OS_CFG_SCHED_LOCK_TIME_MEAS_EN > 0u
    OSSchedLockTimeBegin            = (CPU_TS)0;
    OSSchedLockTimeMax              = (CPU_TS)0;
    OSSchedLockTimeMaxCur           = (CPU_TS)0;
#endif

#ifdef OS_SAFETY_CRITICAL_IEC61508
    OSSafetyCriticalStartFlag       =  DEF_FALSE;
#endif

#if OS_CFG_SCHED_ROUND_ROBIN_EN > 0u
    OSSchedRoundRobinEn             = DEF_FALSE;
    OSSchedRoundRobinDfltTimeQuanta = OSCfg_TickRate_Hz / 10u;
#endif

    if (OSCfg_ISRStkSize > (CPU_STK_SIZE)0) {
        p_stk = OSCfg_ISRStkBasePtr;                        /* Clear exception stack for stack checking.              */
        if (p_stk != (CPU_STK *)0) {
            size  = OSCfg_ISRStkSize;
            while (size > (CPU_STK_SIZE)0) {
                size--;
                *p_stk = (CPU_STK)0;
                p_stk++;
            }
        }
    }

#if OS_CFG_APP_HOOKS_EN > 0u
    OS_AppTaskCreateHookPtr = (OS_APP_HOOK_TCB )0;          /* Clear application hook pointers                        */
    OS_AppTaskDelHookPtr    = (OS_APP_HOOK_TCB )0;
    OS_AppTaskReturnHookPtr = (OS_APP_HOOK_TCB )0;

    OS_AppIdleTaskHookPtr   = (OS_APP_HOOK_VOID)0;
    OS_AppStatTaskHookPtr   = (OS_APP_HOOK_VOID)0;
    OS_AppTaskSwHookPtr     = (OS_APP_HOOK_VOID)0;
    OS_AppTimeTickHookPtr   = (OS_APP_HOOK_VOID)0;
#endif

#if OS_CFG_TASK_REG_TBL_SIZE > 0u
    OSTaskRegNextAvailID    = (OS_REG_ID)0;
#endif

    OS_PrioInit();                                          /* Initialize the priority bitmap table                   */

    OS_RdyListInit();                                       /* Initialize the Ready List                              */


#if OS_CFG_FLAG_EN > 0u                                     /* Initialize the Event Flag module                       */
    OS_FlagInit(p_err);
    if (*p_err != OS_ERR_NONE) {
        return;
    }
#endif


#if OS_CFG_MEM_EN > 0u                                      /* Initialize the Memory Manager module                   */
    OS_MemInit(p_err);
    if (*p_err != OS_ERR_NONE) {
        return;
    }
#endif


#if (OS_MSG_EN) > 0u                                        /* Initialize the free list of OS_MSGs                    */
    OS_MsgPoolInit(p_err);
    if (*p_err != OS_ERR_NONE) {
        return;
    }
#endif


#if OS_CFG_MUTEX_EN > 0u                                    /* Initialize the Mutex Manager module                    */
    OS_MutexInit(p_err);
    if (*p_err != OS_ERR_NONE) {
        return;
    }
#endif


#if OS_CFG_Q_EN > 0u
    OS_QInit(p_err);                                        /* Initialize the Message Queue Manager module            */
    if (*p_err != OS_ERR_NONE) {
        return;
    }
#endif


#if OS_CFG_SEM_EN > 0u                                      /* Initialize the Semaphore Manager module                */
    OS_SemInit(p_err);
    if (*p_err != OS_ERR_NONE) {
        return;
    }
#endif


#if defined(OS_CFG_TLS_TBL_SIZE) && (OS_CFG_TLS_TBL_SIZE > 0u)
    OS_TLS_Init(p_err);                                     /* Initialize Task Local Storage, before creating tasks   */
    if (*p_err != OS_ERR_NONE) {
        return;
    }
#endif


    OS_TaskInit(p_err);                                     /* Initialize the task manager                            */
    if (*p_err != OS_ERR_NONE) {
        return;
    }


#if OS_CFG_ISR_POST_DEFERRED_EN > 0u
    OS_IntQTaskInit(p_err);                                 /* Initialize the Interrupt Queue Handler Task            */
    if (*p_err != OS_ERR_NONE) {
        return;
    }
#endif


    OS_IdleTaskInit(p_err);                                 /* Initialize the Idle Task                               */
    if (*p_err != OS_ERR_NONE) {
        return;
    }


    OS_TickTaskInit(p_err);                                 /* Initialize the Tick Task                               */
    if (*p_err != OS_ERR_NONE) {
        return;
    }


#if OS_CFG_STAT_TASK_EN > 0u                                /* Initialize the Statistic Task                          */
    OS_StatTaskInit(p_err);
    if (*p_err != OS_ERR_NONE) {
        return;
    }
#endif


#if OS_CFG_TMR_EN > 0u                                      /* Initialize the Timer Manager module                    */
    OS_TmrInit(p_err);
    if (*p_err != OS_ERR_NONE) {
        return;
    }
#endif


#if OS_CFG_DBG_EN > 0u
    OS_Dbg_Init();
#endif


    OSCfg_Init();
}

/*$PAGE*/
/*
 ************************************************************************************************************************
 *                                                      ENTER ISR
 *
 * Description: This function is used to notify uC/OS-III that you are about to service an interrupt service routine
 *              (ISR).  This allows uC/OS-III to keep track of interrupt nesting and thus only perform rescheduling at
 *              the last nested ISR.
 *
 * Arguments  : none
 *
 * Returns    : none
 *
 * Note(s)    : 1) This function MUST be called with interrupts already disabled
 *
 *              2) Your ISR can directly increment 'OSIntNestingCtr' without calling this function because OSIntNestingCtr has
 *                 been declared 'global', the port is actually considered part of the OS and thus is allowed to access
 *                 uC/OS-III variables.
 *
 *              3) You MUST still call OSIntExit() even though you increment 'OSIntNestingCtr' directly.
 *
 *              4) You MUST invoke OSIntEnter() and OSIntExit() in pair.  In other words, for every call to OSIntEnter()
 *                 (or direct increment to OSIntNestingCtr) at the beginning of the ISR you MUST have a call to OSIntExit()
 *                 at the end of the ISR.
 *
 *              5) You are allowed to nest interrupts up to 250 levels deep.
 ************************************************************************************************************************
 */

void  OSIntEnter (void)
{
    if (OSRunning != OS_STATE_OS_RUNNING) {                 /* Is OS running?                                         */
        return;                                             /* No                                                     */
    }

    if (OSIntNestingCtr >= (OS_NESTING_CTR)250u) {          /* Have we nested past 250 levels?                        */
        return;                                             /* Yes                                                    */
    }

    OSIntNestingCtr++;                                      /* Increment ISR nesting level                            */
}

/*$PAGE*/
/*
 ************************************************************************************************************************
 *                                                       EXIT ISR
 *
 * Description: This function is used to notify uC/OS-III that you have completed servicing an ISR.  When the last nested
 *              ISR has completed, uC/OS-III will call the scheduler to determine whether a new, high-priority task, is
 *              ready to run.
 *
 * Arguments  : none
 *
 * Returns    : none
 *
 * Note(s)    : 1) You MUST invoke OSIntEnter() and OSIntExit() in pair.  In other words, for every call to OSIntEnter()
 *                 (or direct increment to OSIntNestingCtr) at the beginning of the ISR you MUST have a call to OSIntExit()
 *                 at the end of the ISR.
 *
 *              2) Rescheduling is prevented when the scheduler is locked (see OSSchedLock())
 ************************************************************************************************************************
 */

void  OSIntExit (void)
{
    CPU_SR_ALLOC();



    if (OSRunning != OS_STATE_OS_RUNNING) {                 /* Has the OS started?                                    */
        return;                                             /* No                                                     */
    }

    CPU_INT_DIS();
    if (OSIntNestingCtr == (OS_NESTING_CTR)0) {             /* Prevent OSIntNestingCtr from wrapping                  */
        CPU_INT_EN();
        return;
    }
    OSIntNestingCtr--;
    if (OSIntNestingCtr > (OS_NESTING_CTR)0) {              /* ISRs still nested?                                     */
        CPU_INT_EN();                                       /* Yes                                                    */
        return;
    }

    if (OSSchedLockNestingCtr > (OS_NESTING_CTR)0) {        /* Scheduler still locked?                                */
        CPU_INT_EN();                                       /* Yes                                                    */
        return;
    }

    OSPrioHighRdy   = OS_PrioGetHighest();                  /* Find highest priority                                  */
    OSTCBHighRdyPtr = OSRdyList[OSPrioHighRdy].HeadPtr;     /* Get highest priority task ready-to-run                 */
    if (OSTCBHighRdyPtr == OSTCBCurPtr) {                   /* Current task still the highest priority?               */
        CPU_INT_EN();                                       /* Yes                                                    */
        return;
    }

#if OS_CFG_TASK_PROFILE_EN > 0u
    OSTCBHighRdyPtr->CtxSwCtr++;                            /* Inc. # of context switches for this new task           */
#endif
    OSTaskCtxSwCtr++;                                       /* Keep track of the total number of ctx switches         */

#if defined(OS_CFG_TLS_TBL_SIZE) && (OS_CFG_TLS_TBL_SIZE > 0u)
    OS_TLS_TaskSw();
#endif

    OSIntCtxSw();                                           /* Perform interrupt level ctx switch                     */
    CPU_INT_EN();
}

/*$PAGE*/
/*
 ************************************************************************************************************************
 *                                    INDICATE THAT IT'S NO LONGER SAFE TO CREATE OBJECTS
 *
 * Description: This function is called by the application code to indicate that all initialization has been completed
 *              and that kernel objects are no longer allowed to be created.
 *
 * Arguments  : none
 *
 * Returns    : none
 *
 * Note(s)    : none
 ************************************************************************************************************************
 */

#ifdef OS_SAFETY_CRITICAL_IEC61508
void  OSSafetyCriticalStart (void)
{
    OSSafetyCriticalStartFlag = DEF_TRUE;
}

#endif

/*$PAGE*/
/*
 ************************************************************************************************************************
 *                                                      SCHEDULER
 *
 * Description: This function is called by other uC/OS-III services to determine whether a new, high priority task has
 *              been made ready to run.  This function is invoked by TASK level code and is not used to reschedule tasks
 *              from ISRs (see OSIntExit() for ISR rescheduling).
 *
 * Arguments  : none
 *
 * Returns    : none
 *
 * Note(s)    : 1) Rescheduling is prevented when the scheduler is locked (see OSSchedLock())
 ************************************************************************************************************************
 */

void  OSSched (void)
{
    CPU_SR_ALLOC();



    if (OSIntNestingCtr > (OS_NESTING_CTR)0) {              /* ISRs still nested?                                     */
        return;                                             /* Yes ... only schedule when no nested ISRs              */
    }

    if (OSSchedLockNestingCtr > (OS_NESTING_CTR)0) {        /* Scheduler locked?                                      */
        return;                                             /* Yes                                                    */
    }

    CPU_INT_DIS();
    OSPrioHighRdy   = OS_PrioGetHighest();                  /* Find the highest priority ready                        */
    OSTCBHighRdyPtr = OSRdyList[OSPrioHighRdy].HeadPtr;
    if (OSTCBHighRdyPtr == OSTCBCurPtr) {                   /* Current task is still highest priority task?           */
        CPU_INT_EN();                                       /* Yes ... no need to context switch                      */
        return;
    }

#if OS_CFG_TASK_PROFILE_EN > 0u
    OSTCBHighRdyPtr->CtxSwCtr++;                            /* Inc. # of context switches to this task                */
#endif
    OSTaskCtxSwCtr++;                                       /* Increment context switch counter                       */

#if defined(OS_CFG_TLS_TBL_SIZE) && (OS_CFG_TLS_TBL_SIZE > 0u)
    OS_TLS_TaskSw();
#endif

    OS_TASK_SW();                                           /* Perform a task level context switch                    */
    CPU_INT_EN();
}

/*$PAGE*/
/*
 ************************************************************************************************************************
 *                                                 PREVENT SCHEDULING
 *
 * Description: This function is used to prevent rescheduling from taking place.  This allows your application to prevent
 *              context switches until you are ready to permit context switching.
 *
 * Arguments  : p_err     is a pointer to a variable that will receive an error code:
 *
 *                            OS_ERR_NONE                 The scheduler is locked
 *                            OS_ERR_LOCK_NESTING_OVF     If you attempted to nest call to this function > 250 levels
 *                            OS_ERR_OS_NOT_RUNNING       If uC/OS-III is not running yet.
 *                            OS_ERR_SCHED_LOCK_ISR       If you called this function from an ISR.
 *
 * Returns    : none
 *
 * Note(s)    : 1) You MUST invoke OSSchedLock() and OSSchedUnlock() in pair.  In other words, for every
 *                 call to OSSchedLock() you MUST have a call to OSSchedUnlock().
 ************************************************************************************************************************
 */

void  OSSchedLock (OS_ERR *p_err)
{
    CPU_SR_ALLOC();



#ifdef OS_SAFETY_CRITICAL
    if (p_err == (OS_ERR *)0) {
        OS_SAFETY_CRITICAL_EXCEPTION();
        return;
    }
#endif

#if OS_CFG_CALLED_FROM_ISR_CHK_EN > 0u
    if (OSIntNestingCtr > (OS_NESTING_CTR)0) {              /* Not allowed to call from an ISR                        */
        *p_err = OS_ERR_SCHED_LOCK_ISR;
        return;
    }
#endif

    if (OSRunning != OS_STATE_OS_RUNNING) {                 /* Make sure multitasking is running                      */
        *p_err = OS_ERR_OS_NOT_RUNNING;
        return;
    }

    if (OSSchedLockNestingCtr >= (OS_NESTING_CTR)250u) {    /* Prevent OSSchedLockNestingCtr overflowing              */
        *p_err = OS_ERR_LOCK_NESTING_OVF;
        return;
    }

    CPU_CRITICAL_ENTER();
    OSSchedLockNestingCtr++;                                /* Increment lock nesting level                           */
#if OS_CFG_SCHED_LOCK_TIME_MEAS_EN > 0u
    OS_SchedLockTimeMeasStart();
#endif
    CPU_CRITICAL_EXIT();
    *p_err = OS_ERR_NONE;
}

/*$PAGE*/
/*
 ************************************************************************************************************************
 *                                                  ENABLE SCHEDULING
 *
 * Description: This function is used to re-allow rescheduling.
 *
 * Arguments  : p_err     is a pointer to a variable that will contain an error code returned by this function.
 *
 *                            OS_ERR_NONE
 *                            OS_ERR_OS_NOT_RUNNING       The scheduler has been enabled
 *                            OS_ERR_SCHED_LOCKED         The scheduler is still locked, still nested
 *                            OS_ERR_SCHED_NOT_LOCKED     The scheduler was not locked
 *                            OS_ERR_SCHED_UNLOCK_ISR     If you called this function from an ISR.
 *
 * Returns    : none
 *
 * Note(s)    : 1) You MUST invoke OSSchedLock() and OSSchedUnlock() in pair.  In other words, for every call to
 *                 OSSchedLock() you MUST have a call to OSSchedUnlock().
 ************************************************************************************************************************
 */

void  OSSchedUnlock (OS_ERR *p_err)
{
    CPU_SR_ALLOC();



#ifdef OS_SAFETY_CRITICAL
    if (p_err == (OS_ERR *)0) {
        OS_SAFETY_CRITICAL_EXCEPTION();
        return;
    }
#endif

#if OS_CFG_CALLED_FROM_ISR_CHK_EN > 0u
    if (OSIntNestingCtr > (OS_NESTING_CTR)0) {              /* Not allowed to call from an ISR                        */
        *p_err = OS_ERR_SCHED_UNLOCK_ISR;
        return;
    }
#endif

    if (OSRunning != OS_STATE_OS_RUNNING) {                 /* Make sure multitasking is running                      */
        *p_err = OS_ERR_OS_NOT_RUNNING;
        return;
    }

    if (OSSchedLockNestingCtr == (OS_NESTING_CTR)0) {       /* See if the scheduler is locked                         */
        *p_err = OS_ERR_SCHED_NOT_LOCKED;
        return;
    }

    CPU_CRITICAL_ENTER();
    OSSchedLockNestingCtr--;                                /* Decrement lock nesting level                           */
    if (OSSchedLockNestingCtr > (OS_NESTING_CTR)0) {
        CPU_CRITICAL_EXIT();                                /* Scheduler is still locked                              */
        *p_err = OS_ERR_SCHED_LOCKED;
        return;
    }

#if OS_CFG_SCHED_LOCK_TIME_MEAS_EN > 0u
    OS_SchedLockTimeMeasStop();
#endif

    CPU_CRITICAL_EXIT();                                    /* Scheduler should be re-enabled                         */
    OSSched();                                              /* Run the scheduler                                      */
    *p_err = OS_ERR_NONE;
}

/*$PAGE*/
/*
 ************************************************************************************************************************
 *                                      CONFIGURE ROUND-ROBIN SCHEDULING PARAMETERS
 *
 * Description: This function is called to change the round-robin scheduling parameters.
 *
 * Arguments  : en                determines whether round-robin will be enabled (when DEF_EN) or not (when DEF_DIS)
 *
 *              dflt_time_quanta  default number of ticks between time slices.  0 means assumes OSCfg_TickRate_Hz / 10.
 *
 *              p_err             is a pointer to a variable that will contain an error code returned by this function.
 *
 *                                    OS_ERR_NONE    The call was successful
 *
 * Returns    : none
 ************************************************************************************************************************
 */

#if OS_CFG_SCHED_ROUND_ROBIN_EN > 0u
void  OSSchedRoundRobinCfg (CPU_BOOLEAN en,
                            OS_TICK     dflt_time_quanta,
                            OS_ERR *    p_err)
{
    CPU_SR_ALLOC();



 #ifdef OS_SAFETY_CRITICAL
    if (p_err == (OS_ERR *)0) {
        OS_SAFETY_CRITICAL_EXCEPTION();
        return;
    }
 #endif

    CPU_CRITICAL_ENTER();
    if (en != DEF_ENABLED) {
        OSSchedRoundRobinEn = DEF_DISABLED;
    } else {
        OSSchedRoundRobinEn = DEF_ENABLED;
    }

    if (dflt_time_quanta > (OS_TICK)0) {
        OSSchedRoundRobinDfltTimeQuanta = dflt_time_quanta;
    } else {
        OSSchedRoundRobinDfltTimeQuanta = (OS_TICK)(OSCfg_TickRate_Hz / (OS_RATE_HZ)10);
    }
    CPU_CRITICAL_EXIT();
    *p_err = OS_ERR_NONE;
}
#endif

/*$PAGE*/
/*
 ************************************************************************************************************************
 *                                    YIELD CPU WHEN TASK NO LONGER NEEDS THE TIME SLICE
 *
 * Description: This function is called to give up the CPU when it is done executing before its time slice expires.
 *
 * Argument(s): p_err      is a pointer to a variable that will contain an error code returned by this function.
 *
 *                             OS_ERR_NONE                   The call was successful
 *                             OS_ERR_ROUND_ROBIN_1          Only 1 task at this priority, nothing to yield to
 *                             OS_ERR_ROUND_ROBIN_DISABLED   Round Robin is not enabled
 *                             OS_ERR_SCHED_LOCKED           The scheduler has been locked
 *                             OS_ERR_YIELD_ISR              Can't be called from an ISR
 *
 * Returns    : none
 *
 * Note(s)    : 1) This function MUST be called from a task.
 ************************************************************************************************************************
 */

#if OS_CFG_SCHED_ROUND_ROBIN_EN > 0u
void  OSSchedRoundRobinYield (OS_ERR *p_err)
{
    OS_RDY_LIST  *p_rdy_list;
    OS_TCB       *p_tcb;
    CPU_SR_ALLOC();



 #ifdef OS_SAFETY_CRITICAL
    if (p_err == (OS_ERR *)0) {
        OS_SAFETY_CRITICAL_EXCEPTION();
        return;
    }
 #endif

 #if OS_CFG_CALLED_FROM_ISR_CHK_EN > 0u
    if (OSIntNestingCtr > (OS_NESTING_CTR)0) {              /* Can't call this function from an ISR                   */
        *p_err = OS_ERR_YIELD_ISR;
        return;
    }
 #endif

    if (OSSchedLockNestingCtr > (OS_NESTING_CTR)0) {        /* Can't yield if the scheduler is locked                 */
        *p_err = OS_ERR_SCHED_LOCKED;
        return;
    }

    if (OSSchedRoundRobinEn != DEF_TRUE) {                  /* Make sure round-robin has been enabled                 */
        *p_err = OS_ERR_ROUND_ROBIN_DISABLED;
        return;
    }

    CPU_CRITICAL_ENTER();
    p_rdy_list = &OSRdyList[OSPrioCur];                     /* Can't yield if it's the only task at that priority     */
    if (p_rdy_list->NbrEntries < (OS_OBJ_QTY)2) {
        CPU_CRITICAL_EXIT();
        *p_err = OS_ERR_ROUND_ROBIN_1;
        return;
    }

    OS_RdyListMoveHeadToTail(p_rdy_list);                   /* Move current OS_TCB to the end of the list             */
    p_tcb = p_rdy_list->HeadPtr;                            /* Point to new OS_TCB at head of the list                */
    if (p_tcb->TimeQuanta == (OS_TICK)0) {                  /* See if we need to use the default time slice           */
        p_tcb->TimeQuantaCtr = OSSchedRoundRobinDfltTimeQuanta;
    } else {
        p_tcb->TimeQuantaCtr = p_tcb->TimeQuanta;           /* Load time slice counter with new time                  */
    }

    CPU_CRITICAL_EXIT();

    OSSched();                                              /* Run new task                                           */
    *p_err = OS_ERR_NONE;
}
#endif

/*$PAGE*/
/*
 ************************************************************************************************************************
 *                                                 START MULTITASKING
 *
 * Description: This function is used to start the multitasking process which lets uC/OS-III manages the task that you
 *              created.  Before you can call OSStart(), you MUST have called OSInit() and you MUST have created at least
 *              one application task.
 *
 * Argument(s): p_err      is a pointer to a variable that will contain an error code returned by this function.
 *
 *                             OS_ERR_FATAL_RETURN    OS was running and OSStart() returned.
 *                             OS_ERR_OS_RUNNING      OS is already running, OSStart() has no effect
 *
 * Returns    : none
 *
 * Note(s)    : 1) OSStartHighRdy() MUST:
 *                 a) Call OSTaskSwHook() then,
 *                 b) Load the context of the task pointed to by OSTCBHighRdyPtr.
 *                 c) Execute the task.
 *
 *              2) OSStart() is not supposed to return.  If it does, that would be considered a fatal error.
 ************************************************************************************************************************
 */

void  OSStart (OS_ERR *p_err)
{
#ifdef OS_SAFETY_CRITICAL
    if (p_err == (OS_ERR *)0) {
        OS_SAFETY_CRITICAL_EXCEPTION();
        return;
    }
#endif

    if (OSRunning == OS_STATE_OS_STOPPED) {
        OSPrioHighRdy   = OS_PrioGetHighest();              /* Find the highest priority                              */
        OSPrioCur       = OSPrioHighRdy;
        OSTCBHighRdyPtr = OSRdyList[OSPrioHighRdy].HeadPtr;
        OSTCBCurPtr     = OSTCBHighRdyPtr;
        OSRunning       = OS_STATE_OS_RUNNING;
        OSStartHighRdy();                                   /* Execute target specific code to start task             */
        *p_err           = OS_ERR_FATAL_RETURN;             /* OSStart() is not supposed to return                    */
    } else {
        *p_err           = OS_ERR_OS_RUNNING;               /* OS is already running                                  */
    }
}

/*$PAGE*/
/*
 ************************************************************************************************************************
 *                                                    GET VERSION
 *
 * Description: This function is used to return the version number of uC/OS-III.  The returned value corresponds to
 *              uC/OS-III's version number multiplied by 10000.  In other words, version 3.01.02 would be returned as 30102.
 *
 * Arguments  : p_err   is a pointer to a variable that will receive an error code.  However, OSVersion() set this
 *                      variable to
 *
 *                         OS_ERR_NONE
 *
 * Returns    : The version number of uC/OS-III multiplied by 10000.
 ************************************************************************************************************************
 */

CPU_INT16U  OSVersion (OS_ERR *p_err)
{
#ifdef OS_SAFETY_CRITICAL
    if (p_err == (OS_ERR *)0) {
        OS_SAFETY_CRITICAL_EXCEPTION();
        return ((CPU_INT16U)0u);
    }
#endif

    *p_err = OS_ERR_NONE;
    return (OS_VERSION);
}

/*$PAGE*/
/*
 ************************************************************************************************************************
 *                                                      IDLE TASK
 *
 * Description: This task is internal to uC/OS-III and executes whenever no other higher priority tasks executes because
 *              they are ALL waiting for event(s) to occur.
 *
 * Arguments  : p_arg    is an argument passed to the task when the task is created.
 *
 * Returns    : none
 *
 * Note(s)    : 1) This function is INTERNAL to uC/OS-III and your application MUST NOT call it.
 *
 *              2) OSIdleTaskHook() is called after the critical section to ensure that interrupts will be enabled for at
 *                 least a few instructions.  On some processors (ex. Philips XA), enabling and then disabling interrupts
 *                 doesn't allow the processor enough time to have interrupts enabled before they were disabled again.
 *                 uC/OS-III would thus never recognize interrupts.
 *
 *              3) This hook has been added to allow you to do such things as STOP the CPU to conserve power.
 ************************************************************************************************************************
 */

void  OS_IdleTask (void *p_arg)
{
    CPU_SR_ALLOC();



    p_arg = p_arg;                                          /* Prevent compiler warning for not using 'p_arg'         */

    while (DEF_ON) {
        CPU_CRITICAL_ENTER();
        OSIdleTaskCtr++;
#if OS_CFG_STAT_TASK_EN > 0u
        OSStatTaskCtr++;
#endif
        CPU_CRITICAL_EXIT();

        OSIdleTaskHook();                                   /* Call user definable HOOK                               */
    }
}

/*$PAGE*/
/*
 ************************************************************************************************************************
 *                                               INITIALIZE THE IDLE TASK
 *
 * Description: This function initializes the idle task
 *
 * Arguments  : p_err    is a pointer to a variable that will contain an error code returned by this function.
 *
 * Returns    : none
 *
 * Note(s)    : 1) This function is INTERNAL to uC/OS-III and your application MUST NOT call it.
 ************************************************************************************************************************
 */

void  OS_IdleTaskInit (OS_ERR *p_err)
{
#ifdef OS_SAFETY_CRITICAL
    if (p_err == (OS_ERR *)0) {
        OS_SAFETY_CRITICAL_EXCEPTION();
        return;
    }
#endif

    OSIdleTaskCtr = (OS_IDLE_CTR)0;
    /* ---------------- CREATE THE IDLE TASK ---------------- */
    OSTaskCreate((OS_TCB     *)&OSIdleTaskTCB,
                 (CPU_CHAR   *)((void *)"uC/OS-III Idle Task"),
                 (OS_TASK_PTR)OS_IdleTask,
                 (void       *)0,
                 (OS_PRIO     )(OS_CFG_PRIO_MAX - 1u),
                 (CPU_STK    *)OSCfg_IdleTaskStkBasePtr,
                 (CPU_STK_SIZE)OSCfg_IdleTaskStkLimit,
                 (CPU_STK_SIZE)OSCfg_IdleTaskStkSize,
                 (OS_MSG_QTY  )0u,
                 (OS_TICK     )0u,
                 (void       *)0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_NO_TLS),
                 (OS_ERR     *)p_err);
}

/*$PAGE*/
/*
 ************************************************************************************************************************
 *                                             BLOCK A TASK PENDING ON EVENT
 *
 * Description: This function is called to place a task in the blocked state waiting for an event to occur. This function
 *              exist because it is common to a number of OSxxxPend() services.
 *
 * Arguments  : p_pend_data    is a pointer to an object used to link the task being blocked to the list of task(s)
 *              -----------    pending on the desired object.
 *
 *              p_obj          is a pointer to the object to pend on.  If there are no object used to pend on then
 *              -----          the caller must pass a NULL pointer.
 *
 *              pending_on     Specifies what the task will be pending on:
 *
 *                                 OS_TASK_PEND_ON_FLAG
 *                                 OS_TASK_PEND_ON_TASK_Q     <- No object (pending for a message sent to the task)
 *                                 OS_TASK_PEND_ON_MUTEX
 *                                 OS_TASK_PEND_ON_Q
 *                                 OS_TASK_PEND_ON_SEM
 *                                 OS_TASK_PEND_ON_TASK_SEM   <- No object (pending on a signal sent to the task)
 *
 *              timeout        Is the amount of time the task will wait for the event to occur.
 *
 * Returns    : none
 *
 * Note(s)    : 1) This function is INTERNAL to uC/OS-III and your application MUST NOT call it.
 ************************************************************************************************************************
 */

void  OS_Pend (OS_PEND_DATA *   p_pend_data,
               OS_PEND_OBJ *    p_obj,
               OS_STATE         pending_on,
               OS_TICK          timeout)
{
    OS_PEND_LIST  *p_pend_list;



    OSTCBCurPtr->PendOn     = pending_on;                    /* Resource not available, wait until it is              */
    OSTCBCurPtr->PendStatus = OS_STATUS_PEND_OK;

    OS_TaskBlock(OSTCBCurPtr,                                /* Block the task and add it to the tick list if needed  */
                 timeout);

    if (p_obj != (OS_PEND_OBJ *)0) {                         /* Add the current task to the pend list ...             */
        p_pend_list             = &p_obj->PendList;          /* ... if there is an object to pend on                  */
        p_pend_data->PendObjPtr = p_obj;                     /* Save the pointer to the object pending on             */
        OS_PendDataInit((OS_TCB       *)OSTCBCurPtr,         /* Initialize the remaining field                        */
                        (OS_PEND_DATA *)p_pend_data,
                        (OS_OBJ_QTY    )1);
        OS_PendListInsertPrio(p_pend_list,                   /* Insert in the pend list in priority order             */
                              p_pend_data);
    } else {
        OSTCBCurPtr->PendDataTblEntries = (OS_OBJ_QTY    )0; /* If no object being pended on the clear these fields   */
        OSTCBCurPtr->PendDataTblPtr     = (OS_PEND_DATA *)0; /* ... in the TCB                                        */
    }
#if OS_CFG_DBG_EN > 0u
    OS_PendDbgNameAdd(p_obj,
                      OSTCBCurPtr);
#endif
}

/*$PAGE*/
/*
 ************************************************************************************************************************
 *                                                     ABORT PENDING
 *
 * Description: This function is called by OSxxxPendAbort() functions to abort pending on an event.
 *
 * Arguments  : p_obj          Is a pointer to the object to pend abort.
 *              -----
 *
 *              p_tcb          Is a pointer to the OS_TCB of the task that we'll abort the pend for
 *              -----
 *
 *              ts             The is a timestamp as to when the pend abort occurred
 *
 * Returns    : none
 *
 * Note(s)    : 1) This function is INTERNAL to uC/OS-III and your application MUST NOT call it.
 ************************************************************************************************************************
 */

void  OS_PendAbort (OS_PEND_OBJ *   p_obj,
                    OS_TCB *        p_tcb,
                    CPU_TS          ts)
{
    switch (p_tcb->TaskState) {
    case OS_TASK_STATE_RDY:                                 /* Cannot Pend Abort a task that is ready                 */
    case OS_TASK_STATE_DLY:                                 /* Cannot Pend Abort a task that is delayed               */
    case OS_TASK_STATE_SUSPENDED:                           /* Cannot Pend Abort a suspended task                     */
    case OS_TASK_STATE_DLY_SUSPENDED:                       /* Cannot Pend Abort a suspended task that was also dly'd */
        break;

    case OS_TASK_STATE_PEND:
    case OS_TASK_STATE_PEND_TIMEOUT:
        if (p_tcb->PendOn == OS_TASK_PEND_ON_MULTI) {
            OS_PendAbort1(p_obj,                                 /* Indicate which object was pend aborted            */
                          p_tcb,
                          ts);
        }
#if (OS_MSG_EN > 0u)
        p_tcb->MsgPtr     = (void      *)0;
        p_tcb->MsgSize    = (OS_MSG_SIZE)0u;
#endif
        p_tcb->TS         = ts;
        if (p_obj != (OS_PEND_OBJ *)0) {
            OS_PendListRemove(p_tcb);                            /* Remove task from all pend lists                   */
        }
        OS_TaskRdy(p_tcb);
        p_tcb->TaskState  = OS_TASK_STATE_RDY;                   /* Task will be ready                                */
        p_tcb->PendStatus = OS_STATUS_PEND_ABORT;                /* Indicate pend was aborted                         */
        p_tcb->PendOn     = OS_TASK_PEND_ON_NOTHING;             /* Indicate no longer pending                        */
        break;

    case OS_TASK_STATE_PEND_SUSPENDED:
    case OS_TASK_STATE_PEND_TIMEOUT_SUSPENDED:
        if (p_tcb->PendOn == OS_TASK_PEND_ON_MULTI) {
            OS_PendAbort1(p_obj,                                 /* Indicate which object was pend aborted            */
                          p_tcb,
                          ts);
        }
#if (OS_MSG_EN > 0u)
        p_tcb->MsgPtr     = (void      *)0;
        p_tcb->MsgSize    = (OS_MSG_SIZE)0u;
#endif
        p_tcb->TS         = ts;
        if (p_obj != (OS_PEND_OBJ *)0) {
            OS_PendListRemove(p_tcb);                            /* Remove task from all pend lists                   */
        }
        OS_TickListRemove(p_tcb);                                /* Cancel the timeout                                */
        p_tcb->TaskState  = OS_TASK_STATE_SUSPENDED;             /* Pend Aborted task is still suspended              */
        p_tcb->PendStatus = OS_STATUS_PEND_ABORT;                /* Indicate pend was aborted                         */
        p_tcb->PendOn     = OS_TASK_PEND_ON_NOTHING;             /* Indicate no longer pending                        */
        break;

    default:
        break;
    }
}

/*$PAGE*/
/*
 ************************************************************************************************************************
 *                                           PEND ABORT A TASK PENDING ON MULTIPLE OBJECTS
 *
 * Description: This function is called when a task is pending on multiple objects and one of the objects has been pend
 *              aborted.  This function needs to indicate to the caller which object was pend aborted by placing the
 *              address of the object in the OS_PEND_DATA table corresponding to the pend aborted object.
 *
 *              For example, if the task pends on six (6) objects, the address of those 6 objects are placed in the
 *              .PendObjPtr field of the OS_PEND_DATA table as shown below.  Note that the .PendDataTblEntries of the
 *              OS_TCB would be set to six (6) in this case.  As shown, when the pend call returns because a task pend
 *              aborted 'Obj C' then, only the one entry contains the .RdyObjPtr filled in data and the other entries
 *              contains NULL pointers and zero data.
 *
 *              You should note that the NULL pointers are zero data values are actually filled in by the pend call.
 *
 *
 *                                           .PendObjPtr    .RdyObjPtr     .RdyMsgPtr     .RdyMsgSize    .RdyTS
 *                                         +--------------+--------------+--------------+--------------+--------------+
 *              p_tcb->PendDataTblPtr  ->  |  Obj A       |  0           | 0            | 0            | 0            |
 *                                         +--------------+--------------+--------------+--------------+--------------+
 *                                         |  Obj B       |  0           | 0            | 0            | 0            |
 *                                         +--------------+--------------+--------------+--------------+--------------+
 *                                         |  Obj C       |  Obj C       | 0            | 0            | TS           |
 *                                         +--------------+--------------+--------------+--------------+--------------+
 *                                         |  Obj D       |  0           | 0            | 0            | 0            |
 *                                         +--------------+--------------+--------------+--------------+--------------+
 *                                         |  Obj E       |  0           | 0            | 0            | 0            |
 *                                         +--------------+--------------+--------------+--------------+--------------+
 *                                         |  Obj F       |  0           | 0            | 0            | 0            |
 *                                         +--------------+--------------+--------------+--------------+--------------+
 *
 *
 * Arguments  : p_obj        is a pointer to the object being pend aborted to
 *              -----
 *
 *              p_tcb        is a pointer to the OS_TCB of the task that we'll abort he pend for
 *              -----
 *
 *              ts           is the time stamp of when the pend abort occurred
 *
 * Returns    : none
 *
 * Note(s)    : 1) This function is INTERNAL to uC/OS-III and your application MUST NOT call it.
 ************************************************************************************************************************
 */

void  OS_PendAbort1 (OS_PEND_OBJ *  p_obj,
                     OS_TCB *       p_tcb,
                     CPU_TS         ts)
{
    OS_OBJ_QTY n_pend_list;                                         /* Number of pend lists                           */
    OS_PEND_DATA   *p_pend_data;



    p_pend_data = p_tcb->PendDataTblPtr;                            /* Point to the first OS_PEND_DATA to remove      */
    n_pend_list = p_tcb->PendDataTblEntries;                        /* Get number of entries in the table             */

    while (n_pend_list > (OS_OBJ_QTY)0) {                           /* Mark posted object in OS_PEND_DATA table       */
        if (p_obj == p_pend_data->PendObjPtr) {                     /* Did we find the object pend aborted?           */
            p_pend_data->RdyObjPtr = p_obj;                         /* Yes, indicate the object in the .RdyObjPtr     */
            p_pend_data->RdyTS     = ts;                            /*      save the timestamp of the pend abort      */
            break;
        }
        p_pend_data++;
        n_pend_list--;
    }
}

/*$PAGE*/
/*
 ************************************************************************************************************************
 *                                              INITIALIZE A WAIT LIST TABLE
 *
 * Description: This function is called to initialize the fields of a table of OS_PEND_DATA entries.  It's assumed that
 *              the .PendObjPtr field of each entry in the table is set by the caller and thus will NOT be touched by
 *              this function.
 *
 * Arguments  : p_tcb              is a pointer to the TCB of the task that we want to pend abort.
 *              -----
 *
 *              p_pend_data_tbl    is a pointer to a table (see below) of OS_PEND_DATA elements to initialize.
 *              ---------------
 *
 *                                  .PendObjPtr .RdyObjPtr .RdyMsgPtr .RdyMsgSize .RdyTS .TCBPtr .NextPtr .PrevPtr
 *                                 +-----------+----------+----------+-----------+------+-------+--------+--------+    ^
 *               p_pend_data_tbl-> |     ?     |  0       | 0        | 0         | 0    | p_tcb | 0      | 0      |    |
 *                                 +-----------+----------+----------+-----------+------+-------+--------+--------+    |
 *                                 |     ?     |  0       | 0        | 0         | 0    | p_tcb | 0      | 0      |    |
 *                                 +-----------+----------+----------+-----------+------+-------+--------+--------+    |
 *                                 |     ?     |  0       | 0        | 0         | 0    | p_tcb | 0      | 0      |    |
 *                                 +-----------+----------+----------+-----------+------+-------+--------+--------+  size
 *                                 |     ?     |  0       | 0        | 0         | 0    | p_tcb | 0      | 0      |    |
 *                                 +-----------+----------+----------+-----------+------+-------+--------+--------+    |
 *                                 |     ?     |  0       | 0        | 0         | 0    | p_tcb | 0      | 0      |    |
 *                                 +-----------+----------+----------+-----------+------+-------+--------+--------+    |
 *                                 |     ?     |  0       | 0        | 0         | 0    | p_tcb | 0      | 0      |    |
 *                                 +-----------+----------+----------+-----------+------+-------+--------+--------+    V
 *
 *              tbl_size           is the size of the table in number of entries
 *
 * Returns    : none
 *
 * Note(s)    : 1) This function is INTERNAL to uC/OS-III and your application must not call it.
 *
 *              2) It's possible for the table to be of size 1 when multi-pend is not used
 *
 *              3) Note that the .PendObjPtr is NOT touched because it's assumed to be set by the caller.
 ************************************************************************************************************************
 */

void  OS_PendDataInit (OS_TCB *         p_tcb,
                       OS_PEND_DATA *   p_pend_data_tbl,
                       OS_OBJ_QTY       tbl_size)
{
    OS_OBJ_QTY i;



    p_tcb->PendDataTblEntries = tbl_size;                   /* Link the TCB to the beginning of the table             */
    p_tcb->PendDataTblPtr     = p_pend_data_tbl;

    for (i = 0u; i < tbl_size; i++) {
        p_pend_data_tbl->NextPtr    = (OS_PEND_DATA *)0;    /* Initialize all the fields                              */
        p_pend_data_tbl->PrevPtr    = (OS_PEND_DATA *)0;
        p_pend_data_tbl->RdyObjPtr  = (OS_PEND_OBJ  *)0;
        p_pend_data_tbl->RdyMsgPtr  = (void         *)0;
        p_pend_data_tbl->RdyMsgSize = (OS_MSG_SIZE   )0;
        p_pend_data_tbl->RdyTS      = (CPU_TS        )0;
        p_pend_data_tbl->TCBPtr     = p_tcb;                /* Every entry points back to the TCB of the task         */
        p_pend_data_tbl++;
    }
}

/*$PAGE*/
/*
 ************************************************************************************************************************
 *                                     ADD/REMOVE DEBUG NAMES TO PENDED OBJECT AND OS_TCB
 *
 * Description: These functions are used to add pointers to ASCII 'names' of objects so they can easily be displayed
 *              using a kernel aware tool.
 *
 * Arguments  : p_obj              is a pointer to the object being pended on
 *
 *              p_tcb              is a pointer to the OS_TCB of the task pending on the object
 *
 * Returns    : none
 *
 * Note(s)    : 1) These functions are INTERNAL to uC/OS-III and your application must not call it.
 ************************************************************************************************************************
 */


#if OS_CFG_DBG_EN > 0u
void  OS_PendDbgNameAdd (OS_PEND_OBJ *  p_obj,
                         OS_TCB *       p_tcb)
{
    OS_PEND_LIST  *p_pend_list;
    OS_PEND_DATA  *p_pend_data;
    OS_TCB        *p_tcb1;


    if (p_obj != (OS_PEND_OBJ *)0) {
        p_tcb->DbgNamePtr =  p_obj->NamePtr;                /* Task pending on this object ... save name in TCB       */
        p_pend_list       = &p_obj->PendList;               /* Find name of HP task pending on this object ...        */
        p_pend_data       =  p_pend_list->HeadPtr;
        p_tcb1            =  p_pend_data->TCBPtr;
        p_obj->DbgNamePtr = p_tcb1->NamePtr;                /* ... Save in object                                     */
    } else {
        switch (p_tcb->PendOn) {
        case OS_TASK_PEND_ON_TASK_Q:
            p_tcb->DbgNamePtr = (CPU_CHAR *)((void *)"Task Q");
            break;

        case OS_TASK_PEND_ON_TASK_SEM:
            p_tcb->DbgNamePtr = (CPU_CHAR *)((void *)"Task Sem");
            break;

        default:
            p_tcb->DbgNamePtr = (CPU_CHAR *)((void *)" ");
            break;
        }
    }
}



void  OS_PendDbgNameRemove (OS_PEND_OBJ *   p_obj,
                            OS_TCB *        p_tcb)
{
    OS_PEND_LIST  *p_pend_list;
    OS_PEND_DATA  *p_pend_data;
    OS_TCB        *p_tcb1;


    p_tcb->DbgNamePtr = (CPU_CHAR *)((void *)" ");          /* Remove name of object pended on for readied task       */
    p_pend_list       = &p_obj->PendList;
    p_pend_data       =  p_pend_list->HeadPtr;
    if (p_pend_data  != (OS_PEND_DATA *)0) {
        p_tcb1            = p_pend_data->TCBPtr;
        p_obj->DbgNamePtr = p_tcb1->NamePtr;
    } else {
        p_obj->DbgNamePtr = (CPU_CHAR *)((void *)" ");      /* No other task pending on object                        */
    }
}
#endif

/*$PAGE*/
/*
 ************************************************************************************************************************
 *                               CHANGE THE PRIORITY OF A TASK WAITING IN ONE OR MORE PEND LISTS
 *
 * Description: This function is called to change the position of a task waiting in one or more pend lists.  Because a
 *              task can be waiting on multiple objects then each pend list needs to be updated.  Specifically, the
 *              task can be the highest priority task waiting on one pend list, the lowest priority task waiting in yet
 *              another pend list or somewhere else in another pend list.  Because of this, we need to be able to change
 *              each of those pend lists individually.
 *
 *              The drawing below shows an example of a task (OS_TCB) that belongs to 3 separate pend lists.  Each
 *              pend list can contain multiple tasks (the .PrevPtr and .NextPtr show a '?' to indicate this).  The OS_TCB
 *              contains a pointer (.PendDataTblPtr) to the first entry in the list of pend lists.
 *
 *                                                                                                          OS_TCB
 *                                                                                                   +--------------------+
 *                                                                                                   |                    |
 *                                                                                                   +--------------------+
 *                                                                                                   | PendDataTblEntries |
 *                                      Point to first entry in the OS_PEND_DATA table (i.e. [0])    +--------------------+
 *                                          /-----------------------------<------------------------- | PendDataTblPtr     |
 *                                          |                                                        +--------------------+
 *                                          |                                                                 ^
 *              OS_PEND_LIST                |                                                                 |
 *              +------------+              |                                                                 |
 *              | TailPtr    |              |                                                                 |
 *              +------------+              |                                                                 |
 *              | HeadPtr    |              |                                                                 |
 *              +------------+              |                                        /---------->-------------/
 *              | NbrEntries |              |                                        |                        |
 *              +------------+     [0]      V   OS_PEND_DATA                         |                        |
 *                                     +---------+------------+-------+---------+--------+---------+          |
 *                             ? <---- | PrevPtr | PendObjPtr |       |         | TCBPtr | NextPtr | --> ?    |
 *                                     +---------+------------+-------+---------+--------+---------+          |
 *                                                                                                            |
 *                                                                                                            |
 *                                                                                                            |
 *                                                                                                            |
 *                                                                                                            |
 *              OS_PEND_LIST                                                                Point back to TCB |
 *              +------------+                                                                                |
 *              | TailPtr    |                                                                                |
 *              +------------+                                                                                |
 *              | HeadPtr    |                                                                                |
 *              +------------+                                                      /----------->-------------/
 *              | NbrEntries |                                                      |                         |
 *              +------------+     [1]          OS_PEND_DATA                        |                         |
 *                                     +---------+------------+-------+---------+--------+---------+          |
 *                             ? <---- | PrevPtr | PendObjPtr |       |         | TCBPtr | NextPtr | --> ?    |
 *                                     +---------+------------+-------+---------+--------+---------+          |
 *                                                                                                            |
 *                                                                                                            |
 *                                                                                                            |
 *                                                                                                            |
 *                                                                                                            |
 *              OS_PEND_LIST                                                                                  |
 *              +------------+                                                                                |
 *              | TailPtr    |                                                                                |
 *              +------------+                                                                                |
 *              | HeadPtr    |                                                                                |
 *              +------------+                                                      /----------->-------------/
 *              | NbrEntries |                                                      |
 *              +------------+     [2]          OS_PEND_DATA                        |
 *                                     +---------+------------+-------+---------+--------+---------+
 *                             ? <---- | PrevPtr | PendObjPtr |       |         | TCBPtr | NextPtr | ----> ?
 *                                     +---------+------------+-------+---------+--------+---------+
 *
 *
 * Arguments  : p_tcb       is a pointer to the TCB of the task to move
 *              -----
 *
 *              prio_new    is the new priority for the task
 *
 * Returns    : none
 *
 * Note(s)    : 1) This function is INTERNAL to uC/OS-III and your application MUST NOT call it.
 *
 *              2) It's assumed that the TCB contains the NEW priority in its .Prio field.
 ************************************************************************************************************************
 */

void  OS_PendListChangePrio (OS_TCB *   p_tcb,
                             OS_PRIO    prio_new)
{
    OS_OBJ_QTY n_pend_list;                                         /* Number of pend lists                           */
    OS_PEND_DATA   *p_pend_data;
    OS_PEND_LIST   *p_pend_list;
    OS_PEND_OBJ    *p_obj;


    p_tcb->Prio = prio_new;
    p_pend_data = p_tcb->PendDataTblPtr;                            /* Point to first wait list entry                 */
    n_pend_list = p_tcb->PendDataTblEntries;                        /* Get the number of pend list task is in         */

    while (n_pend_list > 0u) {
        p_obj       =  p_pend_data->PendObjPtr;                     /* Get pointer to pend list                       */
        p_pend_list = &p_obj->PendList;
        if (p_pend_list->NbrEntries > 1u) {                         /* Only move if multiple entries in the list      */
            OS_PendListRemove1(p_pend_list,                         /* Remove entry from current position             */
                               p_pend_data);
            OS_PendListInsertPrio(p_pend_list,                      /* INSERT it back in the list                     */
                                  p_pend_data);
        }
        p_pend_data++;                                              /* Point to next wait list                        */
        n_pend_list--;
    }
}

/*$PAGE*/
/*
 ************************************************************************************************************************
 *                                                INITIALIZE A WAIT LIST
 *
 * Description: This function is called to initialize the fields of an OS_PEND_LIST.
 *
 * Arguments  : p_pend_list   is a pointer to an OS_PEND_LIST
 *              -----------
 *
 * Returns    : none
 *
 * Note(s)    : 1) This function is INTERNAL to uC/OS-III and your application must not call it.
 ************************************************************************************************************************
 */

void  OS_PendListInit (OS_PEND_LIST *p_pend_list)
{
    p_pend_list->HeadPtr    = (OS_PEND_DATA *)0;
    p_pend_list->TailPtr    = (OS_PEND_DATA *)0;
    p_pend_list->NbrEntries = (OS_OBJ_QTY    )0;
}

/*$PAGE*/
/*
 ************************************************************************************************************************
 *                                     INSERT PEND DATA AT THE BEGINNING OF A WAIT LIST
 *
 * Description: This function is called to place an OS_PEND_DATA entry at the beginning of a linked list as follows:
 *
 *              CASE 0: Insert in an empty list.
 *
 *                     OS_PEND_LIST
 *                     +--------------+
 *                     | TailPtr      |-> 0
 *                     +--------------+
 *                     | HeadPtr      |-> 0
 *                     +--------------+
 *                     | NbrEntries=0 |
 *                     +--------------+
 *
 *
 *
 *              CASE 1: Insert BEFORE the current head of list
 *
 *                     OS_PEND_LIST
 *                     +--------------+         OS_PEND_DATA
 *                     | TailPtr      |--+---> +------------+
 *                     +--------------+  |     | NextPtr    |->0
 *                     | HeadPtr      |--/     +------------+
 *                     +--------------+     0<-| PrevPtr    |
 *                     | NbrEntries=1 |        +------------+
 *                     +--------------+        |            |
 *                                             +------------+
 *                                             |            |
 *                                             +------------+
 *
 *
 * Arguments  : p_pend_list    is a pointer to a wait list found inside an object.  The OS_PEND_DATA entry will be
 *              -----------    inserted at the head of the list.
 *
 *              p_pend_data    is a pointer to the OS_PEND_DATA entry to add to the list
 *              -----------
 *
 * Returns    : none
 *
 * Note(s)    : 1) This function is INTERNAL to uC/OS-III and your application MUST NOT call it.
 ************************************************************************************************************************
 */

void  OS_PendListInsertHead (OS_PEND_LIST * p_pend_list,
                             OS_PEND_DATA * p_pend_data)
{
    OS_PEND_DATA  *p_pend_data_next;



    p_pend_list->NbrEntries++;                              /* One more entry in the list                             */
    p_pend_data->NextPtr  = p_pend_list->HeadPtr;           /* Adjust new entry's links                               */
    p_pend_data->PrevPtr  = (OS_PEND_DATA  *)0;
    p_pend_data_next      = p_pend_list->HeadPtr;           /* Adjust old head of list's links                        */
    if (p_pend_data_next != (OS_PEND_DATA *)0) {            /* See if we already have a head to replace               */
        p_pend_data_next->PrevPtr = p_pend_data;            /* Yes, point to new entry                                */
    }
    p_pend_list->HeadPtr = p_pend_data;                     /* We have a new list head                                */
    if (p_pend_list->NbrEntries == 1u) {
        p_pend_list->TailPtr = p_pend_data;
    }
}

/*$PAGE*/
/*
 ************************************************************************************************************************
 *                                   INSERT PEND DATA BASED ON IT'S PRIORITY IN A LIST
 *
 * Description: This function is called to place an OS_PEND_DATA entry in a linked list based on its priority.  The
 *              highest priority being placed at the head of the list.  It's assumed that the OS_PEND_DATA entry to
 *              insert points to the TCB of the task being inserted.  The TCB is also assumed to contain the priority
 *              of the task in its .Prio field.
 *
 *              CASE 0: Insert in an empty list.
 *
 *                     OS_PEND_LIST
 *                     +---------------+
 *                     | TailPtr       |-> 0
 *                     +---------------+
 *                     | HeadPtr       |-> 0
 *                     +---------------+
 *                     | NbrEntries=0  |
 *                     +---------------+
 *
 *
 *
 *              CASE 1: Insert BEFORE or AFTER an OS_TCB
 *
 *                     OS_PEND_LIST
 *                     +--------------+         OS_PEND_DATA
 *                     | TailPtr      |--+---> +------------+
 *                     +--------------+  |     | NextPtr    |->0
 *                     | HeadPtr      |--/     +------------+
 *                     +--------------+     0<-| PrevPtr    |
 *                     | NbrEntries=1 |        +------------+
 *                     +--------------+        |            |
 *                                             +------------+
 *                                             |            |
 *                                             +------------+
 *
 *
 *                     OS_PEND_LIST
 *                     +--------------+
 *                     | TailPtr      |-----------------------------------------------+
 *                     +--------------+         OS_PEND_DATA         OS_PEND_DATA     |    OS_PEND_DATA
 *                     | HeadPtr      |------> +------------+       +------------+    +-> +------------+
 *                     +--------------+        | NextPtr    |------>| NextPtr    | ...... | NextPtr    |->0
 *                     | NbrEntries=N |        +------------+       +------------+        +------------+
 *                     +--------------+     0<-| PrevPtr    |<------| PrevPtr    | ...... | PrevPtr    |
 *                                             +------------+       +------------+        +------------+
 *                                             |            |       |            |        |            |
 *                                             +------------+       +------------+        +------------+
 *                                             |            |       |            |        |            |
 *                                             +------------+       +------------+        +------------+
 *
 *
 * Arguments  : p_pend_list    is a pointer to the OS_PEND_LIST where the OS_PEND_DATA entry will be inserted
 *              -----------
 *
 *              p_pend_data    is the OS_PEND_DATA to insert in the list
 *              -----------
 *
 * Returns    : none
 *
 * Note(s)    : 1) This function is INTERNAL to uC/OS-III and your application MUST NOT call it.
 *
 *              2) 'p_pend_data->TCBPtr->Prio' contains the priority of the TCB associated with the entry to insert.
 *                 We can compare this priority with the priority of other entries in the list.
 ************************************************************************************************************************
 */

void  OS_PendListInsertPrio (OS_PEND_LIST * p_pend_list,
                             OS_PEND_DATA * p_pend_data)
{
    OS_PRIO prio;
    OS_TCB        *p_tcb;
    OS_TCB        *p_tcb_next;
    OS_PEND_DATA  *p_pend_data_prev;
    OS_PEND_DATA  *p_pend_data_next;



    p_tcb = p_pend_data->TCBPtr;                                      /* Obtain the priority of the task to insert    */
    prio  = p_tcb->Prio;
    if (p_pend_list->NbrEntries == (OS_OBJ_QTY)0) {                   /* CASE 0: Insert when there are no entries     */
        p_pend_list->NbrEntries = (OS_OBJ_QTY)1;                      /*         This is the first entry              */
        p_pend_data->NextPtr    = (OS_PEND_DATA *)0;                  /*         No other OS_PEND_DATAs in the list   */
        p_pend_data->PrevPtr    = (OS_PEND_DATA *)0;
        p_pend_list->HeadPtr    = p_pend_data;                        /*                                              */
        p_pend_list->TailPtr    = p_pend_data;
    } else {
        p_pend_list->NbrEntries++;                                    /* CASE 1: One more OS_PEND_DATA in the list    */
        p_pend_data_next = p_pend_list->HeadPtr;
        while (p_pend_data_next != (OS_PEND_DATA *)0) {               /*         Find the position where to insert    */
            p_tcb_next   = p_pend_data_next->TCBPtr;
            if (prio < p_tcb_next->Prio) {
                break;                                                /*         Found! ... insert BEFORE current     */
            } else {
                p_pend_data_next = p_pend_data_next->NextPtr;         /*         Not Found, follow the list           */
            }
        }
        if (p_pend_data_next == (OS_PEND_DATA *)0) {                  /*         TCB to insert is lower in prio       */
            p_pend_data->NextPtr      = (OS_PEND_DATA *)0;            /*         ... insert at the tail.              */
            p_pend_data_prev          = p_pend_list->TailPtr;
            p_pend_data->PrevPtr      = p_pend_data_prev;
            p_pend_data_prev->NextPtr = p_pend_data;
            p_pend_list->TailPtr      = p_pend_data;
        } else {
            if (p_pend_data_next->PrevPtr == (OS_PEND_DATA *)0) {     /*         Is new TCB highest priority?         */
                p_pend_data_next->PrevPtr  = p_pend_data;             /*         Yes, insert as new Head of list      */
                p_pend_data->PrevPtr       = (OS_PEND_DATA *)0;
                p_pend_data->NextPtr       = p_pend_data_next;
                p_pend_list->HeadPtr       = p_pend_data;
            } else {
                p_pend_data_prev           = p_pend_data_next->PrevPtr; /*        No,  insert in between two entries   */
                p_pend_data->PrevPtr       = p_pend_data_prev;
                p_pend_data->NextPtr       = p_pend_data_next;
                p_pend_data_prev->NextPtr  = p_pend_data;
                p_pend_data_next->PrevPtr  = p_pend_data;
            }
        }
    }
}

/*$PAGE*/
/*
 ************************************************************************************************************************
 *                           REMOVE TASK FROM PEND LIST(s) KNOWING ONLY WHICH TCB TO REMOVE
 *
 * Description: This function is called to remove a task from a pend list knowing only the TCB of the task to remove
 *
 *
 *              CASE 0: OS_PEND_DATA list is empty, nothing to do.
 *
 *              CASE 1: Only 1 OS_PEND_DATA in the list.
 *
 *                     OS_PEND_LIST
 *                     +--------------+         OS_PEND_DATA
 *                     | TailPtr      |--+---> +------------+
 *                     +--------------+  |     | NextPtr    |->0
 *                     | HeadPtr      |--/     +------------+
 *                     +--------------+     0<-| PrevPtr    |
 *                     | NbrEntries=1 |        +------------+
 *                     +--------------+        |            |
 *                                             +------------+
 *                                             |            |
 *                                             +------------+
 *
 *              CASE N: Two or more OS_PEND_DATAs in the list.
 *
 *                     OS_PEND_LIST
 *                     +--------------+
 *                     | TailPtr      |-----------------------------------------------+
 *                     +--------------+         OS_PEND_DATA         OS_PEND_DATA     |    OS_PEND_DATA
 *                     | HeadPtr      |------> +------------+       +------------+    +-> +------------+
 *                     +--------------+        | NextPtr    |------>| NextPtr    | ...... | NextPtr    |->0
 *                     | NbrEntries=N |        +------------+       +------------+        +------------+
 *                     +--------------+     0<-| PrevPtr    |<------| PrevPtr    | ...... | PrevPtr    |
 *                                             +------------+       +------------+        +------------+
 *                                             |            |       |            |        |            |
 *                                             +------------+       +------------+        +------------+
 *                                             |            |       |            |        |            |
 *                                             +------------+       +------------+        +------------+
 *
 *
 * Arguments  : p_tcb          is a pointer to the TCB of the task to remove from all pend lists
 *              -----
 *
 * Returns    : none
 *
 * Note(s)    : 1) This function is INTERNAL to uC/OS-III and your application MUST NOT call it.
 ************************************************************************************************************************
 */

void  OS_PendListRemove (OS_TCB *p_tcb)
{
    OS_OBJ_QTY n_pend_list;                                         /* Number of pend lists                           */
    OS_PEND_DATA   *p_pend_data;
    OS_PEND_LIST   *p_pend_list;
    OS_PEND_OBJ    *p_obj;



    p_pend_data = p_tcb->PendDataTblPtr;                            /* Point to the first OS_PEND_DATA to remove      */
    n_pend_list = p_tcb->PendDataTblEntries;                        /* Get number of entries in the table             */

    while (n_pend_list > (OS_OBJ_QTY)0) {
        p_obj       =  p_pend_data->PendObjPtr;                     /* Get pointer to pend list                       */
        p_pend_list = &p_obj->PendList;
        OS_PendListRemove1(p_pend_list,
                           p_pend_data);
        p_pend_data++;
        n_pend_list--;
    }
    p_tcb->PendDataTblEntries = (OS_OBJ_QTY    )0;
    p_tcb->PendDataTblPtr     = (OS_PEND_DATA *)0;
}

/*$PAGE*/
/*
 ************************************************************************************************************************
 *                               REMOVE AN 'OS_PEND_DATA' ENTRY from a 'OS_PEND_LIST'
 *
 * Description: This function is called to remove a task from a wait list knowing only the TCB of the task to remove
 *
 *
 *              CASE 1: Only 1 OS_PEND_DATA in the list.
 *
 *                     OS_PEND_LIST
 *                     +--------------+         OS_PEND_DATA
 *                     | TailPtr      |--+---> +------------+
 *                     +--------------+  |     | NextPtr    |->0
 *                     | HeadPtr      |--/      +------------+
 *                     +--------------+     0<-| PrevPtr    |
 *                     | NbrEntries=1 |        +------------+
 *                     +--------------+        |            |
 *                                             +------------+
 *                                             |            |
 *                                             +------------+
 *
 *              CASE N: Two or more OS_PEND_DATAs in the list.
 *
 *                     OS_PEND_LIST
 *                     +--------------+
 *                     | TailPtr      |-----------------------------------------------+
 *                     +--------------+         OS_PEND_DATA         OS_PEND_DATA     |    OS_PEND_DATA
 *                     | HeadPtr      |------> +------------+       +------------+    +-> +------------+
 *                     +--------------+        | NextPtr    |------>| NextPtr    | ...... | NextPtr    |->0
 *                     | NbrEntries=N |        +------------+       +------------+        +------------+
 *                     +--------------+     0<-| PrevPtr    |<------| PrevPtr    | ...... | PrevPtr    |
 *                                             +------------+       +------------+        +------------+
 *                                             |            |       |            |        |            |
 *                                             +------------+       +------------+        +------------+
 *                                             |            |       |            |        |            |
 *                                             +------------+       +------------+        +------------+
 *
 *
 * Arguments  : p_pend_list     is a pointer to the pend list where 'p_pend_data' will be removed from
 *              -----------
 *
 *              p_pend_data     is a pointer to the OS_PEND_DATA to remove from the pend list
 *              -----------
 *
 * Returns    : none
 *
 * Note(s)    : 1) This function is INTERNAL to uC/OS-III and your application MUST NOT call it.
 ************************************************************************************************************************
 */

void  OS_PendListRemove1 (OS_PEND_LIST *p_pend_list,
                          OS_PEND_DATA *p_pend_data)
{
    OS_PEND_DATA  *p_prev;
    OS_PEND_DATA  *p_next;



    if (p_pend_list->NbrEntries == 1u) {
        p_pend_list->HeadPtr = (OS_PEND_DATA *)0;           /* Only one entry in the pend list                        */
        p_pend_list->TailPtr = (OS_PEND_DATA *)0;

    } else if (p_pend_data->PrevPtr == (OS_PEND_DATA *)0) { /* See if entry is at the head of the list                */
        p_next               = p_pend_data->NextPtr;        /* Yes                                                    */
        p_next->PrevPtr      = (OS_PEND_DATA *)0;
        p_pend_list->HeadPtr = p_next;

    } else if (p_pend_data->NextPtr == (OS_PEND_DATA *)0) { /* See if entry is at the tail of the list                */
        p_prev               = p_pend_data->PrevPtr;        /* Yes                                                    */
        p_prev->NextPtr      = (OS_PEND_DATA *)0;
        p_pend_list->TailPtr = p_prev;

    } else {
        p_prev               = p_pend_data->PrevPtr;        /* Remove from inside the list                            */
        p_next               = p_pend_data->NextPtr;
        p_prev->NextPtr      = p_next;
        p_next->PrevPtr      = p_prev;
    }
    p_pend_list->NbrEntries--;                              /* One less entry in the list                             */
    p_pend_data->NextPtr = (OS_PEND_DATA *)0;
    p_pend_data->PrevPtr = (OS_PEND_DATA *)0;
}

/*$PAGE*/
/*
 ************************************************************************************************************************
 *                                READY A TASK THAT WAS PENDING ON AN OBJECT BEING DELETED
 *
 * Description: This function is called to make a task ready-to-run because an object is being deleted
 *
 * Arguments  : p_obj          is a pointer to the object being deleted
 *              -----
 *
 *              p_tcb          is a pointer to the OS_TCB of the task to make ready-to-run
 *              -----
 *
 *              ts             is a timestamp to indicate when the object was deleted
 *
 * Returns    : none
 *
 * Note(s)    : This function is INTERNAL to uC/OS-III and your application should not call it.
 ************************************************************************************************************************
 */

void  OS_PendObjDel (OS_PEND_OBJ *  p_obj,
                     OS_TCB *       p_tcb,
                     CPU_TS         ts)
{
    switch (p_tcb->TaskState) {
    case OS_TASK_STATE_RDY:                                      /* These states should never occur                   */
    case OS_TASK_STATE_DLY:
    case OS_TASK_STATE_SUSPENDED:
    case OS_TASK_STATE_DLY_SUSPENDED:
        break;

    case OS_TASK_STATE_PEND:
    case OS_TASK_STATE_PEND_TIMEOUT:
        if (p_tcb->PendOn == OS_TASK_PEND_ON_MULTI) {
            OS_PendObjDel1(p_obj,                                /* Indicate which object was pend aborted            */
                           p_tcb,
                           ts);
        }
#if (OS_MSG_EN > 0u)
        p_tcb->MsgPtr     = (void *)0;
        p_tcb->MsgSize    = (OS_MSG_SIZE)0u;
#endif
        p_tcb->TS         = ts;
        OS_PendListRemove(p_tcb);                                /* Remove task from all wait lists                   */
        OS_TaskRdy(p_tcb);
        p_tcb->TaskState  = OS_TASK_STATE_RDY;                   /* Task is readied because object is deleted         */
        p_tcb->PendStatus = OS_STATUS_PEND_DEL;
        p_tcb->PendOn     = OS_TASK_PEND_ON_NOTHING;
        break;

    case OS_TASK_STATE_PEND_SUSPENDED:
    case OS_TASK_STATE_PEND_TIMEOUT_SUSPENDED:
        if (p_tcb->PendOn == OS_TASK_PEND_ON_MULTI) {
            OS_PendObjDel1(p_obj,                                /* Indicate which object was pend aborted            */
                           p_tcb,
                           ts);
        }
#if (OS_MSG_EN > 0u)
        p_tcb->MsgPtr     = (void      *)0;
        p_tcb->MsgSize    = (OS_MSG_SIZE)0u;
#endif
        p_tcb->TS         = ts;
        OS_TickListRemove(p_tcb);                                /* Cancel the timeout                                */
        OS_PendListRemove(p_tcb);                                /* Remove task from all wait lists                   */
        p_tcb->TaskState  = OS_TASK_STATE_SUSPENDED;             /* Task needs to remain suspended                    */
        p_tcb->PendStatus = OS_STATUS_PEND_DEL;
        p_tcb->PendOn     = OS_TASK_PEND_ON_NOTHING;             /* Indicate no longer pending                        */
        break;

    default:
        break;
    }
}

/*$PAGE*/
/*
 ************************************************************************************************************************
 *                                    DELETE AN OBJECT FROM A TASK PENDING ON MULTIPLE OBJECTS
 *
 * Description: This function is called when a task is pending on multiple objects and the object is being deleted.
 *              This function needs to indicate to the caller which object was deleted by placing the address of the
 *              object in the OS_PEND_DATA table corresponding to the deleted object.
 *
 *              For example, if the task pends on six (6) objects, the address of those 6 objects are placed in the
 *              .PendObjPtr field of the OS_PEND_DATA table as shown below.  Note that the .PendDataTblEntries would be
 *              set to six (6) in this case.  As shown, when the pend call returns because a task deleted 'Obj C' then,
 *              only the one entry contains the filled in data and the other entries contains NULL pointers and zero
 *              data.
 *
 *              You should note that the NULL pointers are zero data values are actually filled in by the pend call.
 *
 *
 *                                           .PendObjPtr    .RdyObjPtr     .RdyMsgPtr     .RdyMsgSize    .RdyTS
 *                                         +--------------+--------------+--------------+--------------+--------------+
 *              p_tcb->PendDataTblPtr  ->  |  Obj A       |  0           | 0            | 0            | 0            |
 *                                         +--------------+--------------+--------------+--------------+--------------+
 *                                         |  Obj B       |  0           | 0            | 0            | 0            |
 *                                         +--------------+--------------+--------------+--------------+--------------+
 *                                         |  Obj C       |  Obj C       | 0            | 0            | TS           |
 *                                         +--------------+--------------+--------------+--------------+--------------+
 *                                         |  Obj D       |  0           | 0            | 0            | 0            |
 *                                         +--------------+--------------+--------------+--------------+--------------+
 *                                         |  Obj E       |  0           | 0            | 0            | 0            |
 *                                         +--------------+--------------+--------------+--------------+--------------+
 *                                         |  Obj F       |  0           | 0            | 0            | 0            |
 *                                         +--------------+--------------+--------------+--------------+--------------+
 *
 *
 * Arguments  : p_obj        is a pointer to the object being deleted
 *              -----
 *
 *              p_tcb        is the OS_TCB of the task pending on the object being deleted
 *              -----
 *
 *              ts           is the time stamp of when the object was deleted
 *
 * Returns    : none
 *
 * Note(s)    : 1) This function is INTERNAL to uC/OS-III and your application MUST NOT call it.
 ************************************************************************************************************************
 */

void  OS_PendObjDel1 (OS_PEND_OBJ * p_obj,
                      OS_TCB *      p_tcb,
                      CPU_TS        ts)
{
    OS_OBJ_QTY n_pend_list;                                         /* Number of pend lists                           */
    OS_PEND_DATA   *p_pend_data;



    p_pend_data = p_tcb->PendDataTblPtr;                            /* Point to the first OS_PEND_DATA to remove      */
    n_pend_list = p_tcb->PendDataTblEntries;                        /* Get number of entries in the table             */

    while (n_pend_list > (OS_OBJ_QTY)0) {                           /* Mark posted object in OS_PEND_DATA table       */
        if (p_obj == p_pend_data->PendObjPtr) {                     /* Did we find the object deleted?                */
            p_pend_data->RdyObjPtr = p_obj;                         /* Yes, indicate the object in the .RdyObjPtr     */
            p_pend_data->RdyTS     = ts;                            /*      save the timestamp                        */
            break;
        }
        p_pend_data++;
        n_pend_list--;
    }
}

/*$PAGE*/
/*
 ************************************************************************************************************************
 *                                                   POST TO A TASK
 *
 * Description: This function is called to post to a task.  This function exist because it is common to a number of
 *              OSxxxPost() services.
 *
 * Arguments  : p_obj          Is a pointer to the object being posted to or NULL pointer if there is no object
 *              -----
 *
 *              p_tcb          Is a pointer to the OS_TCB that will receive the 'post'
 *              -----
 *
 *              p_void         If we are posting a message to a task, this is the message that the task will receive
 *
 *              msg_size       If we are posting a message to a task, this is the size of the message
 *
 *              ts             The timestamp as to when the post occurred
 *
 * Returns    : none
 *
 * Note(s)    : This function is INTERNAL to uC/OS-III and your application should not call it.
 ************************************************************************************************************************
 */

void  OS_Post (OS_PEND_OBJ *p_obj,
               OS_TCB *     p_tcb,
               void *       p_void,
               OS_MSG_SIZE  msg_size,
               CPU_TS       ts)
{
    switch (p_tcb->TaskState) {
    case OS_TASK_STATE_RDY:                                      /* Cannot Pend Abort a task that is ready            */
    case OS_TASK_STATE_DLY:                                      /* Cannot Pend Abort a task that is delayed          */
    case OS_TASK_STATE_SUSPENDED:                                /* Cannot Post a suspended task                      */
    case OS_TASK_STATE_DLY_SUSPENDED:                            /* Cannot Post a suspended task that was also dly'd  */
        break;

    case OS_TASK_STATE_PEND:
    case OS_TASK_STATE_PEND_TIMEOUT:
        if (p_tcb->PendOn == OS_TASK_PEND_ON_MULTI) {
            OS_Post1(p_obj,                                      /* Indicate which object was posted to               */
                     p_tcb,
                     p_void,
                     msg_size,
                     ts);
        } else {
#if (OS_MSG_EN > 0u)
            p_tcb->MsgPtr  = p_void;                             /* Deposit message in OS_TCB of task waiting         */
            p_tcb->MsgSize = msg_size;                           /* ... assuming posting a message                    */
#endif
            p_tcb->TS      = ts;
        }
        if (p_obj != (OS_PEND_OBJ *)0) {
            OS_PendListRemove(p_tcb);                            /* Remove task from wait list(s)                     */
#if OS_CFG_DBG_EN > 0u
            OS_PendDbgNameRemove(p_obj,
                                 p_tcb);
#endif
        }
        OS_TaskRdy(p_tcb);                                       /* Make task ready to run                            */
        p_tcb->TaskState  = OS_TASK_STATE_RDY;
        p_tcb->PendStatus = OS_STATUS_PEND_OK;                   /* Clear pend status                                 */
        p_tcb->PendOn     = OS_TASK_PEND_ON_NOTHING;             /* Indicate no longer pending                        */
        break;

    case OS_TASK_STATE_PEND_SUSPENDED:
    case OS_TASK_STATE_PEND_TIMEOUT_SUSPENDED:
        if (p_tcb->PendOn == OS_TASK_PEND_ON_MULTI) {
            OS_Post1(p_obj,                                      /* Indicate which object was posted to               */
                     p_tcb,
                     p_void,
                     msg_size,
                     ts);
        } else {
#if (OS_MSG_EN > 0u)
            p_tcb->MsgPtr  = p_void;                             /* Deposit message in OS_TCB of task waiting         */
            p_tcb->MsgSize = msg_size;                           /* ... assuming posting a message                    */
#endif
            p_tcb->TS      = ts;
        }
        OS_TickListRemove(p_tcb);                                /* Cancel any timeout                                */
        if (p_obj != (OS_PEND_OBJ *)0) {
            OS_PendListRemove(p_tcb);                            /* Remove task from wait list(s)                     */
#if OS_CFG_DBG_EN > 0u
            OS_PendDbgNameRemove(p_obj,
                                 p_tcb);
#endif
        }
        p_tcb->TaskState  = OS_TASK_STATE_SUSPENDED;
        p_tcb->PendStatus = OS_STATUS_PEND_OK;                   /* Clear pend status                                 */
        p_tcb->PendOn     = OS_TASK_PEND_ON_NOTHING;             /* Indicate no longer pending                        */
        break;

    default:
        break;
    }
}

/*$PAGE*/
/*
 ************************************************************************************************************************
 *                                           POST TO A TASK PENDING ON MULTIPLE OBJECTS
 *
 * Description: This function is called when a task is pending on multiple objects and the object has been posted to.
 *              This function needs to indicate to the caller which object was posted to by placing the address of the
 *              object in the OS_PEND_DATA table corresponding to the posted object.
 *
 *              For example, if the task pends on six (6) objects, the address of those 6 objects are placed in the
 *              .PendObjPtr field of the OS_PEND_DATA table as shown below.  Note that the .PendDataTblEntries would be
 *              set to six (6) in this case.  As shown, when the pend call returns because a task or an ISR posted to
 *              'Obj C' then, only the one entry contains the filled in data and the other entries contains NULL pointers
 *              and zero data.
 *
 *              You should note that the NULL pointers are zero data values are actually filled in by the pend call.
 *
 *
 *                                           .PendObjPtr    .RdyObjPtr     .RdyMsgPtr     .RdyMsgSize    .RdyTS
 *                                         +--------------+--------------+--------------+--------------+--------------+
 *              p_tcb->PendDataTblPtr  ->  |  Obj A       |  0           | 0            | 0            | 0            |
 *                                         +--------------+--------------+--------------+--------------+--------------+
 *                                         |  Obj B       |  0           | 0            | 0            | 0            |
 *                                         +--------------+--------------+--------------+--------------+--------------+
 *                                         |  Obj C       |  Obj C       | Msg Ptr      | Msg Size     | TS           |
 *                                         +--------------+--------------+--------------+--------------+--------------+
 *                                         |  Obj D       |  0           | 0            | 0            | 0            |
 *                                         +--------------+--------------+--------------+--------------+--------------+
 *                                         |  Obj E       |  0           | 0            | 0            | 0            |
 *                                         +--------------+--------------+--------------+--------------+--------------+
 *                                         |  Obj F       |  0           | 0            | 0            | 0            |
 *                                         +--------------+--------------+--------------+--------------+--------------+
 *
 *
 * Arguments  : p_obj        is a pointer to the object being posted to
 *              -----
 *
 *              p_tcb        is the OS_TCB of the task receiving the signal or the message
 *              -----
 *
 *              p_void       is the actual message (assuming posting to a message queue).  A NULL pointer otherwise.
 *
 *              msg_size     is the size of the message sent (if posted to a message queue)
 *
 *              ts           is the time stamp of when the post occurred
 *
 * Returns    : none
 *
 * Note(s)    : 1) This function is INTERNAL to uC/OS-III and your application MUST NOT call it.
 ************************************************************************************************************************
 */

void  OS_Post1 (OS_PEND_OBJ *   p_obj,
                OS_TCB *        p_tcb,
                void *          p_void,
                OS_MSG_SIZE     msg_size,
                CPU_TS          ts)
{
    OS_OBJ_QTY n_pend_list;                                         /* Number of pend lists                           */
    OS_PEND_DATA   *p_pend_data;



    p_pend_data = p_tcb->PendDataTblPtr;                            /* Point to the first OS_PEND_DATA to remove      */
    n_pend_list = p_tcb->PendDataTblEntries;                        /* Get number of entries in the table             */

    while (n_pend_list > (OS_OBJ_QTY)0) {                           /* Mark posted object in OS_PEND_DATA table       */
        if (p_obj == p_pend_data->PendObjPtr) {                     /* Did we find the object posted to?              */
            p_pend_data->RdyObjPtr  = p_obj;                        /* Yes, indicate the object in the .RdyObjPtr     */
            p_pend_data->RdyMsgPtr  = p_void;                       /*      store the message posted                  */
            p_pend_data->RdyMsgSize = msg_size;                     /*      store the size of the message posted      */
            p_pend_data->RdyTS      = ts;                           /*      save the timestamp of the post            */
            break;
        }
        p_pend_data++;
        n_pend_list--;
    }
}

/*$PAGE*/
/*
 ************************************************************************************************************************
 *                                                    INITIALIZATION
 *                                               READY LIST INITIALIZATION
 *
 * Description: This function is called by OSInit() to initialize the ready list.  The ready list contains a list of all
 *              the tasks that are ready to run.  The list is actually an array of OS_RDY_LIST.  An OS_RDY_LIST contains
 *              three fields.  The number of OS_TCBs in the list (i.e. .NbrEntries), a pointer to the first OS_TCB in the
 *              OS_RDY_LIST (i.e. .HeadPtr) and a pointer to the last OS_TCB in the OS_RDY_LIST (i.e. .TailPtr).
 *
 *              OS_TCBs are doubly linked in the OS_RDY_LIST and each OS_TCB points pack to the OS_RDY_LIST it belongs
 *              to.
 *
 *              'OS_RDY_LIST  OSRdyTbl[OS_CFG_PRIO_MAX]'  looks like this once initialized:
 *
 *                               +---------------+--------------+
 *                               |               | TailPtr      |-----> 0
 *                          [0]  | NbrEntries=0  +--------------+
 *                               |               | HeadPtr      |-----> 0
 *                               +---------------+--------------+
 *                               |               | TailPtr      |-----> 0
 *                          [1]  | NbrEntries=0  +--------------+
 *                               |               | HeadPtr      |-----> 0
 *                               +---------------+--------------+
 *                                       :              :
 *                                       :              :
 *                                       :              :
 *                               +---------------+--------------+
 *                               |               | TailPtr      |-----> 0
 *          [OS_CFG_PRIO_MAX-1]  | NbrEntries=0  +--------------+
 *                               |               | HeadPtr      |-----> 0
 *                               +---------------+--------------+
 *
 *
 * Arguments  : none
 *
 * Returns    : none
 *
 * Note(s)    : This function is INTERNAL to uC/OS-III and your application should not call it.
 ************************************************************************************************************************
 */

void  OS_RdyListInit (void)
{
    OS_PRIO i;
    OS_RDY_LIST  *p_rdy_list;



    for (i = 0u; i < OS_CFG_PRIO_MAX; i++) {                /* Initialize the array of OS_RDY_LIST at each priority   */
        p_rdy_list = &OSRdyList[i];
        p_rdy_list->NbrEntries = (OS_OBJ_QTY)0;
        p_rdy_list->HeadPtr    = (OS_TCB   *)0;
        p_rdy_list->TailPtr    = (OS_TCB   *)0;
    }
}

/*$PAGE*/
/*
 ************************************************************************************************************************
 *                                             INSERT TCB IN THE READY LIST
 *
 * Description: This function is called to insert a TCB in the ready list.
 *
 *              The TCB is inserted at the tail of the list if the priority of the TCB is the same as the priority of the
 *              current task.  The TCB is inserted at the head of the list if not.
 *
 * Arguments  : p_tcb     is a pointer to the TCB to insert into the ready list
 *              -----
 *
 * Returns    : none
 *
 * Note(s)    : This function is INTERNAL to uC/OS-III and your application should not call it.
 ************************************************************************************************************************
 */

void  OS_RdyListInsert (OS_TCB *p_tcb)
{
    OS_PrioInsert(p_tcb->Prio);
    if (p_tcb->Prio == OSPrioCur) {                         /* Are we readying a task at the same prio?               */
        OS_RdyListInsertTail(p_tcb);                        /* Yes, insert readied task at the end of the list        */
    } else {
        OS_RdyListInsertHead(p_tcb);                        /* No,  insert readied task at the beginning of the list  */
    }
}


/*
 ************************************************************************************************************************
 *                                          INSERT TCB AT THE BEGINNING OF A LIST
 *
 * Description: This function is called to place an OS_TCB at the beginning of a linked list as follows:
 *
 *              CASE 0: Insert in an empty list.
 *
 *                     OS_RDY_LIST
 *                     +--------------+
 *                     | TailPtr      |-> 0
 *                     +--------------+
 *                     | HeadPtr      |-> 0
 *                     +--------------+
 *                     | NbrEntries=0 |
 *                     +--------------+
 *
 *
 *
 *              CASE 1: Insert BEFORE the current head of list
 *
 *                     OS_RDY_LIST
 *                     +--------------+          OS_TCB
 *                     | TailPtr      |--+---> +------------+
 *                     +--------------+  |     | NextPtr    |->0
 *                     | HeadPtr      |--/     +------------+
 *                     +--------------+     0<-| PrevPtr    |
 *                     | NbrEntries=1 |        +------------+
 *                     +--------------+        :            :
 *                                             :            :
 *                                             +------------+
 *
 *
 *                     OS_RDY_LIST
 *                     +--------------+
 *                     | TailPtr      |-----------------------------------------------+
 *                     +--------------+          OS_TCB               OS_TCB          |     OS_TCB
 *                     | HeadPtr      |------> +------------+       +------------+    +-> +------------+
 *                     +--------------+        | NextPtr    |------>| NextPtr    | ...... | NextPtr    |->0
 *                     | NbrEntries=N |        +------------+       +------------+        +------------+
 *                     +--------------+     0<-| PrevPtr    |<------| PrevPtr    | ...... | PrevPtr    |
 *                                             +------------+       +------------+        +------------+
 *                                             :            :       :            :        :            :
 *                                             :            :       :            :        :            :
 *                                             +------------+       +------------+        +------------+
 *
 *
 * Arguments  : p_tcb     is the OS_TCB to insert in the list
 *              -----
 *
 * Returns    : none
 *
 * Note(s)    : 1) This function is INTERNAL to uC/OS-III and your application MUST NOT call it.
 ************************************************************************************************************************
 */

void  OS_RdyListInsertHead (OS_TCB *p_tcb)
{
    OS_RDY_LIST  *p_rdy_list;
    OS_TCB       *p_tcb2;



    p_rdy_list = &OSRdyList[p_tcb->Prio];
    if (p_rdy_list->NbrEntries == (OS_OBJ_QTY)0) {          /* CASE 0: Insert when there are no entries               */
        p_rdy_list->NbrEntries =  (OS_OBJ_QTY)1;            /*         This is the first entry                        */
        p_tcb->NextPtr         =  (OS_TCB   *)0;            /*         No other OS_TCBs in the list                   */
        p_tcb->PrevPtr         =  (OS_TCB   *)0;
        p_rdy_list->HeadPtr    =  p_tcb;                    /*         Both list pointers point to this OS_TCB        */
        p_rdy_list->TailPtr    =  p_tcb;
    } else {                                                /* CASE 1: Insert BEFORE the current head of list         */
        p_rdy_list->NbrEntries++;                           /*         One more OS_TCB in the list                    */
        p_tcb->NextPtr         = p_rdy_list->HeadPtr;       /*         Adjust new OS_TCBs links                       */
        p_tcb->PrevPtr         = (OS_TCB    *)0;
        p_tcb2                 = p_rdy_list->HeadPtr;       /*         Adjust old head of list's links                */
        p_tcb2->PrevPtr        = p_tcb;
        p_rdy_list->HeadPtr    = p_tcb;
    }
}

/*$PAGE*/
/*
 ************************************************************************************************************************
 *                                           INSERT TCB AT THE END OF A LIST
 *
 * Description: This function is called to place an OS_TCB at the end of a linked list as follows:
 *
 *              CASE 0: Insert in an empty list.
 *
 *                     OS_RDY_LIST
 *                     +--------------+
 *                     | TailPtr      |-> 0
 *                     +--------------+
 *                     | HeadPtr      |-> 0
 *                     +--------------+
 *                     | NbrEntries=0 |
 *                     +--------------+
 *
 *
 *
 *              CASE 1: Insert AFTER the current tail of list
 *
 *                     OS_RDY_LIST
 *                     +--------------+          OS_TCB
 *                     | TailPtr      |--+---> +------------+
 *                     +--------------+  |     | NextPtr    |->0
 *                     | HeadPtr      |--/     +------------+
 *                     +--------------+     0<-| PrevPtr    |
 *                     | NbrEntries=1 |        +------------+
 *                     +--------------+        :            :
 *                                             :            :
 *                                             +------------+
 *
 *
 *                     OS_RDY_LIST
 *                     +--------------+
 *                     | TailPtr      |-----------------------------------------------+
 *                     +--------------+          OS_TCB               OS_TCB          |     OS_TCB
 *                     | HeadPtr      |------> +------------+       +------------+    +-> +------------+
 *                     +--------------+        | NextPtr    |------>| NextPtr    | ...... | NextPtr    |->0
 *                     | NbrEntries=N |        +------------+       +------------+        +------------+
 *                     +--------------+     0<-| PrevPtr    |<------| PrevPtr    | ...... | PrevPtr    |
 *                                             +------------+       +------------+        +------------+
 *                                             :            :       :            :        :            :
 *                                             :            :       :            :        :            :
 *                                             +------------+       +------------+        +------------+
 *
 *
 * Arguments  : p_tcb     is the OS_TCB to insert in the list
 *              -----
 *
 * Returns    : none
 *
 * Note(s)    : 1) This function is INTERNAL to uC/OS-III and your application MUST NOT call it.
 ************************************************************************************************************************
 */

void  OS_RdyListInsertTail (OS_TCB *p_tcb)
{
    OS_RDY_LIST  *p_rdy_list;
    OS_TCB       *p_tcb2;



    p_rdy_list = &OSRdyList[p_tcb->Prio];
    if (p_rdy_list->NbrEntries == (OS_OBJ_QTY)0) {          /* CASE 0: Insert when there are no entries               */
        p_rdy_list->NbrEntries  = (OS_OBJ_QTY)1;            /*         This is the first entry                        */
        p_tcb->NextPtr          = (OS_TCB   *)0;            /*         No other OS_TCBs in the list                   */
        p_tcb->PrevPtr          = (OS_TCB   *)0;
        p_rdy_list->HeadPtr     = p_tcb;                    /*         Both list pointers point to this OS_TCB        */
        p_rdy_list->TailPtr     = p_tcb;
    } else {                                                /* CASE 1: Insert AFTER the current tail of list          */
        p_rdy_list->NbrEntries++;                           /*         One more OS_TCB in the list                    */
        p_tcb->NextPtr          = (OS_TCB   *)0;            /*         Adjust new OS_TCBs links                       */
        p_tcb2                  = p_rdy_list->TailPtr;
        p_tcb->PrevPtr          = p_tcb2;
        p_tcb2->NextPtr         = p_tcb;                    /*         Adjust old tail of list's links                */
        p_rdy_list->TailPtr     = p_tcb;
    }
}

/*$PAGE*/
/*
 ************************************************************************************************************************
 *                                                MOVE TCB AT HEAD TO TAIL
 *
 * Description: This function is called to move the current head of a list to the tail of the list.
 *
 *
 *              CASE 0: TCB list is empty, nothing to do.
 *
 *              CASE 1: Only 1 OS_TCB  in the list, nothing to do.
 *
 *              CASE 2: Only 2 OS_TCBs in the list.
 *
 *                     OS_RDY_LIST
 *                     +--------------+
 *                     | TailPtr      |--------------------------+
 *                     +--------------+          OS_TCB          |     OS_TCB
 *                     | HeadPtr      |------> +------------+    +-> +------------+
 *                     +--------------+        | NextPtr    |------> | NextPtr    |->0
 *                     | NbrEntries=2 |        +------------+        +------------+
 *                     +--------------+     0<-| PrevPtr    | <------| PrevPtr    |
 *                                             +------------+        +------------+
 *                                             :            :        :            :
 *                                             :            :        :            :
 *                                             +------------+        +------------+
 *
 *
 *              CASE N: More than 2 OS_TCBs in the list.
 *
 *                     OS_RDY_LIST
 *                     +--------------+
 *                     | TailPtr      |-----------------------------------------------+
 *                     +--------------+          OS_TCB               OS_TCB          |     OS_TCB
 *                     | HeadPtr      |------> +------------+       +------------+    +-> +------------+
 *                     +--------------+        | NextPtr    |------>| NextPtr    | ...... | NextPtr    |->0
 *                     | NbrEntries=N |        +------------+       +------------+        +------------+
 *                     +--------------+     0<-| PrevPtr    |<------| PrevPtr    | ...... | PrevPtr    |
 *                                             +------------+       +------------+        +------------+
 *                                             :            :       :            :        :            :
 *                                             :            :       :            :        :            :
 *                                             +------------+       +------------+        +------------+
 *
 *
 * Arguments  : p_list    is a pointer to the OS_RDY_LIST where the OS_TCB will be inserted
 *              ------
 *
 * Returns    : none
 *
 * Note(s)    : 1) This function is INTERNAL to uC/OS-III and your application MUST NOT call it.
 ************************************************************************************************************************
 */

void  OS_RdyListMoveHeadToTail (OS_RDY_LIST *p_rdy_list)
{
    OS_TCB  *p_tcb1;
    OS_TCB  *p_tcb2;
    OS_TCB  *p_tcb3;



    switch (p_rdy_list->NbrEntries) {
    case 0:
    case 1:
        break;

    case 2:                                                 /* SWAP the TCBs                                          */
        p_tcb1              = p_rdy_list->HeadPtr;          /* Point to current head                                  */
        p_tcb2              = p_rdy_list->TailPtr;          /* Point to current tail                                  */
        p_tcb1->PrevPtr     = p_tcb2;
        p_tcb1->NextPtr     = (OS_TCB *)0;
        p_tcb2->PrevPtr     = (OS_TCB *)0;
        p_tcb2->NextPtr     = p_tcb1;
        p_rdy_list->HeadPtr = p_tcb2;
        p_rdy_list->TailPtr = p_tcb1;
        break;

    default:                                                /* Move only if there are more than 2 OS_TCBs in the list */
        p_tcb1              = p_rdy_list->HeadPtr;          /* Point to current head                                  */
        p_tcb2              = p_rdy_list->TailPtr;          /* Point to current tail                                  */
        p_tcb3              = p_tcb1->NextPtr;              /* Point to new list head                                 */
        p_tcb3->PrevPtr     = (OS_TCB *)0;                  /* Adjust back    link of new list head                   */
        p_tcb1->NextPtr     = (OS_TCB *)0;                  /* Adjust forward link of new list tail                   */
        p_tcb1->PrevPtr     = p_tcb2;                       /* Adjust back    link of new list tail                   */
        p_tcb2->NextPtr     = p_tcb1;                       /* Adjust forward link of old list tail                   */
        p_rdy_list->HeadPtr = p_tcb3;                       /* Adjust new list head and tail pointers                 */
        p_rdy_list->TailPtr = p_tcb1;
        break;
    }
}

/*$PAGE*/
/*
 ************************************************************************************************************************
 *                                REMOVE TCB FROM LIST KNOWING ONLY WHICH OS_TCB TO REMOVE
 *
 * Description: This function is called to remove an OS_TCB from an OS_RDY_LIST knowing the address of the OS_TCB to
 *              remove.
 *
 *
 *              CASE 0: TCB list is empty, nothing to do.
 *
 *              CASE 1: Only 1 OS_TCBs in the list.
 *
 *                     OS_RDY_LIST
 *                     +--------------+          OS_TCB
 *                     | TailPtr      |--+---> +------------+
 *                     +--------------+  |     | NextPtr    |->0
 *                     | HeadPtr      |--/     +------------+
 *                     +--------------+     0<-| PrevPtr    |
 *                     | NbrEntries=1 |        +------------+
 *                     +--------------+        :            :
 *                                             :            :
 *                                             +------------+
 *
 *              CASE N: Two or more OS_TCBs in the list.
 *
 *                     OS_RDY_LIST
 *                     +--------------+
 *                     | TailPtr      |-----------------------------------------------+
 *                     +--------------+          OS_TCB               OS_TCB          |     OS_TCB
 *                     | HeadPtr      |------> +------------+       +------------+    +-> +------------+
 *                     +--------------+        | NextPtr    |------>| NextPtr    | ...... | NextPtr    |->0
 *                     | NbrEntries=N |        +------------+       +------------+        +------------+
 *                     +--------------+     0<-| PrevPtr    |<------| PrevPtr    | ...... | PrevPtr    |
 *                                             +------------+       +------------+        +------------+
 *                                             :            :       :            :        :            :
 *                                             :            :       :            :        :            :
 *                                             +------------+       +------------+        +------------+
 *
 *
 * Arguments  : p_tcb    is a pointer to the OS_TCB to remove
 *              -----
 *
 * Returns    : A pointer to the OS_RDY_LIST where the OS_TCB was
 *
 * Note(s)    : 1) This function is INTERNAL to uC/OS-III and your application MUST NOT call it.
 ************************************************************************************************************************
 */

void  OS_RdyListRemove (OS_TCB *p_tcb)
{
    OS_RDY_LIST  *p_rdy_list;
    OS_TCB       *p_tcb1;
    OS_TCB       *p_tcb2;



    p_rdy_list = &OSRdyList[p_tcb->Prio];
    p_tcb1     = p_tcb->PrevPtr;                            /* Point to next and previous OS_TCB in the list          */
    p_tcb2     = p_tcb->NextPtr;
    if (p_tcb1 == (OS_TCB *)0) {                            /* Was the OS_TCB to remove was at the head?              */
        if (p_tcb2 == (OS_TCB *)0) {                        /* Yes, was it the only OS_TCB?                           */
            p_rdy_list->NbrEntries = (OS_OBJ_QTY)0;         /*      Yes, no more entries                              */
            p_rdy_list->HeadPtr    = (OS_TCB   *)0;
            p_rdy_list->TailPtr    = (OS_TCB   *)0;
            OS_PrioRemove(p_tcb->Prio);
        } else {
            p_rdy_list->NbrEntries--;                       /*      No,  one less entry                               */
            p_tcb2->PrevPtr        = (OS_TCB   *)0;         /*           adjust back link of new list head            */
            p_rdy_list->HeadPtr    = p_tcb2;                /*           adjust OS_RDY_LIST's new head                */
        }
    } else {
        p_rdy_list->NbrEntries--;                           /* No,  one less entry                                    */
        p_tcb1->NextPtr = p_tcb2;
        if (p_tcb2 == (OS_TCB *)0) {
            p_rdy_list->TailPtr = p_tcb1;                   /*      Removing the TCB at the tail, adj the tail ptr    */
        } else {
            p_tcb2->PrevPtr     = p_tcb1;
        }
    }
    p_tcb->PrevPtr = (OS_TCB *)0;
    p_tcb->NextPtr = (OS_TCB *)0;
}

/*$PAGE*/
/*
 ************************************************************************************************************************
 *                                             SCHEDULE THE ISR HANDLER TASK
 *
 * Description: This function is called by other uC/OS-III services to schedule task at priority 0 which is always the
 *              ISR handler task.
 *
 * Arguments  : none
 *
 * Returns    : none
 *
 * Note(s)    : 1) This function is INTERNAL to uC/OS-III and your application MUST NOT call it.
 ************************************************************************************************************************
 */

#if OS_CFG_ISR_POST_DEFERRED_EN > 0u
void  OS_Sched0 (void)
{
    CPU_SR_ALLOC();



    CPU_INT_DIS();
    OSPrioHighRdy   = (OS_PRIO)0;                           /* Force the priority to 0                                */
    OSTCBHighRdyPtr = &OSIntQTaskTCB;                       /* Always schedule the ISR handler task                   */
 #if OS_CFG_TASK_PROFILE_EN > 0u
    OSTCBHighRdyPtr->CtxSwCtr++;                            /* Inc. # of context switches to this task                */
 #endif
    OSTaskCtxSwCtr++;                                       /* Increment context switch counter                       */
    OS_TASK_SW();                                           /* Perform a task level context switch                    */
    CPU_INT_EN();
}
#endif

/*$PAGE*/
/*
 ************************************************************************************************************************
 *                                               SCHEDULER LOCK TIME MEASUREMENT
 *
 * Description: These functions are used to measure the peak amount of time that the scheduler is locked
 *
 * Arguments  : none
 *
 * Returns    : none
 *
 * Note(s)    : 1) The are internal functions to uC/OS-III and MUST not be called by your application code.
 *
 *              2) It's assumed that these functions are called when interrupts are disabled.
 *
 *              3) We are reading the CPU_TS_TmrRd() directly even if this is a 16-bit timer.  The reason is that we
 *                 don't expect to have the scheduler locked for 65536 counts even at the rate the TS timer is updated.
 *                 In other words, locking the scheduler for longer than 65536 count would not be a good thing for a
 *                 real-time system.
 ************************************************************************************************************************
 */

#if OS_CFG_SCHED_LOCK_TIME_MEAS_EN > 0u
void  OS_SchedLockTimeMeasStart (void)
{
    if (OSSchedLockNestingCtr == 1u) {
        OSSchedLockTimeBegin = CPU_TS_TmrRd();
    }
}




void  OS_SchedLockTimeMeasStop (void)
{
    CPU_TS_TMR delta;


    if (OSSchedLockNestingCtr == (OS_NESTING_CTR)0) {       /* Make sure we fully un-nested scheduler lock            */
        delta = CPU_TS_TmrRd()                              /* Compute the delta time between begin and end           */
                - OSSchedLockTimeBegin;
        if (OSSchedLockTimeMax    < delta) {                /* Detect peak value                                      */
            OSSchedLockTimeMax    = delta;
        }
        if (OSSchedLockTimeMaxCur < delta) {                /* Detect peak value (for resettable value)               */
            OSSchedLockTimeMaxCur = delta;
        }
    }
}
#endif

/*$PAGE*/
/*
 ************************************************************************************************************************
 *                                        RUN ROUND-ROBIN SCHEDULING ALGORITHM
 *
 * Description: This function is called on every tick to determine if a new task at the same priority needs to execute.
 *
 *
 * Arguments  : p_rdy_list    is a pointer to the OS_RDY_LIST entry of the ready list at the current priority
 *              ----------
 *
 * Returns    : none
 *
 * Note(s)    : 1) This function is INTERNAL to uC/OS-III and your application MUST NOT call it.
 ************************************************************************************************************************
 */

#if OS_CFG_SCHED_ROUND_ROBIN_EN > 0u
void  OS_SchedRoundRobin (OS_RDY_LIST *p_rdy_list)
{
    OS_TCB   *p_tcb;
    CPU_SR_ALLOC();



    if (OSSchedRoundRobinEn != DEF_TRUE) {                  /* Make sure round-robin has been enabled                 */
        return;
    }

    CPU_CRITICAL_ENTER();
    p_tcb = p_rdy_list->HeadPtr;                            /* Decrement time quanta counter                          */

    if (p_tcb == (OS_TCB *)0) {
        CPU_CRITICAL_EXIT();
        return;
    }

    if (p_tcb == &OSIdleTaskTCB) {
        CPU_CRITICAL_EXIT();
        return;
    }

    if (p_tcb->TimeQuantaCtr > (OS_TICK)0) {
        p_tcb->TimeQuantaCtr--;
    }

    if (p_tcb->TimeQuantaCtr > (OS_TICK)0) {                /* Task not done with its time quanta                     */
        CPU_CRITICAL_EXIT();
        return;
    }

    if (p_rdy_list->NbrEntries < (OS_OBJ_QTY)2) {           /* See if it's time to time slice current task            */
        CPU_CRITICAL_EXIT();                                /* ... only if multiple tasks at same priority            */
        return;
    }

    if (OSSchedLockNestingCtr > (OS_NESTING_CTR)0) {        /* Can't round-robin if the scheduler is locked           */
        CPU_CRITICAL_EXIT();
        return;
    }

    OS_RdyListMoveHeadToTail(p_rdy_list);                   /* Move current OS_TCB to the end of the list             */
    p_tcb = p_rdy_list->HeadPtr;                            /* Point to new OS_TCB at head of the list                */
    if (p_tcb->TimeQuanta == (OS_TICK)0) {                  /* See if we need to use the default time slice           */
        p_tcb->TimeQuantaCtr = OSSchedRoundRobinDfltTimeQuanta;
    } else {
        p_tcb->TimeQuantaCtr = p_tcb->TimeQuanta;           /* Load time slice counter with new time                  */
    }
    CPU_CRITICAL_EXIT();
}
#endif

/*$PAGE*/
/*
 ************************************************************************************************************************
 *                                                     BLOCK A TASK
 *
 * Description: This function is called to remove a task from the ready list and also insert it in the timer tick list if
 *              the specified timeout is non-zero.
 *
 * Arguments  : p_tcb          is a pointer to the OS_TCB of the task block
 *              -----
 *
 *              timeout        is the desired timeout
 *
 * Returns    : none
 *
 * Note(s)    : 1) This function is INTERNAL to uC/OS-III and your application MUST NOT call it.
 ************************************************************************************************************************
 */

void  OS_TaskBlock (OS_TCB *p_tcb,
                    OS_TICK timeout)
{
    OS_ERR err;


    if (timeout > (OS_TICK)0) {                             /* Add task to tick list if timeout non zero               */
        OS_TickListInsert(p_tcb,
                          timeout,
                          OS_OPT_TIME_TIMEOUT,
                          &err);
        if (err == OS_ERR_NONE) {
            p_tcb->TaskState = OS_TASK_STATE_PEND_TIMEOUT;
        } else {
            p_tcb->TaskState = OS_TASK_STATE_PEND;
        }
    } else {
        p_tcb->TaskState = OS_TASK_STATE_PEND;
    }
    OS_RdyListRemove(p_tcb);
}

/*$PAGE*/
/*
 ************************************************************************************************************************
 *                                                     READY A TASK
 *
 * Description: This function is called to make a task ready-to-run.
 *
 * Arguments  : p_tcb          is a pointer to the OS_TCB of the task to make ready-to-run
 *              -----
 *
 * Returns    : none
 *
 * Note(s)    : 1) This function is INTERNAL to uC/OS-III and your application MUST NOT call it.
 ************************************************************************************************************************
 */

void  OS_TaskRdy (OS_TCB *p_tcb)
{
    OS_TickListRemove(p_tcb);                               /* Remove from tick list                                  */
    if ((p_tcb->TaskState & OS_TASK_STATE_BIT_SUSPENDED) == (OS_STATE)0) {
        OS_RdyListInsert(p_tcb);                            /* Insert the task in the ready list                      */
    }
}
