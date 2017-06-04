/*
 ************************************************************************************************************************
 *                                                      uC/OS-III
 *                                                 The Real-Time Kernel
 *
 *                                  (c) Copyright 2009-2012; Micrium, Inc.; Weston, FL
 *                           All rights reserved.  Protected by international copyright laws.
 *
 *                                                   MUTEX MANAGEMENT
 *
 * File    : OS_MUTEX.C
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
const CPU_CHAR  *os_mutex__c = "$Id: $";
#endif


#if OS_CFG_MUTEX_EN > 0u
/*
 ************************************************************************************************************************
 *                                                   CREATE A MUTEX
 *
 * Description: This function creates a mutex.
 *
 * Arguments  : p_mutex       is a pointer to the mutex to initialize.  Your application is responsible for allocating
 *                            storage for the mutex.
 *
 *              p_name        is a pointer to the name you would like to give the mutex.
 *
 *              p_err         is a pointer to a variable that will contain an error code returned by this function.
 *
 *                                OS_ERR_NONE                    if the call was successful
 *                                OS_ERR_CREATE_ISR              if you called this function from an ISR
 *                                OS_ERR_ILLEGAL_CREATE_RUN_TIME if you are trying to create the Mutex after you called
 *                                                                 OSSafetyCriticalStart().
 *                                OS_ERR_NAME                    if 'p_name'  is a NULL pointer
 *                                OS_ERR_OBJ_CREATED             if the mutex has already been created
 *                                OS_ERR_OBJ_PTR_NULL            if 'p_mutex' is a NULL pointer
 *
 * Returns    : none
 ************************************************************************************************************************
 */

void  OSMutexCreate (OS_MUTEX * p_mutex,
                     CPU_CHAR * p_name,
                     OS_ERR *   p_err)
{
    CPU_SR_ALLOC();



 #ifdef OS_SAFETY_CRITICAL
    if (p_err == (OS_ERR *)0) {
        OS_SAFETY_CRITICAL_EXCEPTION();
        return;
    }
 #endif

 #ifdef OS_SAFETY_CRITICAL_IEC61508
    if (OSSafetyCriticalStartFlag == DEF_TRUE) {
        *p_err = OS_ERR_ILLEGAL_CREATE_RUN_TIME;
        return;
    }
 #endif

 #if OS_CFG_CALLED_FROM_ISR_CHK_EN > 0u
    if (OSIntNestingCtr > (OS_NESTING_CTR)0) {              /* Not allowed to be called from an ISR                   */
        *p_err = OS_ERR_CREATE_ISR;
        return;
    }
 #endif

 #if OS_CFG_ARG_CHK_EN > 0u
    if (p_mutex == (OS_MUTEX *)0) {                         /* Validate 'p_mutex'                                     */
        *p_err = OS_ERR_OBJ_PTR_NULL;
        return;
    }
 #endif

    OS_CRITICAL_ENTER();
    p_mutex->Type              =  OS_OBJ_TYPE_MUTEX;        /* Mark the data structure as a mutex                     */
    p_mutex->NamePtr           =  p_name;
    p_mutex->OwnerTCBPtr       = (OS_TCB       *)0;
    p_mutex->OwnerNestingCtr   = (OS_NESTING_CTR)0;         /* Mutex is available                                     */
    p_mutex->TS                = (CPU_TS        )0;
    p_mutex->OwnerOriginalPrio =  OS_CFG_PRIO_MAX;
    OS_PendListInit(&p_mutex->PendList);                    /* Initialize the waiting list                            */

 #if OS_CFG_DBG_EN > 0u
    OS_MutexDbgListAdd(p_mutex);
 #endif
    OSMutexQty++;

    OS_CRITICAL_EXIT_NO_SCHED();
    *p_err = OS_ERR_NONE;
}

