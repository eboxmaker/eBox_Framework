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

//是否使用printf功能,该功能占用存储空间较多，目前确认的使用该功能的包括uart和1602
#define USE_PRINTF1	1

#if USE_PRINTF1
  //是否使用DEBUG, 1 使用  0 不使用
  #define EBOX_DEBUG  1
#endif

#if EBOX_DEBUG
  // 定义输出设备.该设备需要支持printf
  #include "ebox_uart.h"
  extern Uart uart1;
  #define OUT   uart1
  #define DBG(...) OUT.printf(__VA_ARGS__)
#else
  #define DBG(...)
#endif

#define USE_TIMEOUT   1

/*选择内存管理文件, 1 使用ebxo_mem内存管理。 0 不使用
 * ebox_mem将所有未使用内存作为内存分配区域,更灵活，但占用flash比microlib大约500byte
 */
#define USE_EBOX_MEM  1


/* flash写入新数据时，是否擦除覆盖当前区块的其它数据; 
 * 0 写入新数据，并保留当前区块其他位置旧数据。 需要占用FLASH_PAGE_SIZE大小的内存作为缓冲区
 * 1 只保留新写入的数据。 节省内存
 */
#define FLASH_OVERRIDE  1

#endif

