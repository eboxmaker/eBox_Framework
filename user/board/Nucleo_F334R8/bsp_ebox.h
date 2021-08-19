/**
  ******************************************************************************
  * @file    bsp_ebox.h
  * @author  cat_li
  * @version V1.0
  * @date    2018/07/31
  * @brief   硬件相关信息声明
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
#ifndef __BSP_EBOX_H
#define __BSP_EBOX_H
#include "ebox.h"
#include "soft_i2c.h"

////#include "w5500.h"
//#include "../Ethernet3/utility/w5500.h"
//#include "button.h"
//#include "led.h"
#define	HARDWARE	"ebox_spark，STM32F407IGT6"

extern SoftI2c Wire;

#define LED1 PA5
#define BTN1 PC13
#define UART uart2


void print_log(const char *name = "test", const char *date = __DATE__);

#endif