/*$PAGE*/
/*
 ************************************************************************************************************************
 *                                                   DELETE A MUTEX
 *
 * Description: This function deletes a mutex and readies all tasks pending on the mutex.
 *
 * Arguments  : p_mutex       is a pointer to the mutex to delete
 *
 *              opt           determines delete options as follows:
 *
 *                                OS_OPT_DEL_NO_PEND          Delete mutex ONLY if no task pending
 *                                OS_OPT_DEL_ALWAYS           Deletes the mutex even if tasks are waiting.
 *                                                            In this case, all the tasks pending will be readied.
 *
 *              p_err         is a pointer to a variable that will contain an error code returned by this function.
 *
 *                                OS_ERR_NONE                 The call was successful and the mutex was deleted
 *                                OS_ERR_DEL_ISR              If you attempted to delete the mutex from an ISR
 *                                OS_ERR_OBJ_PTR_NULL         If 'p_mutex' is a NULL pointer.
 *                                OS_ERR_OBJ_TYPE             If 'p_mutex' is not pointing to a mutex
 *                                OS_ERR_OPT_INVALID          An invalid option was specified
 *                                OS_ERR_STATE_INVALID        Task is in an invalid state
 *                                OS_ERR_TASK_WAITING         One or more tasks were waiting on the mutex
 *
 * Returns    : == 0          if no tasks were waiting on the mutex, or upon error.
 *              >  0          if one or more tasks waiting on the mutex are now readied and informed.
 *
 * Note(s)    : 1) This function must be used with care.  Tasks that would normally expect the presence of the mutex MUST
 *                 check the return code of OSMutexPend().
 *
 *              2) OSMutexAccept() callers will not know that the intended mutex has been deleted.
 *
 *              3) Because ALL tasks pending on the mutex will be readied, you MUST be careful in applications where the
 *                 mutex is used for mutual exclusion because the resource(s) will no longer be guarded by the mutex.
 ************************************************************************************************************************
 */

 #if OS_CFG_MUTEX_DEL_EN > 0u
OS_OBJ_QTY  OSMutexDel (OS_MUTEX *  p_mutex,
                        OS_OPT      opt,
                        OS_ERR *    p_err)
{
    OS_OBJ_QTY cnt;
    OS_OBJ_QTY nbr_tasks;
    OS_PEND_DATA  *p_pend_data;
    OS_PEND_LIST  *p_pend_list;
    OS_TCB        *p_tcb;
    OS_TCB        *p_tcb_owner;
    CPU_TS ts;
    CPU_SR_ALLOC();



  #ifdef OS_SAFETY_CRITICAL
    if (p_err == (OS_ERR *)0) {
        OS_SAFETY_CRITICAL_EXCEPTION();
        return ((OS_OBJ_QTY)0);
    }
  #endif

  #if OS_CFG_CALLED_FROM_ISR_CHK_EN > 0u
    if (OSIntNestingCtr > (OS_NESTING_CTR)0) {                  /* Not allowed to delete a mutex from an ISR          */
        *p_err = OS_ERR_DEL_ISR;
        return ((OS_OBJ_QTY)0);
    }
  #endif

  #if OS_CFG_ARG_CHK_EN > 0u
    if (p_mutex == (OS_MUTEX *)0) {                             /* Validate 'p_mutex'                                 */
        *p_err = OS_ERR_OBJ_PTR_NULL;
        return ((OS_OBJ_QTY)0);
    }
    switch (opt) {                                              /* Validate 'opt'                                     */
    case OS_OPT_DEL_NO_PEND:
    case OS_OPT_DEL_ALWAYS:
        break;

    default:
        *p_err =  OS_ERR_OPT_INVALID;
        return ((OS_OBJ_QTY)0);
    }
  #endif

  #if OS_CFG_OBJ_TYPE_CHK_EN > 0u
    if (p_mutex->Type != OS_OBJ_TYPE_MUTEX) {                   /* Make sure mutex was created                        */
        *p_err = OS_ERR_OBJ_TYPE;
        return ((OS_OBJ_QTY)0);
    }
  #endif

    OS_CRITICAL_ENTER();
    p_pend_list = &p_mutex->PendList;
    cnt         = p_pend_list->NbrEntries;
    nbr_tasks   = cnt;
    switch (opt) {
    case OS_OPT_DEL_NO_PEND:                                    /* Delete mutex only if no task waiting               */
        if (nbr_tasks == (OS_OBJ_QTY)0) {
  #if OS_CFG_DBG_EN > 0u
            OS_MutexDbgListRemove(p_mutex);
  #endif
            OSMutexQty--;
            OS_MutexClr(p_mutex);
            OS_CRITICAL_EXIT();
            *p_err = OS_ERR_NONE;
        } else {
            OS_CRITICAL_EXIT();
            *p_err = OS_ERR_TASK_WAITING;
        }
        break;

    case OS_OPT_DEL_ALWAYS:                                                /* Always delete the mutex                 */
        p_tcb_owner = p_mutex->OwnerTCBPtr;                                /* Did we had to change the prio of owner? */
        if ((p_tcb_owner       != (OS_TCB *)0) &&
            (p_tcb_owner->Prio !=  p_mutex->OwnerOriginalPrio)) {
            switch (p_tcb_owner->TaskState) {                              /* yes                                     */
            case OS_TASK_STATE_RDY:
                OS_RdyListRemove(p_tcb_owner);
                p_tcb_owner->Prio = p_mutex->OwnerOriginalPrio;            /* Lower owner's prio back                 */
                OS_PrioInsert(p_tcb_owner->Prio);
                OS_RdyListInsertTail(p_tcb_owner);                         /* Insert owner in ready list at new prio  */
                break;

            case OS_TASK_STATE_DLY:
            case OS_TASK_STATE_SUSPENDED:
            case OS_TASK_STATE_DLY_SUSPENDED:
                p_tcb_owner->Prio = p_mutex->OwnerOriginalPrio;            /* Not in any pend list, change the prio   */
                break;

            case OS_TASK_STATE_PEND:
            case OS_TASK_STATE_PEND_TIMEOUT:
            case OS_TASK_STATE_PEND_SUSPENDED:
            case OS_TASK_STATE_PEND_TIMEOUT_SUSPENDED:
                OS_PendListChangePrio(p_tcb_owner,                         /* Owner is pending on another object      */
                                      p_mutex->OwnerOriginalPrio);
                break;

            default:
                OS_CRITICAL_EXIT();
                *p_err = OS_ERR_STATE_INVALID;
                return ((OS_OBJ_QTY)0);
            }
        }

        ts = OS_TS_GET();                                                  /* Get timestamp                           */
        while (cnt > 0u) {                                                 /* Remove all tasks from the pend list     */
            p_pend_data = p_pend_list->HeadPtr;
            p_tcb       = p_pend_data->TCBPtr;
            OS_PendObjDel((OS_PEND_OBJ *)((void *)p_mutex),
                          p_tcb,
                          ts);
            cnt--;
        }
  #if OS_CFG_DBG_EN > 0u
        OS_MutexDbgListRemove(p_mutex);
  #endif
        OSMutexQty--;
        OS_MutexClr(p_mutex);
        OS_CRITICAL_EXIT_NO_SCHED();
        OSSched();                                                         /* Find highest priority task ready to run */
        *p_err = OS_ERR_NONE;
        break;

    default:
        OS_CRITICAL_EXIT();
        *p_err = OS_ERR_OPT_INVALID;
        break;
    }
    return (nbr_tasks);
}
 #endif

