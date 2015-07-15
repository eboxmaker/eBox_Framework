/*
file   : common.h
author : shentq
version: V1.0
date   : 2015/7/5

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


#define LSBFIRST 0
#define MSBFIRST 1

#define PI 3.1415926535898
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

//以后NVIC_PriorityGroupConfig()函数不需要再被调用。更不能再以非NVIC_GROUP_CONFIG值填充调用
#define NVIC_GROUP_CONFIG NVIC_PriorityGroup_2   /*!< 2 bits for pre-emption priority
																									 2 bits for subpriority */

#define Interrupts() 		__enable_irq()
#define noInterrupts() 	__disable_irq()
	
typedef void (*callbackFun)(void);	 
	 
void eBoxInit(void);	

uint32_t millis( void ) ;
void delay_ms(uint32_t ms);
void delay_us(uint16_t us);
void delayus(uint32_t us);

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
	
};
//对io速度要求较高的地方不建议使用
#define digitalWrite(pin,val)	pin->write(val)
#define digitalRead(pin) 			pin->read()
#define pinMode(pin,val)			pin->mode(val)

uint16_t analogRead(GPIO* pin);	
uint16_t 	analogReadToVoltage(GPIO* pin); 

void shiftOut(GPIO* dataPin, GPIO* clockPin, uint8_t bitOrder, uint8_t val);
uint8_t shiftIn(GPIO* dataPin, GPIO* clockPin, uint8_t bitOrder);
////////////////////////////////////////////////////////////////////////////////
//声明所有引脚
#define PA0 ((GPIO*)&GPA0)
#define PA1 ((GPIO*)&GPA1)
#define PA2 ((GPIO*)&GPA2)
#define PA3 ((GPIO*)&GPA3)
#define PA4 ((GPIO*)&GPA4)
#define PA5 ((GPIO*)&GPA5)
#define PA6 ((GPIO*)&GPA6)
#define PA7 ((GPIO*)&GPA7)
#define PA8 ((GPIO*)&GPA8)
#define PA9 ((GPIO*)&GPA9)
#define PA10 ((GPIO*)&GPA10)
#define PA11 ((GPIO*)&GPA11)
#define PA12 ((GPIO*)&GPA12)
#define PA13 ((GPIO*)&GPA13)
#define PA14 ((GPIO*)&GPA14)
#define PA15 ((GPIO*)&GPA15)

#define PB0 ((GPIO*)&GPB0)
#define PB1 ((GPIO*)&GPB1)
#define PB2 ((GPIO*)&GPB2)
#define PB3 ((GPIO*)&GPB3)
#define PB4 ((GPIO*)&GPB4)
#define PB5 ((GPIO*)&GPB5)
#define PB6 ((GPIO*)&GPB6)
#define PB7 ((GPIO*)&GPB7)
#define PB8 ((GPIO*)&GPB8)
#define PB9 ((GPIO*)&GPB9)
#define PB10 ((GPIO*)&GPB10)
#define PB11 ((GPIO*)&GPB11)
#define PB12 ((GPIO*)&GPB12)
#define PB13 ((GPIO*)&GPB13)
#define PB14 ((GPIO*)&GPB14)
#define PB15 ((GPIO*)&GPB15)

#define PC0 ((GPIO*)&GPC0)
#define PC1 ((GPIO*)&GPC1)
#define PC2 ((GPIO*)&GPC2)
#define PC3 ((GPIO*)&GPC3)
#define PC4 ((GPIO*)&GPC4)
#define PC5 ((GPIO*)&GPC5)
#define PC6 ((GPIO*)&GPC6)
#define PC7 ((GPIO*)&GPC7)
#define PC8 ((GPIO*)&GPC8)
#define PC9 ((GPIO*)&GPC9)
#define PC10 ((GPIO*)&GPC10)
#define PC11 ((GPIO*)&GPC11)
#define PC12 ((GPIO*)&GPC12)
#define PC13 ((GPIO*)&GPC13)
#define PC14 ((GPIO*)&GPC14)
#define PC15 ((GPIO*)&GPC15)

#define PD0 ((GPIO*)&GPD0)
#define PD1 ((GPIO*)&GPD1)
#define PD2 ((GPIO*)&GPD2)
#define PD3 ((GPIO*)&GPD3)
#define PD4 ((GPIO*)&GPD4)
#define PD5 ((GPIO*)&GPD5)
#define PD6 ((GPIO*)&GPD6)
#define PD7 ((GPIO*)&GPD7)
#define PD8 ((GPIO*)&GPD8)
#define PD9 ((GPIO*)&GPD9)
#define PD10 ((GPIO*)&GPD10)
#define PD11 ((GPIO*)&GPD11)
#define PD12 ((GPIO*)&GPD12)
#define PD13 ((GPIO*)&GPD13)
#define PD14 ((GPIO*)&GPD14)
#define PD15 ((GPIO*)&GPD15)

#define PE0 ((GPIO*)&GPE0)
#define PE1 ((GPIO*)&GPE1)
#define PE2 ((GPIO*)&GPE2)
#define PE3 ((GPIO*)&GPE3)
#define PE4 ((GPIO*)&GPE4)
#define PE5 ((GPIO*)&GPE5)
#define PE6 ((GPIO*)&GPE6)
#define PE7 ((GPIO*)&GPE7)
#define PE8 ((GPIO*)&GPE8)
#define PE9 ((GPIO*)&GPE9)
#define PE10 ((GPIO*)&GPE10)
#define PE11 ((GPIO*)&GPE11)
#define PE12 ((GPIO*)&GPE12)
#define PE13 ((GPIO*)&GPE13)
#define PE14 ((GPIO*)&GPE14)
#define PE15 ((GPIO*)&GPE15)

