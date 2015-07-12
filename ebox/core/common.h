/*
file   : common.h
author : shentq
version: V1.0
date   : 2015/7/5
brief  : This file provides time service , analog,digital and so on.

Copyright (c) 2015, eBox by shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/
#ifndef __COMMON_H
#define __COMMON_H

#include "stm32f10x.h"                  // Device header
#ifdef __cplusplus
extern "C"{
#endif



/////////////////////////////////////////


#define true 0x1
#define false 0x0
 
#define HIGH 0x1
#define LOW  0x0

#define NVIC_GROUP_CONFIG NVIC_PriorityGroup_2   /*!< 2 bits for pre-emption priority
                                                 2 bits for subpriority */

#define LSBFIRST 0
#define MSBFIRST 1

//#define CHANGE 2
//#define FALLING 3
//#define RISING 4
typedef enum
{ 
	AIN = 0x0,
  INPUT = 0x04,
  INPUT_PD = 0x28,
  INPUT_PU = 0x48,
  OUTPUT_OD = 0x14,
  OUTPUT_PP = 0x10,
  AF_OD = 0x1C,
  AF_PP = 0x18
}PINMODE;

class GPIO
{
	public:
		GPIO(GPIO_TypeDef* _port,uint16_t _pin);
		void mode(PINMODE modeVal);
		void set();
		void reset();
		void write(uint8_t val);
		void read(uint8_t* val);	
		uint8_t read(void);
	
		GPIO_TypeDef* port;
		uint16_t pin;
		uint32_t rcc;
	
};

 

//spi相关公共宏和声明//////////////////////////////////
#define SPI_MODE0 0x02
#define SPI_MODE1 0x00
#define SPI_MODE2 0x03
#define SPI_MODE3 0x01

#define SPI_CLOCK_DIV2     SPI_BaudRatePrescaler_2  
#define SPI_CLOCK_DIV4     SPI_BaudRatePrescaler_4  
#define SPI_CLOCK_DIV8     SPI_BaudRatePrescaler_8  
#define SPI_CLOCK_DIV16    SPI_BaudRatePrescaler_16 
#define SPI_CLOCK_DIV32    SPI_BaudRatePrescaler_32 
#define SPI_CLOCK_DIV64    SPI_BaudRatePrescaler_64 
#define SPI_CLOCK_DIV128   SPI_BaudRatePrescaler_128
#define SPI_CLOCK_DIV256   SPI_BaudRatePrescaler_256

#define SPI_BITODER_MSB		SPI_FirstBit_MSB                
#define SPI_BITODER_LSB		SPI_FirstBit_LSB     
           
typedef struct
{
	uint8_t devNum;
	uint8_t mode;
	uint16_t prescaler;
	uint16_t bitOrder;
}SPICONFIG;


#define Interrupts() 		__enable_irq()
#define noInterrupts() 	__disable_irq()
	
typedef void (*callbackFun)(void);	 
	 
void eBoxInit(void);	

uint32_t millis( void ) ;
void delay_ms(uint32_t ms);
void delay_us(uint16_t us);
void delayus(uint32_t us);

//不建议使用
#define digitalWrite(pin,val)	pin->write(val)
#define digitalRead(pin) 			pin->read()
#define pinMode(pin,val)			pin->mode(val)

uint16_t 	analogRead(GPIO* pin);
uint16_t 	analogReadToVoltage(GPIO* pin); 

void shiftOut(GPIO* dataPin, GPIO* clockPin, uint8_t bitOrder, uint8_t val);
uint8_t shiftIn(GPIO* dataPin, GPIO* clockPin, uint8_t bitOrder);




#ifdef __cplusplus
}
#endif

#endif
