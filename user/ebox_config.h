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
  extern Uart uart2;
  #define OUT   uart2
  #define DBG(...) OUT.printf(__VA_ARGS__)
#else
  #define DBG(...)
#endif

#define USE_TIMEOUT 1

#endif

