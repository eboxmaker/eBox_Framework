/*
 ************************************************************************************************************************
 *                                                      uC/OS-III
 *                                                 The Real-Time Kernel
 *
 *                                  (c) Copyright 2009-2012; Micrium, Inc.; Weston, FL
 *                           All rights reserved.  Protected by international copyright laws.
 *
 *                                             MEMORY PARTITION MANAGEMENT
 *
 * File    : OS_MEM.C
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

#define   MICRIUM_SOURCE
#include  <os.h>

#ifdef VSC_INCLUDE_SOURCE_FILE_NAMES
const CPU_CHAR  *os_mem__c = "$Id: $";
#endif


#if OS_CFG_MEM_EN > 0u
/*
 ************************************************************************************************************************
 *                                               CREATE A MEMORY PARTITION
 *
 * Description : Create a fixed-sized memory partition that will be managed by uC/OS-III.
 *
 * Arguments   : p_mem    is a pointer to a memory partition control block which is allocated in user memory space.
 *
 *               p_name   is a pointer to an ASCII string to provide a name to the memory partition.
 *
 *               p_addr   is the starting address of the memory partition
 *
 *               n_blks   is the number of memory blocks to create from the partition.
 *
 *               blk_size is the size (in bytes) of each block in the memory partition.
 *
 *               p_err    is a pointer to a variable containing an error message which will be set by this function to
 *                        either:
 *
 *                            OS_ERR_NONE                    if the memory partition has been created correctly.
 *                            OS_ERR_ILLEGAL_CREATE_RUN_TIME if you are trying to create the memory partition after you
 *                                                             called OSSafetyCriticalStart().
 *                            OS_ERR_MEM_INVALID_BLKS        user specified an invalid number of blocks (must be >= 2)
 *                            OS_ERR_MEM_INVALID_P_ADDR      if you are specifying an invalid address for the memory
 *                                                           storage of the partition or, the block does not align on a
 *                                                           pointer boundary
 *                            OS_ERR_MEM_INVALID_SIZE        user specified an invalid block size
 *                                                             - must be greater than the size of a pointer
 *                                                             - must be able to hold an integral number of pointers
 * Returns    : none
 ************************************************************************************************************************
 */

void  OSMemCreate (OS_MEM *     p_mem,
                   CPU_CHAR *   p_name,
                   void *       p_addr,
                   OS_MEM_QTY   n_blks,
                   OS_MEM_SIZE  blk_size,
                   OS_ERR *     p_err)
{
 #if OS_CFG_ARG_CHK_EN > 0u
    CPU_DATA align_msk;
 #endif
    OS_MEM_QTY i;
    OS_MEM_QTY loops;
    CPU_INT08U    *p_blk;
    void         **p_link;
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
    if (OSIntNestingCtr > (OS_NESTING_CTR)0) {              /* Not allowed to call from an ISR                        */
        *p_err = OS_ERR_MEM_CREATE_ISR;
        return;
    }
 #endif

 #if OS_CFG_ARG_CHK_EN > 0u
    if (p_addr == (void *)0) {                              /* Must pass a valid address for the memory part.         */
        *p_err   = OS_ERR_MEM_INVALID_P_ADDR;
        return;
    }
    if (n_blks < (OS_MEM_QTY)2) {                           /* Must have at least 2 blocks per partition              */
        *p_err = OS_ERR_MEM_INVALID_BLKS;
        return;
    }
    if (blk_size < sizeof(void *)) {                        /* Must contain space for at least a pointer              */
        *p_err = OS_ERR_MEM_INVALID_SIZE;
        return;
    }
    align_msk = sizeof(void *) - 1u;
    if (align_msk > 0u) {
        if (((CPU_ADDR)p_addr & align_msk) != 0u) {          /* Must be pointer size aligned                           */
            *p_err = OS_ERR_MEM_INVALID_P_ADDR;
            return;
        }
        if ((blk_size & align_msk) != 0u) {                 /* Block size must be a multiple address size             */
            *p_err = OS_ERR_MEM_INVALID_SIZE;
            return;
        }
    }
 #endif

    p_link = (void **)p_addr;                               /* Create linked list of free memory blocks               */
    p_blk  = (CPU_INT08U *)p_addr;
    loops  = n_blks - 1u;
    for (i = 0u; i < loops; i++) {
        p_blk +=  blk_size;
        *p_link = (void  *)p_blk;                           /* Save pointer to NEXT block in CURRENT block            */
        p_link = (void **)(void *)p_blk;                    /* Position     to NEXT block                             */
    }
    *p_link             = (void *)0;                        /* Last memory block points to NULL                       */

    OS_CRITICAL_ENTER();
    p_mem->Type        = OS_OBJ_TYPE_MEM;                   /* Set the type of object                                 */
    p_mem->NamePtr     = p_name;                            /* Save name of memory partition                          */
    p_mem->AddrPtr     = p_addr;                            /* Store start address of memory partition                */
    p_mem->FreeListPtr = p_addr;                            /* Initialize pointer to pool of free blocks              */
    p_mem->NbrFree     = n_blks;                            /* Store number of free blocks in MCB                     */
    p_mem->NbrMax      = n_blks;
    p_mem->BlkSize     = blk_size;                          /* Store block size of each memory blocks                 */

 #if OS_CFG_DBG_EN > 0u
    OS_MemDbgListAdd(p_mem);
 #endif

    OSMemQty++;

    OS_CRITICAL_EXIT_NO_SCHED();
    *p_err = OS_ERR_NONE;
}