#define PF0 ((GPIO*)&GPF0)
#define PF1 ((GPIO*)&GPF1)
#define PF2 ((GPIO*)&GPF2)
#define PF3 ((GPIO*)&GPF3)
#define PF4 ((GPIO*)&GPF4)
#define PF5 ((GPIO*)&GPF5)
#define PF6 ((GPIO*)&GPF6)
#define PF7 ((GPIO*)&GPF7)
#define PF8 ((GPIO*)&GPF8)
#define PF9 ((GPIO*)&GPF9)
#define PF10 ((GPIO*)&GPF10)
#define PF11 ((GPIO*)&GPF11)
#define PF12 ((GPIO*)&GPF12)
#define PF13 ((GPIO*)&GPF13)
#define PF14 ((GPIO*)&GPF14)
#define PF15 ((GPIO*)&GPF15)

#define PG0 ((GPIO*)&GPG0)
#define PG1 ((GPIO*)&GPG1)
#define PG2 ((GPIO*)&GPG2)
#define PG3 ((GPIO*)&GPG3)
#define PG4 ((GPIO*)&GPG4)
#define PG5 ((GPIO*)&GPG5)
#define PG6 ((GPIO*)&GPG6)
#define PG7 ((GPIO*)&GPG7)
#define PG8 ((GPIO*)&GPG8)
#define PG9 ((GPIO*)&GPG9)
#define PG10 ((GPIO*)&GPG10)
#define PG11 ((GPIO*)&GPG11)
#define PG12 ((GPIO*)&GPG12)
#define PG13 ((GPIO*)&GPG13)
#define PG14 ((GPIO*)&GPG14)
#define PG15 ((GPIO*)&GPG15)



extern GPIO GPA0;
extern GPIO GPA1;
extern GPIO GPA2;
extern GPIO GPA3;
extern GPIO GPA4;
extern GPIO GPA5;
extern GPIO GPA6;
extern GPIO GPA7;
extern GPIO GPA8;
extern GPIO GPA9;
extern GPIO GPA10;
extern GPIO GPA11;
extern GPIO GPA12;
extern GPIO GPA13;
extern GPIO GPA14;
extern GPIO GPA15;

extern GPIO GPB0;
extern GPIO GPB1;
extern GPIO GPB2;
extern GPIO GPB3;
extern GPIO GPB4;
extern GPIO GPB5;
extern GPIO GPB5;
extern GPIO GPB6;
extern GPIO GPB7;
extern GPIO GPB8;
extern GPIO GPB9;
extern GPIO GPB10;
extern GPIO GPB11;
extern GPIO GPB12;
extern GPIO GPB13;
extern GPIO GPB14;
extern GPIO GPB15;

extern GPIO GPC0;
extern GPIO GPC1;
extern GPIO GPC2;
extern GPIO GPC3;
extern GPIO GPC4;
extern GPIO GPC5;
extern GPIO GPC5;
extern GPIO GPC6;
extern GPIO GPC7;
extern GPIO GPC8;
extern GPIO GPC9;
extern GPIO GPC10;
extern GPIO GPC11;
extern GPIO GPC12;
extern GPIO GPC13;
extern GPIO GPC14;
extern GPIO GPC15;

extern GPIO GPD0;
extern GPIO GPD1;
extern GPIO GPD2;
extern GPIO GPD3;
extern GPIO GPD4;
extern GPIO GPD5;
extern GPIO GPD5;
extern GPIO GPD6;
extern GPIO GPD7;
extern GPIO GPD8;
extern GPIO GPD9;
extern GPIO GPD10;
extern GPIO GPD11;
extern GPIO GPD12;
extern GPIO GPD13;
extern GPIO GPD14;
extern GPIO GPD15;

extern GPIO GPE0;
extern GPIO GPE1;
extern GPIO GPE2;
extern GPIO GPE3;
extern GPIO GPE4;
extern GPIO GPE5;
extern GPIO GPE5;
extern GPIO GPE6;
extern GPIO GPE7;
extern GPIO GPE8;
extern GPIO GPE9;
extern GPIO GPE10;
extern GPIO GPE11;
extern GPIO GPE12;
extern GPIO GPE13;
extern GPIO GPE14;
extern GPIO GPE15;

extern GPIO GPF0;
extern GPIO GPF1;
extern GPIO GPF2;
extern GPIO GPF3;
extern GPIO GPF4;
extern GPIO GPF5;
extern GPIO GPF5;
extern GPIO GPF6;
extern GPIO GPF7;
extern GPIO GPF8;
extern GPIO GPF9;
extern GPIO GPF10;
extern GPIO GPF11;
extern GPIO GPF12;
extern GPIO GPF13;
extern GPIO GPF14;
extern GPIO GPF15;

extern GPIO GPG0;
extern GPIO GPG1;
extern GPIO GPG2;
extern GPIO GPG3;
extern GPIO GPG4;
extern GPIO GPG5;
extern GPIO GPG5;
extern GPIO GPG6;
extern GPIO GPG7;
extern GPIO GPG8;
extern GPIO GPG9;
extern GPIO GPG10;
extern GPIO GPG11;
extern GPIO GPG12;
extern GPIO GPG13;
extern GPIO GPG14;
extern GPIO GPG15;


#ifdef __cplusplus
}
#endif

#endif