/*$PAGE*/
/*
 ************************************************************************************************************************
 *                                                    PEND ON MUTEX
 *
 * Description: This function waits for a mutex.
 *
 * Arguments  : p_mutex       is a pointer to the mutex
 *
 *              timeout       is an optional timeout period (in clock ticks).  If non-zero, your task will wait for the
 *                            resource up to the amount of time (in 'ticks') specified by this argument.  If you specify
 *                            0, however, your task will wait forever at the specified mutex or, until the resource
 *                            becomes available.
 *
 *              opt           determines whether the user wants to block if the mutex is not available or not:
 *
 *                                OS_OPT_PEND_BLOCKING
 *                                OS_OPT_PEND_NON_BLOCKING
 *
 *              p_ts          is a pointer to a variable that will receive the timestamp of when the mutex was posted or
 *                            pend aborted or the mutex deleted.  If you pass a NULL pointer (i.e. (CPU_TS *)0) then you
 *                            will not get the timestamp.  In other words, passing a NULL pointer is valid and indicates
 *                            that you don't need the timestamp.
 *
 *              p_err         is a pointer to a variable that will contain an error code returned by this function.
 *
 *                                OS_ERR_NONE               The call was successful and your task owns the resource
 *                                OS_ERR_MUTEX_OWNER        If calling task already owns the mutex
 *                                OS_ERR_OBJ_DEL            If 'p_mutex' was deleted
 *                                OS_ERR_OBJ_PTR_NULL       If 'p_mutex' is a NULL pointer.
 *                                OS_ERR_OBJ_TYPE           If 'p_mutex' is not pointing at a mutex
 *                                OS_ERR_OPT_INVALID        If you didn't specify a valid option
 *                                OS_ERR_PEND_ABORT         If the pend was aborted by another task
 *                                OS_ERR_PEND_ISR           If you called this function from an ISR and the result
 *                                                          would lead to a suspension.
 *                                OS_ERR_PEND_WOULD_BLOCK   If you specified non-blocking but the mutex was not
 *                                                          available.
 *                                OS_ERR_SCHED_LOCKED       If you called this function when the scheduler is locked
 *                                OS_ERR_STATE_INVALID      If the task is in an invalid state
 *                                OS_ERR_STATUS_INVALID     If the pend status has an invalid value
 *                                OS_ERR_TIMEOUT            The mutex was not received within the specified timeout.
 *
 * Returns    : none
 ************************************************************************************************************************
 */

