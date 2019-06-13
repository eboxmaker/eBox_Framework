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


#define	HARDWARE	"stm32nucleo，STM32F030F4"
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

extern mcuGpio PA13;
extern mcuGpio PA14;

extern mcuGpio PB1;

extern mcuGpio PF0;
extern mcuGpio PF1;


extern Uart uart1;

#endif

#define LED1 PA5
#define LED2 PA5
//#define LED3 PB10

//#define LED_R   PB7
//#define LED_G   PB8
//#define LED_B   PB9

#define PWMLED  PA5
#define TIM3CH1		TIM3,&PA6
#define TIM1CH1		TIM1,&PA8
#define TIM1CH2		TIM1,&PA9

#define BtnPin	PC13

#define	UART    uart1

//#define I2C     i2c1
#define I2C     si2c1

//#define	ADC_0   PA0
//#define	ADC_1   PA1
//#define	ADC_2   PA2

//#define DAC_0   PB8
void print_log(const char *name, const char *date);




#endif

