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
#define USE_PRINTF	1

#if USE_PRINTF
//是否使用DEBUG, 1 使用  0 不使用
#define EBOX_DEBUG  0
#endif

#if EBOX_DEBUG
// 定义输出设备.该设备需要支持printf
#include "ebox_uart.h"
extern Uart UART;
#define DBG(...) UART.printf(__VA_ARGS__)
#else
#define DBG(...)
#endif

///*选择内存管理文件, 1 使用ebxo_mem内存管理。 0 microlib内存管理
// * ebox_mem将所有未使用内存作为内存分配区域,更灵活，但占用flash比microlib大约500byte
// * microlib可管理的内存区域由.s启动文件中heap_size决定
// */
//#define USE_EBOX_MEM  1


///*内存块大小，动态分配内存时增量,过小容易导致多次才能分配成功，过大浪费内存。最大不能
// *超过启动文件 starup_stm32fxxxx.s 中的 Heap_Size
// *printf模块中调用
// */
//#define	BUF_BLOCK				64	//64

/* flash写入新数据时，是否擦除覆盖当前区块的其它数据; 
 * 0 写入新数据，并保留当前区块其他位置旧数据。 需要占用FLASH_PAGE_SIZE大小的内存作为缓冲区
 * 1 只保留新写入的数据。 节省内存
 */
#define FLASH_OVERRIDE	1

////是否使用超时,1 使用  0 不使用
//#define USE_TIMEOUT			1
////是否使用内部参考电压 1 使用, 0 不使用
//#define USE_INTERVREF		0


#endif