void  OSMutexPend (OS_MUTEX *   p_mutex,
                   OS_TICK      timeout,
                   OS_OPT       opt,
                   CPU_TS *     p_ts,
                   OS_ERR *     p_err)
{
    OS_PEND_DATA pend_data;
    OS_TCB       *p_tcb;
    CPU_SR_ALLOC();



 #ifdef OS_SAFETY_CRITICAL
    if (p_err == (OS_ERR *)0) {
        OS_SAFETY_CRITICAL_EXCEPTION();
        return;
    }
 #endif

 #if OS_CFG_CALLED_FROM_ISR_CHK_EN > 0u
    if (OSIntNestingCtr > (OS_NESTING_CTR)0) {              /* Not allowed to call from an ISR                        */
        *p_err = OS_ERR_PEND_ISR;
        return;
    }
 #endif

 #if OS_CFG_ARG_CHK_EN > 0u
    if (p_mutex == (OS_MUTEX *)0) {                         /* Validate arguments                                     */
        *p_err = OS_ERR_OBJ_PTR_NULL;
        return;
    }
    switch (opt) {                                          /* Validate 'opt'                                         */
    case OS_OPT_PEND_BLOCKING:
    case OS_OPT_PEND_NON_BLOCKING:
        break;

    default:
        *p_err = OS_ERR_OPT_INVALID;
        return;
    }
 #endif

 #if OS_CFG_OBJ_TYPE_CHK_EN > 0u
    if (p_mutex->Type != OS_OBJ_TYPE_MUTEX) {               /* Make sure mutex was created                            */
        *p_err = OS_ERR_OBJ_TYPE;
        return;
    }
 #endif

    if (p_ts != (CPU_TS *)0) {
        *p_ts  = (CPU_TS  )0;                               /* Initialize the returned timestamp                      */
    }

    CPU_CRITICAL_ENTER();
    if (p_mutex->OwnerNestingCtr == (OS_NESTING_CTR)0) {    /* Resource available?                                    */
        p_mutex->OwnerTCBPtr       =  OSTCBCurPtr;          /* Yes, caller may proceed                                */
        p_mutex->OwnerOriginalPrio =  OSTCBCurPtr->Prio;
        p_mutex->OwnerNestingCtr   = (OS_NESTING_CTR)1;
        if (p_ts != (CPU_TS *)0) {
            *p_ts  = p_mutex->TS;
        }
        CPU_CRITICAL_EXIT();
        *p_err = OS_ERR_NONE;
        return;
    }

    if (OSTCBCurPtr == p_mutex->OwnerTCBPtr) {              /* See if current task is already the owner of the mutex  */
        p_mutex->OwnerNestingCtr++;
        if (p_ts != (CPU_TS *)0) {
            *p_ts  = p_mutex->TS;
        }
        CPU_CRITICAL_EXIT();
        *p_err = OS_ERR_MUTEX_OWNER;                        /* Indicate that current task already owns the mutex      */
        return;
    }

    if ((opt & OS_OPT_PEND_NON_BLOCKING) != (OS_OPT)0) {    /* Caller wants to block if not available?                */
        CPU_CRITICAL_EXIT();
        *p_err = OS_ERR_PEND_WOULD_BLOCK;                   /* No                                                     */
        return;
    } else {
        if (OSSchedLockNestingCtr > (OS_NESTING_CTR)0) {    /* Can't pend when the scheduler is locked                */
            CPU_CRITICAL_EXIT();
            *p_err = OS_ERR_SCHED_LOCKED;
            return;
        }
    }
    /* Lock the scheduler/re-enable interrupts                */
    OS_CRITICAL_ENTER_CPU_EXIT();
    p_tcb = p_mutex->OwnerTCBPtr;                           /* Point to the TCB of the Mutex owner                    */
    if (p_tcb->Prio > OSTCBCurPtr->Prio) {                  /* See if mutex owner has a lower priority than current   */
        switch (p_tcb->TaskState) {
        case OS_TASK_STATE_RDY:
            OS_RdyListRemove(p_tcb);                        /* Remove from ready list at current priority             */
            p_tcb->Prio = OSTCBCurPtr->Prio;                /* Raise owner's priority                                 */
            OS_PrioInsert(p_tcb->Prio);
            OS_RdyListInsertHead(p_tcb);                    /* Insert in ready list at new priority                   */
            break;

        case OS_TASK_STATE_DLY:
        case OS_TASK_STATE_DLY_SUSPENDED:
        case OS_TASK_STATE_SUSPENDED:
            p_tcb->Prio = OSTCBCurPtr->Prio;                /* Only need to raise the owner's priority                */
            break;

        case OS_TASK_STATE_PEND:                            /* Change the position of the task in the wait list       */
        case OS_TASK_STATE_PEND_TIMEOUT:
        case OS_TASK_STATE_PEND_SUSPENDED:
        case OS_TASK_STATE_PEND_TIMEOUT_SUSPENDED:
            OS_PendListChangePrio(p_tcb,
                                  OSTCBCurPtr->Prio);
            break;

        default:
            OS_CRITICAL_EXIT();
            *p_err = OS_ERR_STATE_INVALID;
            return;
        }
    }

    OS_Pend(&pend_data,                                     /* Block task pending on Mutex                            */
            (OS_PEND_OBJ *)((void *)p_mutex),
            OS_TASK_PEND_ON_MUTEX,
            timeout);

    OS_CRITICAL_EXIT_NO_SCHED();

    OSSched();                                              /* Find the next highest priority task ready to run       */

    CPU_CRITICAL_ENTER();
    switch (OSTCBCurPtr->PendStatus) {
    case OS_STATUS_PEND_OK:                                 /* We got the mutex                                       */
        if (p_ts != (CPU_TS *)0) {
            *p_ts  = OSTCBCurPtr->TS;
        }
        *p_err = OS_ERR_NONE;
        break;

    case OS_STATUS_PEND_ABORT:                              /* Indicate that we aborted                               */
        if (p_ts != (CPU_TS *)0) {
            *p_ts  = OSTCBCurPtr->TS;
        }
        *p_err = OS_ERR_PEND_ABORT;
        break;

    case OS_STATUS_PEND_TIMEOUT:                            /* Indicate that we didn't get mutex within timeout       */
        if (p_ts != (CPU_TS *)0) {
            *p_ts  = (CPU_TS  )0;
        }
        *p_err = OS_ERR_TIMEOUT;
        break;

    case OS_STATUS_PEND_DEL:                                /* Indicate that object pended on has been deleted        */
        if (p_ts != (CPU_TS *)0) {
            *p_ts  = OSTCBCurPtr->TS;
        }
        *p_err = OS_ERR_OBJ_DEL;
        break;

    default:
        *p_err = OS_ERR_STATUS_INVALID;
        break;
    }
    CPU_CRITICAL_EXIT();
}

