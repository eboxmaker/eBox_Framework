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
//#include "led.h"
#include "lcd_1.8.h"
//#include "cnc.h"
#include "../Ethernet3/utility/w5500.h"

#define	HARDWARE	"ebox_spark，STM32F103C8T6"

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

#if (MCU_PINS >= 64)
extern mcuGpio PC0;
extern mcuGpio PC1;
extern mcuGpio PC2;
extern mcuGpio PC3;
extern mcuGpio PC4;
extern mcuGpio PC5;
extern mcuGpio PC5;
extern mcuGpio PC6;
extern mcuGpio PC7;
extern mcuGpio PC8;
extern mcuGpio PC9;
extern mcuGpio PC10;
extern mcuGpio PC11;
extern mcuGpio PC12;
#endif
extern mcuGpio PC13;
extern mcuGpio PC14;
extern mcuGpio PC15;

#if (MCU_PINS >= 64)
extern mcuGpio PD0;
extern mcuGpio PD1;
#endif


extern Uart uart1;
extern Uart uart2;
extern Uart uart3;

extern mcuSpi spi1;
extern mcuSpi spi2;

#endif


#define LED1 PB8
#define LED2 PB9
#define LED3 PB10

//#define LED_R   PB7
//#define LED_G   PB8
//#define LED_B   PB9

//#define PWMLED  PB8

#define BtnPin	PA8

#define	UART    uart1
#define	Serial  uart1
//#define I2C     i2c1
#define I2C     i2c2
//#define I2C     si2c1
//#define I2C     si2c2

//#define	ADC_0   PA0
//#define	ADC_1   PA1
//#define	ADC_2   PA2

//#define DAC_0   PB8



//extern Button   btn;
//extern Led      led1;
//extern Led      led2;
//extern Led      led3;
//extern CNC      cnc;
extern Lcd      lcd;
extern W5500Class w5500;

extern void print_log(const char *name = "", const char *date = "");



#endif

