/*
file   : ebox.h
author : shentq
version: V1.1
date   : 2016/4/6

Copyright 2015 shentq. All Rights Reserved.

Copyright Notice
No GPArt of this software may be used for any commercial activities by any form or means, without the prior written consent of shentqlf@163.com.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentqlf@163.com assumes no responsibility for any errors contained herein.
*/
#ifndef __EBOX_H
#define __EBOX_H



#include "common.h"



#include "can.h"
#include "exti.h"
#include "gtimer.h"
#include "pwm.h"
#include "in_capture.h"
#include "i2c.h"
#include "spi.h"
#include "uart.h"
#include "timer_one.h"
#include "rtc.h"
#include "iflash.h"
#include "dac.h"
#include "wdg.h"



#include "stdio.h"



////////系统debug支持////////////////////////////////////////////////////////////////
#define EBOX_DEBUG 1
#if EBOX_DEBUG
extern USART uart1;//根据不同的串口名称此处需要做相应的修改
#define DBG(...) uart1.printf(__VA_ARGS__)
#else
#define  DBG(...)
#endif
////////////////////////////////////////////////////////////////////////////////
//创建所有引脚对象和宏定义其指针
extern USART uart1;
extern USART uart2;
extern USART uart3;
extern USART uart4;
extern USART uart5;

extern SPI spi1;
extern SPI spi2;
extern SOFTSPI sspi1;

extern I2C i2c1;
extern I2C i2c2;
extern SOFTI2C si2c;
extern SOFTI2C si2c1;
extern SOFTI2C si2c2;

extern CAN can1;

extern GPIO PA0;
extern GPIO PA1;
extern GPIO PA2;
extern GPIO PA3;
extern GPIO PA4;
extern GPIO PA5;
extern GPIO PA6;
extern GPIO PA7;
extern GPIO PA8;
extern GPIO PA9;
extern GPIO PA10;
extern GPIO PA11;
extern GPIO PA12;
extern GPIO PA13;
extern GPIO PA14;
extern GPIO PA15;

extern GPIO PB0;
extern GPIO PB1;
extern GPIO PB2;
extern GPIO PB3;
extern GPIO PB4;
extern GPIO PB5;
extern GPIO PB5;
extern GPIO PB6;
extern GPIO PB7;
extern GPIO PB8;
extern GPIO PB9;
extern GPIO PB10;
extern GPIO PB11;
extern GPIO PB12;
extern GPIO PB13;
extern GPIO PB14;
extern GPIO PB15;

extern GPIO PC0;
extern GPIO PC1;
extern GPIO PC2;
extern GPIO PC3;
extern GPIO PC4;
extern GPIO PC5;
extern GPIO PC5;
extern GPIO PC6;
extern GPIO PC7;
extern GPIO PC8;
extern GPIO PC9;
extern GPIO PC10;
extern GPIO PC11;
extern GPIO PC12;
extern GPIO PC13;
extern GPIO PC14;
extern GPIO PC15;

extern GPIO PD0;
extern GPIO PD1;
extern GPIO PD2;
extern GPIO PD3;
extern GPIO PD4;
extern GPIO PD5;
extern GPIO PD5;
extern GPIO PD6;
extern GPIO PD7;
extern GPIO PD8;
extern GPIO PD9;
extern GPIO PD10;
extern GPIO PD11;
extern GPIO PD12;
extern GPIO PD13;
extern GPIO PD14;
extern GPIO PD15;

extern GPIO PE0;
extern GPIO PE1;
extern GPIO PE2;
extern GPIO PE3;
extern GPIO PE4;
extern GPIO PE5;
extern GPIO PE5;
extern GPIO PE6;
extern GPIO PE7;
extern GPIO PE8;
extern GPIO PE9;
extern GPIO PE10;
extern GPIO PE11;
extern GPIO PE12;
extern GPIO PE13;
extern GPIO PE14;
extern GPIO PE15;

extern GPIO PF0;
extern GPIO PF1;
extern GPIO PF2;
extern GPIO PF3;
extern GPIO PF4;
extern GPIO PF5;
extern GPIO PF5;
extern GPIO PF6;
extern GPIO PF7;
extern GPIO PF8;
extern GPIO PF9;
extern GPIO PF10;
extern GPIO PF11;
extern GPIO PF12;
extern GPIO PF13;
extern GPIO PF14;
extern GPIO PF15;

extern GPIO PG0;
extern GPIO PG1;
extern GPIO PG2;
extern GPIO PG3;
extern GPIO PG4;
extern GPIO PG5;
extern GPIO PG5;
extern GPIO PG6;
extern GPIO PG7;
extern GPIO PG8;
extern GPIO PG9;
extern GPIO PG10;
extern GPIO PG11;
extern GPIO PG12;
extern GPIO PG13;
extern GPIO PG14;
extern GPIO PG15;





#define PPA0 ((GPIO*)&PA0)
#define PPA1 ((GPIO*)&PA1)
#define PPA2 ((GPIO*)&PA2)
#define PPA3 ((GPIO*)&PA3)
#define PPA4 ((GPIO*)&PA4)
#define PPA5 ((GPIO*)&PA5)
#define PPA6 ((GPIO*)&PA6)
#define PPA7 ((GPIO*)&PA7)
#define PPA8 ((GPIO*)&PA8)
#define PPA9 ((GPIO*)&PA9)
#define PPA10 ((GPIO*)&PA10)
#define PPA11 ((GPIO*)&PA11)
#define PPA12 ((GPIO*)&PA12)
#define PPA13 ((GPIO*)&PA13)
#define PPA14 ((GPIO*)&PA14)
#define PPA15 ((GPIO*)&PA15)