/*$PAGE*/
/*
 ************************************************************************************************************************
 *                                               ABORT WAITING ON A MUTEX
 *
 * Description: This function aborts & readies any tasks currently waiting on a mutex.  This function should be used
 *              to fault-abort the wait on the mutex, rather than to normally signal the mutex via OSMutexPost().
 *
 * Arguments  : p_mutex   is a pointer to the mutex
 *
 *              opt       determines the type of ABORT performed:
 *
 *                            OS_OPT_PEND_ABORT_1          ABORT wait for a single task (HPT) waiting on the mutex
 *                            OS_OPT_PEND_ABORT_ALL        ABORT wait for ALL tasks that are  waiting on the mutex
 *                            OS_OPT_POST_NO_SCHED         Do not call the scheduler
 *
 *              p_err     is a pointer to a variable that will contain an error code returned by this function.
 *
 *                            OS_ERR_NONE                  At least one task waiting on the mutex was readied and
 *                                                         informed of the aborted wait; check return value for the
 *                                                         number of tasks whose wait on the mutex was aborted.
 *                            OS_ERR_OBJ_PTR_NULL          If 'p_mutex' is a NULL pointer.
 *                            OS_ERR_OBJ_TYPE              If 'p_mutex' is not pointing at a mutex
 *                            OS_ERR_OPT_INVALID           If you specified an invalid option
 *                            OS_ERR_PEND_ABORT_ISR        If you attempted to call this function from an ISR
 *                            OS_ERR_PEND_ABORT_NONE       No task were pending
 *
 * Returns    : == 0          if no tasks were waiting on the mutex, or upon error.
 *              >  0          if one or more tasks waiting on the mutex are now readied and informed.
 ************************************************************************************************************************
 */

 #if OS_CFG_MUTEX_PEND_ABORT_EN > 0u
