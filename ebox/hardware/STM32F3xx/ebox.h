/**
  ******************************************************************************
  * @file    ebox.h
  * @author  shentq
  * @version V2.1
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



#include "ebox_core.h"


#include "ebox_gpio.h"
//#include "ebox_adc.h"
//#include "ebox_dac.h"
//#include "ebox_exti.h"
//#include "ebox_timer.h"
//#include "ebox_pwm.h"
#include "ebox_mcu_i2c.h"
#include "ebox_mcu_spi.h"
#include "ebox_uart.h"
//#include "ebox_uart_stream.h"
//#include "ebox_in_capture.h"
//#include "ebox_encoder.h"
//#include "ebox_rtc.h"
//#include "ebox_iflash.h"
//#include "ebox_wdg.h"
//#include "ebox_iflash.h"
//#include "ebox_slave_spi.h"
//#include "ebox_can.h"

#include "stdio.h"

extern "C" {

    void ebox_init(void);
}


////创建所有引脚对象和宏定义其指针
extern Uart uart1;
extern Uart uart2;
extern Uart uart3;
//extern Uart uart4;
//extern Uart uart5;

//extern mcuSpi spi1;
//extern mcuSpi spi2;
//extern SoftSpi sspi1;
//extern SoftSpi sspi2;

extern mcuI2c mcuI2c1;
//extern mcuI2c i2c2;
//extern SoftI2c si2c;
//extern SoftI2c si2c1;
//extern SoftI2c si2c2;

//extern Can can1;

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
#if (MCU_PINS >= 100)

extern mcuGpio PD2;
extern mcuGpio PD3;
extern mcuGpio PD4;
extern mcuGpio PD5;
extern mcuGpio PD5;
extern mcuGpio PD6;
extern mcuGpio PD7;
extern mcuGpio PD8;
extern mcuGpio PD9;
extern mcuGpio PD10;
extern mcuGpio PD11;
extern mcuGpio PD12;
extern mcuGpio PD13;
extern mcuGpio PD14;
extern mcuGpio PD15;

extern mcuGpio PE0;
extern mcuGpio PE1;
extern mcuGpio PE2;
extern mcuGpio PE3;
extern mcuGpio PE4;
extern mcuGpio PE5;
extern mcuGpio PE5;
extern mcuGpio PE6;
extern mcuGpio PE7;
extern mcuGpio PE8;
extern mcuGpio PE9;
extern mcuGpio PE10;
extern mcuGpio PE11;
extern mcuGpio PE12;
extern mcuGpio PE13;
extern mcuGpio PE14;
extern mcuGpio PE15;
#endif

#if (MCU_PINS >= 144)
extern mcuGpio PF0;
extern mcuGpio PF1;
extern mcuGpio PF2;
extern mcuGpio PF3;
extern mcuGpio PF4;
extern mcuGpio PF5;
extern mcuGpio PF5;
extern mcuGpio PF6;
extern mcuGpio PF7;
extern mcuGpio PF8;
extern mcuGpio PF9;
extern mcuGpio PF10;
extern mcuGpio PF11;
extern mcuGpio PF12;
extern mcuGpio PF13;
extern mcuGpio PF14;
extern mcuGpio PF15;

extern mcuGpio PG0;
extern mcuGpio PG1;
extern mcuGpio PG2;
extern mcuGpio PG3;
extern mcuGpio PG4;
extern mcuGpio PG5;
extern mcuGpio PG5;
extern mcuGpio PG6;
extern mcuGpio PG7;
extern mcuGpio PG8;
extern mcuGpio PG9;
extern mcuGpio PG10;
extern mcuGpio PG11;
extern mcuGpio PG12;
extern mcuGpio PG13;
extern mcuGpio PG14;
extern mcuGpio PG15;
#endif


#if (MCU_PINS >= 176)

extern mcuGpio PH0;
extern mcuGpio PH1;
extern mcuGpio PH2;
extern mcuGpio PH3;
extern mcuGpio PH4;
extern mcuGpio PH5;
extern mcuGpio PH6;
extern mcuGpio PH7;
extern mcuGpio PH8;
extern mcuGpio PH9;
extern mcuGpio PH10;
extern mcuGpio PH11;
extern mcuGpio PH12;
extern mcuGpio PH13;
extern mcuGpio PH14;
extern mcuGpio PH15;


extern mcuGpio PI5;
extern mcuGpio PI6;
extern mcuGpio PI7;
#endif







#endif