/*$PAGE*/
/*
 ************************************************************************************************************************
 *                                                  GET A MEMORY BLOCK
 *
 * Description : Get a memory block from a partition
 *
 * Arguments   : p_mem   is a pointer to the memory partition control block
 *
 *               p_err   is a pointer to a variable containing an error message which will be set by this function to
 *                       either:
 *
 *                       OS_ERR_NONE               if the memory partition has been created correctly.
 *                       OS_ERR_MEM_INVALID_P_MEM  if you passed a NULL pointer for 'p_mem'
 *                       OS_ERR_MEM_NO_FREE_BLKS   if there are no more free memory blocks to allocate to the caller
 *
 * Returns     : A pointer to a memory block if no error is detected
 *               A pointer to NULL if an error is detected
 ************************************************************************************************************************
 */

void  *OSMemGet (OS_MEM *   p_mem,
                 OS_ERR *   p_err)
{
    void    *p_blk;
    CPU_SR_ALLOC();



 #ifdef OS_SAFETY_CRITICAL
    if (p_err == (OS_ERR *)0) {
        OS_SAFETY_CRITICAL_EXCEPTION();
        return ((void *)0);
    }
 #endif

 #if OS_CFG_ARG_CHK_EN > 0u
    if (p_mem == (OS_MEM *)0) {                             /* Must point to a valid memory partition                 */
        *p_err  = OS_ERR_MEM_INVALID_P_MEM;
        return ((void *)0);
    }
 #endif

    CPU_CRITICAL_ENTER();
    if (p_mem->NbrFree == (OS_MEM_QTY)0) {                  /* See if there are any free memory blocks                */
        CPU_CRITICAL_EXIT();
        *p_err = OS_ERR_MEM_NO_FREE_BLKS;                   /* No,  Notify caller of empty memory partition           */
        return ((void *)0);                                 /*      Return NULL pointer to caller                     */
    }
    p_blk              = p_mem->FreeListPtr;                /* Yes, point to next free memory block                   */
    p_mem->FreeListPtr = *(void **)p_blk;                   /*      Adjust pointer to new free list                   */
    p_mem->NbrFree--;                                       /*      One less memory block in this partition           */
    CPU_CRITICAL_EXIT();
    *p_err = OS_ERR_NONE;                                   /*      No error                                          */
    return (p_blk);                                         /*      Return memory block to caller                     */
}