OS_OBJ_QTY  OSMutexPendAbort (OS_MUTEX *p_mutex,
                              OS_OPT    opt,
                              OS_ERR *  p_err)
{
    OS_PEND_LIST  *p_pend_list;
    OS_TCB        *p_tcb;
    CPU_TS ts;
    OS_OBJ_QTY nbr_tasks;
    CPU_SR_ALLOC();



  #ifdef OS_SAFETY_CRITICAL
    if (p_err == (OS_ERR *)0) {
        OS_SAFETY_CRITICAL_EXCEPTION();
        return ((OS_OBJ_QTY)0u);
    }
  #endif

  #if OS_CFG_CALLED_FROM_ISR_CHK_EN > 0u
    if (OSIntNestingCtr > (OS_NESTING_CTR)0u) {             /* Not allowed to Pend Abort from an ISR                  */
        *p_err =  OS_ERR_PEND_ABORT_ISR;
        return ((OS_OBJ_QTY)0u);
    }
  #endif

  #if OS_CFG_ARG_CHK_EN > 0u
    if (p_mutex == (OS_MUTEX *)0) {                         /* Validate 'p_mutex'                                     */
        *p_err =  OS_ERR_OBJ_PTR_NULL;
        return ((OS_OBJ_QTY)0u);
    }
    switch (opt) {                                          /* Validate 'opt'                                         */
    case OS_OPT_PEND_ABORT_1:
    case OS_OPT_PEND_ABORT_ALL:
    case OS_OPT_PEND_ABORT_1   | OS_OPT_POST_NO_SCHED:
    case OS_OPT_PEND_ABORT_ALL | OS_OPT_POST_NO_SCHED:
        break;

    default:
        *p_err =  OS_ERR_OPT_INVALID;
        return ((OS_OBJ_QTY)0u);
    }
  #endif

  #if OS_CFG_OBJ_TYPE_CHK_EN > 0u
    if (p_mutex->Type != OS_OBJ_TYPE_MUTEX) {               /* Make sure mutex was created                            */
        *p_err =  OS_ERR_OBJ_TYPE;
        return ((OS_OBJ_QTY)0u);
    }
  #endif

    CPU_CRITICAL_ENTER();
    p_pend_list = &p_mutex->PendList;
    if (p_pend_list->NbrEntries == (OS_OBJ_QTY)0u) {        /* Any task waiting on mutex?                             */
        CPU_CRITICAL_EXIT();                                /* No                                                     */
        *p_err =  OS_ERR_PEND_ABORT_NONE;
        return ((OS_OBJ_QTY)0u);
    }

    OS_CRITICAL_ENTER_CPU_EXIT();
    nbr_tasks = 0u;
    ts        = OS_TS_GET();                                /* Get local time stamp so all tasks get the same time    */
    while (p_pend_list->NbrEntries > (OS_OBJ_QTY)0u) {
        p_tcb = p_pend_list->HeadPtr->TCBPtr;
        OS_PendAbort((OS_PEND_OBJ *)((void *)p_mutex),
                     p_tcb,
                     ts);
        nbr_tasks++;
        if (opt != OS_OPT_PEND_ABORT_ALL) {                 /* Pend abort all tasks waiting?                          */
            break;                                          /* No                                                     */
        }
    }
    OS_CRITICAL_EXIT_NO_SCHED();

    if ((opt & OS_OPT_POST_NO_SCHED) == (OS_OPT)0u) {
        OSSched();                                          /* Run the scheduler                                      */
    }

    *p_err = OS_ERR_NONE;
    return (nbr_tasks);
}
 #endif

/*$PAGE*/
/*
 ************************************************************************************************************************
 *                                                   POST TO A MUTEX
 *
 * Description: This function signals a mutex
 *
 * Arguments  : p_mutex  is a pointer to the mutex
 *
 *              opt      is an option you can specify to alter the behavior of the post.  The choices are:
 *
 *                           OS_OPT_POST_NONE        No special option selected
 *                           OS_OPT_POST_NO_SCHED    If you don't want the scheduler to be called after the post.
 *
 *              p_err    is a pointer to a variable that will contain an error code returned by this function.
 *
 *                           OS_ERR_NONE             The call was successful and the mutex was signaled.
 *                           OS_ERR_MUTEX_NESTING    Mutex owner nested its use of the mutex
 *                           OS_ERR_MUTEX_NOT_OWNER  If the task posting is not the Mutex owner
 *                           OS_ERR_OBJ_PTR_NULL     If 'p_mutex' is a NULL pointer.
 *                           OS_ERR_OBJ_TYPE         If 'p_mutex' is not pointing at a mutex
 *                           OS_ERR_POST_ISR         If you attempted to post from an ISR
 *
 * Returns    : none
 ************************************************************************************************************************
 */

