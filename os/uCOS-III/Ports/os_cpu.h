/*
 *********************************************************************************************************
 *                                                uC/OS-III
 *                                          The Real-Time Kernel
 *
 *
 *                           (c) Copyright 2009-2010; Micrium, Inc.; Weston, FL
 *                    All rights reserved.  Protected by international copyright laws.
 *
 *                                           ARM Cortex-M3 Port
 *
 * File      : OS_CPU.H
 * Version   : V3.01.2
 * By        : JJL
 *
 * LICENSING TERMS:
 * ---------------
 *             uC/OS-III is provided in source form to registered licensees ONLY.  It is
 *             illegal to distribute this source code to any third party unless you receive
 *             written permission by an authorized Micrium representative.  Knowledge of
 *             the source code may NOT be used to develop a similar product.
 *
 *             Please help us continue to provide the Embedded community with the finest
 *             software available.  Your honesty is greatly appreciated.
 *
 *             You can contact us at www.micrium.com.
 *
 * For       : ARMv7M Cortex-M3
 * Mode      : Thumb2
 * Toolchain : RealView
 *********************************************************************************************************
 */

#ifndef  OS_CPU_H
#define  OS_CPU_H

#ifdef   OS_CPU_GLOBALS
 #define  OS_CPU_EXT
#else
 #define  OS_CPU_EXT  extern
#endif

/*
 *********************************************************************************************************
 *                                               MACROS
 *********************************************************************************************************
 */

#ifndef  NVIC_INT_CTRL
 #define  NVIC_INT_CTRL                      *((CPU_REG32 *)0xE000ED04)
#endif

#ifndef  NVIC_PENDSVSET
 #define  NVIC_PENDSVSET                                    0x10000000
#endif

#define  OS_TASK_SW()               NVIC_INT_CTRL = NVIC_PENDSVSET
#define  OSIntCtxSw()               NVIC_INT_CTRL = NVIC_PENDSVSET


/*
 *********************************************************************************************************
 *                                       TIMESTAMP CONFIGURATION
 *
 * Note(s) : (1) OS_TS_GET() is generally defined as CPU_TS_Get32() to allow CPU timestamp timer to be of
 *               any data type size.
 *
 *           (2) For architectures that provide 32-bit or higher precision free running counters
 *               (i.e. cycle count registers):
 *
 *               (a) OS_TS_GET() may be defined as CPU_TS_TmrRd() to improve performance when retrieving
 *                   the timestamp.
 *
 *               (b) CPU_TS_TmrRd() MUST be configured to be greater or equal to 32-bits to avoid
 *                   truncation of TS.
 *********************************************************************************************************
 */

#if      OS_CFG_TS_EN == 1u
 #define  OS_TS_GET()               (CPU_TS)CPU_TS_TmrRd()  /* See Note #2a.                                          */
#else
 #define  OS_TS_GET()               (CPU_TS)0u
#endif

#if (CPU_CFG_TS_32_EN    == DEF_ENABLED) && \
    (CPU_CFG_TS_TMR_SIZE  < CPU_WORD_SIZE_32)
/* CPU_CFG_TS_TMR_SIZE MUST be >= 32-bit (see Note #2b).  */
 #error  "cpu_cfg.h, CPU_CFG_TS_TMR_SIZE MUST be >= CPU_WORD_SIZE_32"
#endif


/*
 *********************************************************************************************************
 *                              OS TICK INTERRUPT PRIORITY CONFIGURATION
 *
 * Note(s) : (1) For systems that don't need any high, real-time priority interrupts; the tick interrupt
 *               should be configured as the highest priority interrupt but won't adversely affect system
 *               operations.
 *
 *           (2) For systems that need one or more high, real-time interrupts; these should be configured
 *               higher than the tick interrupt which MAY delay execution of the tick interrupt.
 *
 *               (a) If the higher priority interrupts do NOT continually consume CPU cycles but only
 *                   occasionally delay tick interrupts, then the real-time interrupts can successfully
 *                   handle their intermittent/periodic events with the system not losing tick interrupts
 *                   but only increasing the jitter.
 *
 *               (b) If the higher priority interrupts consume enough CPU cycles to continually delay the
 *                   tick interrupt, then the CPU/system is most likely over-burdened & can't be expected
 *                   to handle all its interrupts/tasks. The system time reference gets compromised as a
 *                   result of losing tick interrupts.
 *********************************************************************************************************
 */

#define  OS_CPU_CFG_SYSTICK_PRIO           0u

/*
 *********************************************************************************************************
 *                                          GLOBAL VARIABLES
 *********************************************************************************************************
 */

OS_CPU_EXT CPU_STK *      OS_CPU_ExceptStkBase;

/*
 *********************************************************************************************************
 *                                         FUNCTION PROTOTYPES
 *********************************************************************************************************
 */

void        OSStartHighRdy       (void);

void        PendSV_Handler (void);


void        OS_CPU_SysTickHandler(void);
void        OS_CPU_SysTickInit   (CPU_INT32U cnts);

#endif