/*$PAGE*/
/*
 ************************************************************************************************************************
 *                                                 RELEASE A MEMORY BLOCK
 *
 * Description : Returns a memory block to a partition
 *
 * Arguments   : p_mem    is a pointer to the memory partition control block
 *
 *               p_blk    is a pointer to the memory block being released.
 *
 *               p_err    is a pointer to a variable that will contain an error code returned by this function.
 *
 *                            OS_ERR_NONE               if the memory block was inserted into the partition
 *                            OS_ERR_MEM_FULL           if you are returning a memory block to an already FULL memory
 *                                                      partition (You freed more blocks than you allocated!)
 *                            OS_ERR_MEM_INVALID_P_BLK  if you passed a NULL pointer for the block to release.
 *                            OS_ERR_MEM_INVALID_P_MEM  if you passed a NULL pointer for 'p_mem'
 ************************************************************************************************************************
 */

void  OSMemPut (OS_MEM *p_mem,
                void *  p_blk,
                OS_ERR *p_err)
{
    CPU_SR_ALLOC();



 #ifdef OS_SAFETY_CRITICAL
    if (p_err == (OS_ERR *)0) {
        OS_SAFETY_CRITICAL_EXCEPTION();
        return;
    }
 #endif

 #if OS_CFG_ARG_CHK_EN > 0u
    if (p_mem == (OS_MEM *)0) {                             /* Must point to a valid memory partition                 */
        *p_err  = OS_ERR_MEM_INVALID_P_MEM;
        return;
    }
    if (p_blk == (void *)0) {                               /* Must release a valid block                             */
        *p_err  = OS_ERR_MEM_INVALID_P_BLK;
        return;
    }
 #endif

    CPU_CRITICAL_ENTER();
    if (p_mem->NbrFree >= p_mem->NbrMax) {                  /* Make sure all blocks not already returned              */
        CPU_CRITICAL_EXIT();
        *p_err = OS_ERR_MEM_FULL;
        return;
    }
    *(void **)p_blk    = p_mem->FreeListPtr;                /* Insert released block into free block list             */
    p_mem->FreeListPtr = p_blk;
    p_mem->NbrFree++;                                       /* One more memory block in this partition                */
    CPU_CRITICAL_EXIT();
    *p_err              = OS_ERR_NONE;                      /* Notify caller that memory block was released           */
}

/*$PAGE*/
/*
 ************************************************************************************************************************
 *                                           ADD MEMORY PARTITION TO DEBUG LIST
 *
 * Description : This function is called by OSMemCreate() to add the memory partition to the debug table.
 *
 * Arguments   : p_mem    Is a pointer to the memory partition
 *
 * Returns     : none
 *
 * Note(s)    : This function is INTERNAL to uC/OS-III and your application should not call it.
 ************************************************************************************************************************
 */

 #if OS_CFG_DBG_EN > 0u
void  OS_MemDbgListAdd (OS_MEM *p_mem)
{
    p_mem->DbgPrevPtr               = (OS_MEM *)0;
    if (OSMemDbgListPtr == (OS_MEM *)0) {
        p_mem->DbgNextPtr           = (OS_MEM *)0;
    } else {
        p_mem->DbgNextPtr           =  OSMemDbgListPtr;
        OSMemDbgListPtr->DbgPrevPtr =  p_mem;
    }
    OSMemDbgListPtr                 =  p_mem;
}
 #endif

/*$PAGE*/
/*
 ************************************************************************************************************************
 *                                           INITIALIZE MEMORY PARTITION MANAGER
 *
 * Description : This function is called by uC/OS-III to initialize the memory partition manager.  Your
 *               application MUST NOT call this function.
 *
 * Arguments   : none
 *
 * Returns     : none
 *
 * Note(s)    : This function is INTERNAL to uC/OS-III and your application should not call it.
 ************************************************************************************************************************
 */

void  OS_MemInit (OS_ERR *p_err)
{
 #ifdef OS_SAFETY_CRITICAL
    if (p_err == (OS_ERR *)0) {
        OS_SAFETY_CRITICAL_EXCEPTION();
        return;
    }
 #endif

 #if OS_CFG_DBG_EN > 0u
    OSMemDbgListPtr = (OS_MEM   *)0;
 #endif

    OSMemQty        = (OS_OBJ_QTY)0;
    *p_err           = OS_ERR_NONE;
}
#endif
