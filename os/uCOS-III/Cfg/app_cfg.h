/*
 *********************************************************************************************************
 *                                              EXAMPLE CODE
 *
 *                           (c) Copyright 2009-2013; Micrium, Inc.; Weston, FL
 *
 *               All rights reserved.  Protected by international copyright laws.
 *               Knowledge of the source code may NOT be used to develop a similar product.
 *               Please help us continue to provide the Embedded community with the finest
 *               software available.  Your honesty is greatly appreciated.
 *********************************************************************************************************
 */

/*
 *********************************************************************************************************
 *                                      APPLICATION CONFIGURATION
 *
 *                                     ST Microelectronics STM32
 *                                              on the
 *
 *                                     Micrium uC-Eval-STM32F107
 *                                         Evaluation Board
 *
 * Filename      : app_cfg.h
 * Version       : V1.00
 * Programmer(s) : JJL
 *                 EHS
 *                 DC
 *********************************************************************************************************
 */

#ifndef  __APP_CFG_H__
#define  __APP_CFG_H__


/*
 *********************************************************************************************************
 *                                       MODULE ENABLE / DISABLE
 *********************************************************************************************************
 */

#define STARTUP_TASK_STK_SIZE   256


#define  APP_CFG_SERIAL_EN                          DEF_ENABLED

/*
 *********************************************************************************************************
 *                                            TASK PRIORITIES
 *********************************************************************************************************
 */

#define  APP_TASK_START_PRIO                        2


/*
 *********************************************************************************************************
 *                                            TASK STACK SIZES
 *                             Size of the task stacks (# of OS_STK entries)
 *********************************************************************************************************
 */

#define  APP_TASK_START_STK_SIZE                    128


/*
 *********************************************************************************************************
 *                                    BSP CONFIGURATION: RS-232
 *********************************************************************************************************
 */

#define  BSP_CFG_SER_COMM_SEL                                   BSP_SER_COMM_UART_02
#define  BSP_CFG_TS_TMR_SEL                         2


/*
 *********************************************************************************************************
 *                                     TRACE / DEBUG CONFIGURATION
 *********************************************************************************************************
 */
#if 0
 #define  TRACE_LEVEL_OFF                            0
 #define  TRACE_LEVEL_INFO                           1
 #define  TRACE_LEVEL_DEBUG                          2
#endif

#define  APP_TRACE_LEVEL                            TRACE_LEVEL_INFO
#define  APP_TRACE                                  BSP_Ser_Printf

#define  APP_TRACE_INFO(x)            ((APP_TRACE_LEVEL >= TRACE_LEVEL_INFO)  ? (void)(APP_TRACE x) : (void)0)
#define  APP_TRACE_DEBUG(x)           ((APP_TRACE_LEVEL >= TRACE_LEVEL_DEBUG) ? (void)(APP_TRACE x) : (void)0)


#endif