void  OSMutexPost (OS_MUTEX *   p_mutex,
                   OS_OPT       opt,
                   OS_ERR *     p_err)
{
    OS_PEND_LIST  *p_pend_list;
    OS_TCB        *p_tcb;
    CPU_TS ts;
    CPU_SR_ALLOC();



 #ifdef OS_SAFETY_CRITICAL
    if (p_err == (OS_ERR *)0) {
        OS_SAFETY_CRITICAL_EXCEPTION();
        return;
    }
 #endif

 #if OS_CFG_CALLED_FROM_ISR_CHK_EN > 0u
    if (OSIntNestingCtr > (OS_NESTING_CTR)0) {              /* Not allowed to call from an ISR                        */
        *p_err = OS_ERR_POST_ISR;
        return;
    }
 #endif

 #if OS_CFG_ARG_CHK_EN > 0u
    if (p_mutex == (OS_MUTEX *)0) {                         /* Validate 'p_mutex'                                     */
        *p_err = OS_ERR_OBJ_PTR_NULL;
        return;
    }
    switch (opt) {                                          /* Validate 'opt'                                         */
    case OS_OPT_POST_NONE:
    case OS_OPT_POST_NO_SCHED:
        break;

    default:
        *p_err =  OS_ERR_OPT_INVALID;
        return;
    }
 #endif

 #if OS_CFG_OBJ_TYPE_CHK_EN > 0u
    if (p_mutex->Type != OS_OBJ_TYPE_MUTEX) {               /* Make sure mutex was created                            */
        *p_err = OS_ERR_OBJ_TYPE;
        return;
    }
 #endif

    CPU_CRITICAL_ENTER();
    if (OSTCBCurPtr != p_mutex->OwnerTCBPtr) {              /* Make sure the mutex owner is releasing the mutex       */
        CPU_CRITICAL_EXIT();
        *p_err = OS_ERR_MUTEX_NOT_OWNER;
        return;
    }

    OS_CRITICAL_ENTER_CPU_EXIT();
    ts          = OS_TS_GET();                              /* Get timestamp                                          */
    p_mutex->TS = ts;
    p_mutex->OwnerNestingCtr--;                             /* Decrement owner's nesting counter                      */
    if (p_mutex->OwnerNestingCtr > (OS_NESTING_CTR)0) {     /* Are we done with all nestings?                         */
        OS_CRITICAL_EXIT();                                 /* No                                                     */
        *p_err = OS_ERR_MUTEX_NESTING;
        return;
    }

    p_pend_list = &p_mutex->PendList;
    if (p_pend_list->NbrEntries == (OS_OBJ_QTY)0) {         /* Any task waiting on mutex?                             */
        p_mutex->OwnerTCBPtr     = (OS_TCB       *)0;       /* No                                                     */
        p_mutex->OwnerNestingCtr = (OS_NESTING_CTR)0;
        OS_CRITICAL_EXIT();
        *p_err = OS_ERR_NONE;
        return;
    }
    /* Yes                                                    */
    if (OSTCBCurPtr->Prio != p_mutex->OwnerOriginalPrio) {
        OS_RdyListRemove(OSTCBCurPtr);
        OSTCBCurPtr->Prio = p_mutex->OwnerOriginalPrio;     /* Lower owner's priority back to its original one        */
        OS_PrioInsert(OSTCBCurPtr->Prio);
        OS_RdyListInsertTail(OSTCBCurPtr);                  /* Insert owner in ready list at new priority             */
        OSPrioCur         = OSTCBCurPtr->Prio;
    }
    /* Get TCB from head of pend list                         */
    p_tcb                      = p_pend_list->HeadPtr->TCBPtr;
    p_mutex->OwnerTCBPtr       = p_tcb;                     /* Give mutex to new owner                                */
    p_mutex->OwnerOriginalPrio = p_tcb->Prio;
    p_mutex->OwnerNestingCtr   = (OS_NESTING_CTR)1;
    /* Post to mutex                                          */
    OS_Post((OS_PEND_OBJ *)((void *)p_mutex),
            (OS_TCB      *)p_tcb,
            (void        *)0,
            (OS_MSG_SIZE  )0,
            (CPU_TS       )ts);

    OS_CRITICAL_EXIT_NO_SCHED();

    if ((opt & OS_OPT_POST_NO_SCHED) == (OS_OPT)0) {
        OSSched();                                          /* Run the scheduler                                      */
    }

    *p_err = OS_ERR_NONE;
}

