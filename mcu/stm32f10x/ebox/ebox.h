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
#include "ebox_mem.h"


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









#endif

