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

#define	HARDWARE	"stm32f072x"
#define F0

#if USE_OBJECT == 0
extern mcuGpio PA0;
extern mcuGpio PA1;
extern mcuGpio PA2;
extern mcuGpio PA3;
extern mcuGpio PA4;
extern mcuGpio PA5;
extern mcuGpio PA6;
extern mcuGpio PA7;
extern mcuGpio PA8;
extern mcuGpio PA9;
extern mcuGpio PA10;
extern mcuGpio PA11;
extern mcuGpio PA12;
extern mcuGpio PA13;
extern mcuGpio PA14;
extern mcuGpio PA15;

extern mcuGpio PB0;
extern mcuGpio PB1;
extern mcuGpio PB2;
extern mcuGpio PB3;
extern mcuGpio PB4;
extern mcuGpio PB5;
extern mcuGpio PB5;
extern mcuGpio PB6;
extern mcuGpio PB7;
extern mcuGpio PB8;
extern mcuGpio PB9;
extern mcuGpio PB10;
extern mcuGpio PB11;
extern mcuGpio PB12;
extern mcuGpio PB13;
extern mcuGpio PB14;
extern mcuGpio PB15;

extern mcuGpio PF0;
extern mcuGpio PF1;
extern mcuGpio PF6;
extern mcuGpio PF7;
extern Uart uart1;
extern Uart uart2;
#endif

#include "parallel_gpio.h"
#include "tft9325.h"
#include "graphic.h"
#include "EventGpio.h"
#include "EventManager.h"

extern Lcd 						lcd;
extern Graphic 				gpu;
//extern EventGpio 			btn;
//extern EventManager 	manager;


#define PWMLED  	PA5
#define LED1  		PA5
#define TIM3CH1		TIM3,&PA6
#define TIM1CH1		TIM1,&PA8
#define TIM1CH2		TIM1,&PA9

//#define	UART    uart1
#define UART	  uart1
//#define I2C     i2c1
#define I2C     si2c1

void print_log(const char *name, const char *date);




#endif