/*$PAGE*/
/*
 ************************************************************************************************************************
 *                                            CLEAR THE CONTENTS OF A MUTEX
 *
 * Description: This function is called by OSMutexDel() to clear the contents of a mutex
 *
 *
 * Argument(s): p_mutex      is a pointer to the mutex to clear
 *              -------
 *
 * Returns    : none
 *
 * Note(s)    : This function is INTERNAL to uC/OS-III and your application should not call it.
 ************************************************************************************************************************
 */

void  OS_MutexClr (OS_MUTEX *p_mutex)
{
    p_mutex->Type              =  OS_OBJ_TYPE_NONE;         /* Mark the data structure as a NONE                      */
    p_mutex->NamePtr           = (CPU_CHAR     *)((void *)"?MUTEX");
    p_mutex->OwnerTCBPtr       = (OS_TCB       *)0;
    p_mutex->OwnerNestingCtr   = (OS_NESTING_CTR)0;
    p_mutex->TS                = (CPU_TS        )0;
    p_mutex->OwnerOriginalPrio =  OS_CFG_PRIO_MAX;
    OS_PendListInit(&p_mutex->PendList);                    /* Initialize the waiting list                            */
}

/*$PAGE*/
/*
 ************************************************************************************************************************
 *                                          ADD/REMOVE MUTEX TO/FROM DEBUG LIST
 *
 * Description: These functions are called by uC/OS-III to add or remove a mutex to/from the debug list.
 *
 * Arguments  : p_mutex     is a pointer to the mutex to add/remove
 *
 * Returns    : none
 *
 * Note(s)    : These functions are INTERNAL to uC/OS-III and your application should not call it.
 ************************************************************************************************************************
 */


 #if OS_CFG_DBG_EN > 0u
void  OS_MutexDbgListAdd (OS_MUTEX *p_mutex)
{
    p_mutex->DbgNamePtr               = (CPU_CHAR *)((void *)" ");
    p_mutex->DbgPrevPtr               = (OS_MUTEX *)0;
    if (OSMutexDbgListPtr == (OS_MUTEX *)0) {
        p_mutex->DbgNextPtr           = (OS_MUTEX *)0;
    } else {
        p_mutex->DbgNextPtr           =  OSMutexDbgListPtr;
        OSMutexDbgListPtr->DbgPrevPtr =  p_mutex;
    }
    OSMutexDbgListPtr                 =  p_mutex;
}



void  OS_MutexDbgListRemove (OS_MUTEX *p_mutex)
{
    OS_MUTEX  *p_mutex_next;
    OS_MUTEX  *p_mutex_prev;


    p_mutex_prev = p_mutex->DbgPrevPtr;
    p_mutex_next = p_mutex->DbgNextPtr;

    if (p_mutex_prev == (OS_MUTEX *)0) {
        OSMutexDbgListPtr = p_mutex_next;
        if (p_mutex_next != (OS_MUTEX *)0) {
            p_mutex_next->DbgPrevPtr = (OS_MUTEX *)0;
        }
        p_mutex->DbgNextPtr = (OS_MUTEX *)0;

    } else if (p_mutex_next == (OS_MUTEX *)0) {
        p_mutex_prev->DbgNextPtr = (OS_MUTEX *)0;
        p_mutex->DbgPrevPtr      = (OS_MUTEX *)0;

    } else {
        p_mutex_prev->DbgNextPtr =  p_mutex_next;
        p_mutex_next->DbgPrevPtr =  p_mutex_prev;
        p_mutex->DbgNextPtr      = (OS_MUTEX *)0;
        p_mutex->DbgPrevPtr      = (OS_MUTEX *)0;
    }
}
 #endif

/*$PAGE*/
/*
 ************************************************************************************************************************
 *                                                MUTEX INITIALIZATION
 *
 * Description: This function is called by OSInit() to initialize the mutex management.
 *
 *
 * Argument(s): p_err        is a pointer to a variable that will contain an error code returned by this function.
 *
 *                                OS_ERR_NONE     the call was successful
 *
 * Returns    : none
 *
 * Note(s)    : 1) This function is INTERNAL to uC/OS-III and your application MUST NOT call it.
 ************************************************************************************************************************
 */

void  OS_MutexInit (OS_ERR *p_err)
{
 #ifdef OS_SAFETY_CRITICAL
    if (p_err == (OS_ERR *)0) {
        OS_SAFETY_CRITICAL_EXCEPTION();
        return;
    }
 #endif

 #if OS_CFG_DBG_EN > 0u
    OSMutexDbgListPtr = (OS_MUTEX *)0;
 #endif

    OSMutexQty        = (OS_OBJ_QTY)0;
    *p_err             =  OS_ERR_NONE;
}

#endif                                                      /* OS_CFG_MUTEX_EN                                        */
