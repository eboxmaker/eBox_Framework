/**
  ******************************************************************************
  * @file    bsp_ebox.h
  * @author  cat_li
  * @version V1.0
  * @date    2018/07/31
  * @brief   Ӳ�������Ϣ����
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
//#include "led.h"
#include "lcd_1.8.h"
//#include "cnc.h"
#include "button.h"
#include "gui.h"

#include "oled_ssd1322.h"

extern OledSSD1322 oled;
extern GUI gui;
extern GuiSideBar side_bar;

#define	HARDWARE	"ebox_spark��STM32F103C8T6"

#define UART uart1
#define LED1 PB8
#define LED2 PB9
#define LED3 PB10

extern void print_log(const char *name = "", const char *date = "");



#endif

