/**
  ******************************************************************************
  * @file    ebox_config.h
  * @author  cat_li
  * @version V2.0
  * @date    2016/10/21
  * @brief   ebox配置裁剪文件
  ******************************************************************************
  * @attention
  *
  * No part of this software may be used for any commercial activities by any form
  * or means, without the prior written consent of shentq. This specification is
  * preliminary and is subject to change at any time without notice. shentq assumes
  * no responsibility for any errors contained herein.
  * <h2><center>&copy; Copyright 2015 shentq. All Rights Reserved.</center></h2>
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __EBOX_CONFIG_H
#define __EBOX_CONFIG_H

/* 是否使用printf功能,该功能占用存储空间较多，目前确认的使用该功能的包括uart和1602
 * 1 标准库printf，优点：可靠，稳定；缺点：占用空间大
 * 2 自定义printf，有点：空间小；缺点：可能有bug。仅建议在flash紧张时使用
 */
#define USE_PRINTF 1

#if  USE_PRINTF == 1				
#include <stdio.h>
#include <stdarg.h>
#define ebox_vsnprintf(...)    vsnprintf(__VA_ARGS__)
#define ebox_snprintf(...)     snprintf(__VA_ARGS__)
#define ebox_vsprintf(...)     vsprintf(__VA_ARGS__)
#define ebox_sprintf(...)      sprintf(__VA_ARGS__)

#elif  USE_PRINTF == 2
#include "Myprintf.h"
#define ebox_vsnprintf(...)    _ebox_vsnprintf(__VA_ARGS__)
#define ebox_snprintf(...)     _ebox_snprintf(__VA_ARGS__)
#define ebox_vsprintf(...)     _ebox_vsprintf(__VA_ARGS__)
#define ebox_sprintf(...)      _ebox_sprintf(__VA_ARGS__)

#endif


#if USE_PRINTF
//是否使用DEBUG, 1 使用  0 不使用
#define EBOX_DEBUG  1
#endif

#if EBOX_DEBUG
#define DBG_UART   uart1
#define DBG(...)    ebox_printf(__VA_ARGS__)
#else
#define DBG(...)
#endif


/* flash写入新数据时，是否擦除覆盖当前区块的其它数据;
 * 0 写入新数据，并保留当前区块其他位置旧数据。 需要占用FLASH_PAGE_SIZE大小的内存作为缓冲区
 * 1 只保留新写入的数据。 节省内存
 */
#define FLASH_OVERRIDE  1

/* 是否使用object，该配置影响object.cpp & ebox.h
 * 0 不使用object，用户需要根据需要实例对象,建议在bsp_ebox中实例
 * 1 使用默认实例
 */
#define	USE_OBJECT			0

#endif

