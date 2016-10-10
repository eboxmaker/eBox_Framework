/**
  ******************************************************************************
  * @file    ebox.h
  * @author  shentq
  * @version V1.2
  * @date    2016/08/14
  * @brief   
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

#ifndef __EBOX_H
#define __EBOX_H



#include "ebox_common.h"



#include "ebox_can.h"
#include "ebox_exti.h"
#include "ebox_timer.h"
#include "ebox_pwm.h"
#include "ebox_in_capture.h"
#include "ebox_i2c.h"
#include "ebox_spi.h"
#include "ebox_uart.h"
#include "ebox_rtc.h"
#include "ebox_iflash.h"
#include "ebox_dac.h"
#include "ebox_wdg.h"



#include "stdio.h"



////////系统debug支持////////////////////////////////////////////////////////////////
#define EBOX_DEBUG 1
#if EBOX_DEBUG
extern Uart uart1;//根据不同的串口名称此处需要做相应的修改
#define DBG(...) uart1.printf(__VA_ARGS__)
#else
#define  DBG(...)
#endif
////////////////////////////////////////////////////////////////////////////////
//创建所有引脚对象和宏定义其指针
extern Uart uart1;
extern Uart uart2;
extern Uart uart3;
extern Uart uart4;
extern Uart uart5;

extern Spi spi1;
extern Spi spi2;
extern SoftSpi sspi1;

extern I2c i2c1;
extern I2c i2c2;
extern SoftI2c si2c;
extern SoftI2c si2c1;
extern SoftI2c si2c2;

extern Can can1;

extern Gpio PA0;
extern Gpio PA1;
extern Gpio PA2;
extern Gpio PA3;
extern Gpio PA4;
extern Gpio PA5;
extern Gpio PA6;
extern Gpio PA7;
extern Gpio PA8;
extern Gpio PA9;
extern Gpio PA10;
extern Gpio PA11;
extern Gpio PA12;
extern Gpio PA13;
extern Gpio PA14;
extern Gpio PA15;

extern Gpio PB0;
extern Gpio PB1;
extern Gpio PB2;
extern Gpio PB3;
extern Gpio PB4;
extern Gpio PB5;
extern Gpio PB5;
extern Gpio PB6;
extern Gpio PB7;
extern Gpio PB8;
extern Gpio PB9;
extern Gpio PB10;
extern Gpio PB11;
extern Gpio PB12;
extern Gpio PB13;
extern Gpio PB14;
extern Gpio PB15;

extern Gpio PC0;
extern Gpio PC1;
extern Gpio PC2;
extern Gpio PC3;
extern Gpio PC4;
extern Gpio PC5;
extern Gpio PC5;
extern Gpio PC6;
extern Gpio PC7;
extern Gpio PC8;
extern Gpio PC9;
extern Gpio PC10;
extern Gpio PC11;
extern Gpio PC12;
extern Gpio PC13;
extern Gpio PC14;
extern Gpio PC15;

extern Gpio PD0;
extern Gpio PD1;
extern Gpio PD2;
extern Gpio PD3;
extern Gpio PD4;
extern Gpio PD5;
extern Gpio PD5;
extern Gpio PD6;
extern Gpio PD7;
extern Gpio PD8;
extern Gpio PD9;
extern Gpio PD10;
extern Gpio PD11;
extern Gpio PD12;
extern Gpio PD13;
extern Gpio PD14;
extern Gpio PD15;

extern Gpio PE0;
extern Gpio PE1;
extern Gpio PE2;
extern Gpio PE3;
extern Gpio PE4;
extern Gpio PE5;
extern Gpio PE5;
extern Gpio PE6;
extern Gpio PE7;
extern Gpio PE8;
extern Gpio PE9;
extern Gpio PE10;
extern Gpio PE11;
extern Gpio PE12;
extern Gpio PE13;
extern Gpio PE14;
extern Gpio PE15;

extern Gpio PF0;
extern Gpio PF1;
extern Gpio PF2;
extern Gpio PF3;
extern Gpio PF4;
extern Gpio PF5;
extern Gpio PF5;
extern Gpio PF6;
extern Gpio PF7;
extern Gpio PF8;
extern Gpio PF9;
extern Gpio PF10;
extern Gpio PF11;
extern Gpio PF12;
extern Gpio PF13;
extern Gpio PF14;
extern Gpio PF15;

extern Gpio PG0;
extern Gpio PG1;
extern Gpio PG2;
extern Gpio PG3;
extern Gpio PG4;
extern Gpio PG5;
extern Gpio PG5;
extern Gpio PG6;
extern Gpio PG7;
extern Gpio PG8;
extern Gpio PG9;
extern Gpio PG10;
extern Gpio PG11;
extern Gpio PG12;
extern Gpio PG13;
extern Gpio PG14;
extern Gpio PG15;





#define PPA0 ((Gpio*)&PA0)
#define PPA1 ((Gpio*)&PA1)
#define PPA2 ((Gpio*)&PA2)
#define PPA3 ((Gpio*)&PA3)
#define PPA4 ((Gpio*)&PA4)
#define PPA5 ((Gpio*)&PA5)
#define PPA6 ((Gpio*)&PA6)
#define PPA7 ((Gpio*)&PA7)
#define PPA8 ((Gpio*)&PA8)
#define PPA9 ((Gpio*)&PA9)
#define PPA10 ((Gpio*)&PA10)
#define PPA11 ((Gpio*)&PA11)
#define PPA12 ((Gpio*)&PA12)
#define PPA13 ((Gpio*)&PA13)
#define PPA14 ((Gpio*)&PA14)
#define PPA15 ((Gpio*)&PA15)

#define PPB0 ((Gpio*)&PB0)
#define PPB1 ((Gpio*)&PB1)
#define PPB2 ((Gpio*)&PB2)
#define PPB3 ((Gpio*)&PB3)
#define PPB4 ((Gpio*)&PB4)
#define PPB5 ((Gpio*)&PB5)
#define PPB6 ((Gpio*)&PB6)
#define PPB7 ((Gpio*)&PB7)
#define PPB8 ((Gpio*)&PB8)
#define PPB9 ((Gpio*)&PB9)
#define PPB10 ((Gpio*)&PB10)
#define PPB11 ((Gpio*)&PB11)
#define PPB12 ((Gpio*)&PB12)
#define PPB13 ((Gpio*)&PB13)
#define PPB14 ((Gpio*)&PB14)
#define PPB15 ((Gpio*)&PB15)

#define PPC0 ((Gpio*)&PC0)
#define PPC1 ((Gpio*)&PC1)
#define PPC2 ((Gpio*)&PC2)
#define PPC3 ((Gpio*)&PC3)
#define PPC4 ((Gpio*)&PC4)
#define PPC5 ((Gpio*)&PC5)
#define PPC6 ((Gpio*)&PC6)
#define PPC7 ((Gpio*)&PC7)
#define PPC8 ((Gpio*)&PC8)
#define PPC9 ((Gpio*)&PC9)
#define PPC10 ((Gpio*)&PC10)
#define PPC11 ((Gpio*)&PC11)
#define PPC12 ((Gpio*)&PC12)
#define PPC13 ((Gpio*)&PC13)
#define PPC14 ((Gpio*)&PC14)
#define PPC15 ((Gpio*)&PC15)

#define PPD0 ((Gpio*)&PD0)
#define PPD1 ((Gpio*)&PD1)
#define PPD2 ((Gpio*)&PD2)
#define PPD3 ((Gpio*)&PD3)
#define PPD4 ((Gpio*)&PD4)
#define PPD5 ((Gpio*)&PD5)
#define PPD6 ((Gpio*)&PD6)
#define PPD7 ((Gpio*)&PD7)
#define PPD8 ((Gpio*)&PD8)
#define PPD9 ((Gpio*)&PD9)
#define PPD10 ((Gpio*)&PD10)
#define PPD11 ((Gpio*)&PD11)
#define PPD12 ((Gpio*)&PD12)
#define PPD13 ((Gpio*)&PD13)
#define PPD14 ((Gpio*)&PD14)
#define PPD15 ((Gpio*)&PD15)

#define PPE0 ((Gpio*)&PE0)
#define PPE1 ((Gpio*)&PE1)
#define PPE2 ((Gpio*)&PE2)
#define PPE3 ((Gpio*)&PE3)
#define PPE4 ((Gpio*)&PE4)
#define PPE5 ((Gpio*)&PE5)
#define PPE6 ((Gpio*)&PE6)
#define PPE7 ((Gpio*)&PE7)
#define PPE8 ((Gpio*)&PE8)
#define PPE9 ((Gpio*)&PE9)
#define PPE10 ((Gpio*)&PE10)
#define PPE11 ((Gpio*)&PE11)
#define PPE12 ((Gpio*)&PE12)
#define PPE13 ((Gpio*)&PE13)
#define PPE14 ((Gpio*)&PE14)
#define PPE15 ((Gpio*)&PE15)

#define PPF0 ((Gpio*)&PF0)
#define PPF1 ((Gpio*)&PF1)
#define PPF2 ((Gpio*)&PF2)
#define PPF3 ((Gpio*)&PF3)
#define PPF4 ((Gpio*)&PF4)
#define PPF5 ((Gpio*)&PF5)
#define PPF6 ((Gpio*)&PF6)
#define PPF7 ((Gpio*)&PF7)
#define PPF8 ((Gpio*)&PF8)
#define PPF9 ((Gpio*)&PF9)
#define PPF10 ((Gpio*)&PF10)
#define PPF11 ((Gpio*)&PF11)
#define PPF12 ((Gpio*)&PF12)
#define PPF13 ((Gpio*)&PF13)
#define PPF14 ((Gpio*)&PF14)
#define PPF15 ((Gpio*)&PF15)

#define PPG0 ((Gpio*)&PG0)
#define PPG1 ((Gpio*)&PG1)
#define PPG2 ((Gpio*)&PG2)
#define PPG3 ((Gpio*)&PG3)
#define PPG4 ((Gpio*)&PG4)
#define PPG5 ((Gpio*)&PG5)
#define PPG6 ((Gpio*)&PG6)
#define PPG7 ((Gpio*)&PG7)
#define PPG8 ((Gpio*)&PG8)
#define PPG9 ((Gpio*)&PG9)
#define PPG10 ((Gpio*)&PG10)
#define PPG11 ((Gpio*)&PG11)
#define PPG12 ((Gpio*)&PG12)
#define PPG13 ((Gpio*)&PG13)
#define PPPG14 ((Gpio*)&PG14)
#define PPG15 ((Gpio*)&PG15)







#endif