#define PPB0 ((GPIO*)&PB0)
#define PPB1 ((GPIO*)&PB1)
#define PPB2 ((GPIO*)&PB2)
#define PPB3 ((GPIO*)&PB3)
#define PPB4 ((GPIO*)&PB4)
#define PPB5 ((GPIO*)&PB5)
#define PPB6 ((GPIO*)&PB6)
#define PPB7 ((GPIO*)&PB7)
#define PPB8 ((GPIO*)&PB8)
#define PPB9 ((GPIO*)&PB9)
#define PPB10 ((GPIO*)&PB10)
#define PPB11 ((GPIO*)&PB11)
#define PPB12 ((GPIO*)&PB12)
#define PPB13 ((GPIO*)&PB13)
#define PPB14 ((GPIO*)&PB14)
#define PPB15 ((GPIO*)&PB15)

#define PPC0 ((GPIO*)&PC0)
#define PPC1 ((GPIO*)&PC1)
#define PPC2 ((GPIO*)&PC2)
#define PPC3 ((GPIO*)&PC3)
#define PPC4 ((GPIO*)&PC4)
#define PPC5 ((GPIO*)&PC5)
#define PPC6 ((GPIO*)&PC6)
#define PPC7 ((GPIO*)&PC7)
#define PPC8 ((GPIO*)&PC8)
#define PPC9 ((GPIO*)&PC9)
#define PPC10 ((GPIO*)&PC10)
#define PPC11 ((GPIO*)&PC11)
#define PPC12 ((GPIO*)&PC12)
#define PPC13 ((GPIO*)&PC13)
#define PPC14 ((GPIO*)&PC14)
#define PPC15 ((GPIO*)&PC15)

#define PPD0 ((GPIO*)&PD0)
#define PPD1 ((GPIO*)&PD1)
#define PPD2 ((GPIO*)&PD2)
#define PPD3 ((GPIO*)&PD3)
#define PPD4 ((GPIO*)&PD4)
#define PPD5 ((GPIO*)&PD5)
#define PPD6 ((GPIO*)&PD6)
#define PPD7 ((GPIO*)&PD7)
#define PPD8 ((GPIO*)&PD8)
#define PPD9 ((GPIO*)&PD9)
#define PPD10 ((GPIO*)&PD10)
#define PPD11 ((GPIO*)&PD11)
#define PPD12 ((GPIO*)&PD12)
#define PPD13 ((GPIO*)&PD13)
#define PPD14 ((GPIO*)&PD14)
#define PPD15 ((GPIO*)&PD15)

#define PPE0 ((GPIO*)&PE0)
#define PPE1 ((GPIO*)&PE1)
#define PPE2 ((GPIO*)&PE2)
#define PPE3 ((GPIO*)&PE3)
#define PPE4 ((GPIO*)&PE4)
#define PPE5 ((GPIO*)&PE5)
#define PPE6 ((GPIO*)&PE6)
#define PPE7 ((GPIO*)&PE7)
#define PPE8 ((GPIO*)&PE8)
#define PPE9 ((GPIO*)&PE9)
#define PPE10 ((GPIO*)&PE10)
#define PPE11 ((GPIO*)&PE11)
#define PPE12 ((GPIO*)&PE12)
#define PPE13 ((GPIO*)&PE13)
#define PPE14 ((GPIO*)&PE14)
#define PPE15 ((GPIO*)&PE15)

#define PPF0 ((GPIO*)&PF0)
#define PPF1 ((GPIO*)&PF1)
#define PPF2 ((GPIO*)&PF2)
#define PPF3 ((GPIO*)&PF3)
#define PPF4 ((GPIO*)&PF4)
#define PPF5 ((GPIO*)&PF5)
#define PPF6 ((GPIO*)&PF6)
#define PPF7 ((GPIO*)&PF7)
#define PPF8 ((GPIO*)&PF8)
#define PPF9 ((GPIO*)&PF9)
#define PPF10 ((GPIO*)&PF10)
#define PPF11 ((GPIO*)&PF11)
#define PPF12 ((GPIO*)&PF12)
#define PPF13 ((GPIO*)&PF13)
#define PPF14 ((GPIO*)&PF14)
#define PPF15 ((GPIO*)&PF15)

#define PPG0 ((GPIO*)&PG0)
#define PPG1 ((GPIO*)&PG1)
#define PPG2 ((GPIO*)&PG2)
#define PPG3 ((GPIO*)&PG3)
#define PPG4 ((GPIO*)&PG4)
#define PPG5 ((GPIO*)&PG5)
#define PPG6 ((GPIO*)&PG6)
#define PPG7 ((GPIO*)&PG7)
#define PPG8 ((GPIO*)&PG8)
#define PPG9 ((GPIO*)&PG9)
#define PPG10 ((GPIO*)&PG10)
#define PPG11 ((GPIO*)&PG11)
#define PPG12 ((GPIO*)&PG12)
#define PPG13 ((GPIO*)&PG13)
#define PPPG14 ((GPIO*)&PG14)
#define PPG15 ((GPIO*)&PG15)







#endif

