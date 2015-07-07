/*
file   : digital.cpp
author : shentq
version: V1.0
date   : 2015/7/5
brief  :   digital input and output function

Copyright (c) 2015, eBox by shentqlf@163.com. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentqlf@163.com.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentqlf@163.com assumes no responsibility for any errors contained herein.
*/

#include "common.h"

//uint16_t ioPinStatu[TOTAL_PIN];//IO口是否使用

typedef struct 
{
  GPIO_TypeDef* port ;
  uint32_t pin ;
  uint32_t rcc ;
} PIN_INFO ;

const PIN_INFO pinInfo[]=
{
	//0-15
  { GPIOA, GPIO_Pin_0,  RCC_APB2Periph_GPIOA}, //1
  { GPIOA, GPIO_Pin_1,  RCC_APB2Periph_GPIOA}, // 2
  { GPIOA, GPIO_Pin_2,  RCC_APB2Periph_GPIOA}, // 3
  { GPIOA, GPIO_Pin_3,  RCC_APB2Periph_GPIOA}, //4
  { GPIOA, GPIO_Pin_4,  RCC_APB2Periph_GPIOA}, // 5
  { GPIOA, GPIO_Pin_5,  RCC_APB2Periph_GPIOA}, //6
  { GPIOA, GPIO_Pin_6,  RCC_APB2Periph_GPIOA}, // 7
  { GPIOA, GPIO_Pin_7,  RCC_APB2Periph_GPIOA}, //8
  { GPIOA, GPIO_Pin_8,  RCC_APB2Periph_GPIOA}, // 9
  { GPIOA, GPIO_Pin_9,  RCC_APB2Periph_GPIOA}, //10
  { GPIOA, GPIO_Pin_10,  RCC_APB2Periph_GPIOA}, // 11
  { GPIOA, GPIO_Pin_11,  RCC_APB2Periph_GPIOA}, //12
  { GPIOA, GPIO_Pin_12,  RCC_APB2Periph_GPIOA}, // 13
  { GPIOA, GPIO_Pin_13,  RCC_APB2Periph_GPIOA}, //14
  { GPIOA, GPIO_Pin_14,  RCC_APB2Periph_GPIOA}, // 15
  { GPIOA, GPIO_Pin_15,  RCC_APB2Periph_GPIOA}, //16

	
	
	//16-31
  { GPIOB, GPIO_Pin_0,  RCC_APB2Periph_GPIOB}, //16 
  { GPIOB, GPIO_Pin_1,  RCC_APB2Periph_GPIOB}, //17 
  { GPIOB, GPIO_Pin_2,  RCC_APB2Periph_GPIOB}, //18 
  { GPIOB, GPIO_Pin_3,  RCC_APB2Periph_GPIOB}, // 19
  { GPIOB, GPIO_Pin_4,  RCC_APB2Periph_GPIOB}, // 20
  { GPIOB, GPIO_Pin_5,  RCC_APB2Periph_GPIOB}, // 21
  { GPIOB, GPIO_Pin_6,  RCC_APB2Periph_GPIOB}, // 22
  { GPIOB, GPIO_Pin_7,  RCC_APB2Periph_GPIOB}, // 23
  { GPIOB, GPIO_Pin_8,  RCC_APB2Periph_GPIOB}, // 24
  { GPIOB, GPIO_Pin_9,  RCC_APB2Periph_GPIOB}, // 25
  { GPIOB, GPIO_Pin_10,  RCC_APB2Periph_GPIOB}, // 26
  { GPIOB, GPIO_Pin_11,  RCC_APB2Periph_GPIOB}, // 27
  { GPIOB, GPIO_Pin_12,  RCC_APB2Periph_GPIOB}, //28 
  { GPIOB, GPIO_Pin_13,  RCC_APB2Periph_GPIOB}, // 29
  { GPIOB, GPIO_Pin_14,  RCC_APB2Periph_GPIOB}, // 30
  { GPIOB, GPIO_Pin_15,  RCC_APB2Periph_GPIOB}, // 31
	
	//32-47
	{ GPIOC, GPIO_Pin_0,  RCC_APB2Periph_GPIOC}, // 32
  { GPIOC, GPIO_Pin_1,  RCC_APB2Periph_GPIOC}, //33 
  { GPIOC, GPIO_Pin_2,  RCC_APB2Periph_GPIOC}, //34 
  { GPIOC, GPIO_Pin_3,  RCC_APB2Periph_GPIOC}, //35 
  { GPIOC, GPIO_Pin_4,  RCC_APB2Periph_GPIOC}, //36 
  { GPIOC, GPIO_Pin_5,  RCC_APB2Periph_GPIOC}, //37 
  { GPIOC, GPIO_Pin_6,  RCC_APB2Periph_GPIOC}, //38 
  { GPIOC, GPIO_Pin_7,  RCC_APB2Periph_GPIOC}, //39 
  { GPIOC, GPIO_Pin_8,  RCC_APB2Periph_GPIOC}, //40 
  { GPIOC, GPIO_Pin_9,  RCC_APB2Periph_GPIOC}, //41 
  { GPIOC, GPIO_Pin_10,  RCC_APB2Periph_GPIOC}, //42 
  { GPIOC, GPIO_Pin_11,  RCC_APB2Periph_GPIOC}, //43 
  { GPIOC, GPIO_Pin_12,  RCC_APB2Periph_GPIOC}, // 44
  { GPIOC, GPIO_Pin_13,  RCC_APB2Periph_GPIOC}, //45 
  { GPIOC, GPIO_Pin_14,  RCC_APB2Periph_GPIOC}, //46 
  { GPIOC, GPIO_Pin_15,  RCC_APB2Periph_GPIOC}, // 47
	
	//48-63
	{ GPIOD, GPIO_Pin_0,  RCC_APB2Periph_GPIOD}, //48 
  { GPIOD, GPIO_Pin_1,  RCC_APB2Periph_GPIOD}, //49 
  { GPIOD, GPIO_Pin_2,  RCC_APB2Periph_GPIOD}, //50 
  { GPIOD, GPIO_Pin_3,  RCC_APB2Periph_GPIOD}, // 51
  { GPIOD, GPIO_Pin_4,  RCC_APB2Periph_GPIOD}, // 52
  { GPIOD, GPIO_Pin_5,  RCC_APB2Periph_GPIOD}, // 53
  { GPIOD, GPIO_Pin_6,  RCC_APB2Periph_GPIOD}, // 54
  { GPIOD, GPIO_Pin_7,  RCC_APB2Periph_GPIOD}, // 55
  { GPIOD, GPIO_Pin_8,  RCC_APB2Periph_GPIOD}, // 56
  { GPIOD, GPIO_Pin_9,  RCC_APB2Periph_GPIOD}, //57 
  { GPIOD, GPIO_Pin_10,  RCC_APB2Periph_GPIOD}, //58 
  { GPIOD, GPIO_Pin_11,  RCC_APB2Periph_GPIOD}, //59 
  { GPIOD, GPIO_Pin_12,  RCC_APB2Periph_GPIOD}, //60 
  { GPIOD, GPIO_Pin_13,  RCC_APB2Periph_GPIOD}, //61 
  { GPIOD, GPIO_Pin_14,  RCC_APB2Periph_GPIOD}, //62 
  { GPIOD, GPIO_Pin_15,  RCC_APB2Periph_GPIOD}, // 63	

	//64-79
	{ GPIOE, GPIO_Pin_0,  RCC_APB2Periph_GPIOE}, // 64
  { GPIOE, GPIO_Pin_1,  RCC_APB2Periph_GPIOE}, // 65
  { GPIOE, GPIO_Pin_2,  RCC_APB2Periph_GPIOE}, // 66
  { GPIOE, GPIO_Pin_3,  RCC_APB2Periph_GPIOE}, // 67
  { GPIOE, GPIO_Pin_4,  RCC_APB2Periph_GPIOE}, // 68
  { GPIOE, GPIO_Pin_5,  RCC_APB2Periph_GPIOE}, // 69
  { GPIOE, GPIO_Pin_6,  RCC_APB2Periph_GPIOE}, // 70
  { GPIOE, GPIO_Pin_7,  RCC_APB2Periph_GPIOE}, // 71
  { GPIOE, GPIO_Pin_8,  RCC_APB2Periph_GPIOE}, // 72
  { GPIOE, GPIO_Pin_9,  RCC_APB2Periph_GPIOE}, // 73
  { GPIOE, GPIO_Pin_10,  RCC_APB2Periph_GPIOE}, // 74
  { GPIOE, GPIO_Pin_11,  RCC_APB2Periph_GPIOE}, // 75
  { GPIOE, GPIO_Pin_12,  RCC_APB2Periph_GPIOE}, //76 
  { GPIOE, GPIO_Pin_13,  RCC_APB2Periph_GPIOE}, // 77
  { GPIOE, GPIO_Pin_14,  RCC_APB2Periph_GPIOE}, // 78
  { GPIOE, GPIO_Pin_15,  RCC_APB2Periph_GPIOE}, // 79	
	//80-95
	{ GPIOF, GPIO_Pin_0,  RCC_APB2Periph_GPIOF}, // 80
  { GPIOF, GPIO_Pin_1,  RCC_APB2Periph_GPIOF}, // 81
  { GPIOF, GPIO_Pin_2,  RCC_APB2Periph_GPIOF}, // 82
  { GPIOF, GPIO_Pin_3,  RCC_APB2Periph_GPIOF}, // 83
  { GPIOF, GPIO_Pin_4,  RCC_APB2Periph_GPIOF}, // 84
  { GPIOF, GPIO_Pin_5,  RCC_APB2Periph_GPIOF}, // 85
  { GPIOF, GPIO_Pin_6,  RCC_APB2Periph_GPIOF}, // 86
  { GPIOE, GPIO_Pin_7,  RCC_APB2Periph_GPIOF}, // 87
  { GPIOF, GPIO_Pin_8,  RCC_APB2Periph_GPIOF}, // 88
  { GPIOF, GPIO_Pin_9,  RCC_APB2Periph_GPIOF}, // 89
  { GPIOF, GPIO_Pin_10,  RCC_APB2Periph_GPIOF}, // 90
  { GPIOF, GPIO_Pin_11,  RCC_APB2Periph_GPIOF}, // 91
  { GPIOF, GPIO_Pin_12,  RCC_APB2Periph_GPIOF}, //92 
  { GPIOF, GPIO_Pin_13,  RCC_APB2Periph_GPIOF}, // 93
  { GPIOF, GPIO_Pin_14,  RCC_APB2Periph_GPIOF}, // 94
  { GPIOF, GPIO_Pin_15,  RCC_APB2Periph_GPIOF}, // 95
};

void pinMode(uint32_t pin, uint32_t mode)
{
	
//	uint8_t bit = digitalPinToBitMask(pin);
//	uint8_t port = digitalPinToPort(pin);
	
//	GPIO_TypeDef *gpio_port = g_APinDescription[Pin].pPort;
//  uint16_t gpio_pin = g_APinDescription[Pin].Pin;
	GPIO_TypeDef * gpio_port = pinInfo[pin].port;
	uint16_t gpio_pin = pinInfo[pin].pin;
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
  RCC_APB2PeriphClockCmd(pinInfo[pin].rcc,ENABLE);
  GPIO_InitStructure.GPIO_Pin = gpio_pin;

	switch(mode)
	{
		/////////////////////////////////////////////////////////
		case INPUT:
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		break;
		case INPUT_PULLUP:
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		break;
		case INPUT_PULLDOWN:
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	//////////////////////////////////////////////////////////
		case OUTPUT:
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		break;
		case AF_PP:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //
		  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		break;
	  case AF_OD:
			 RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //
			 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
       GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  break;
		/////////////////////////////////////////////////////////////
	  case AIN:
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    break;

		default:
		break ;
	}
		GPIO_Init(gpio_port, &GPIO_InitStructure);   //初始化GPIOC端口

}
void digitalWrite( uint32_t pin, uint32_t ulVal )
{
  if ( ulVal == 0 )
  {
    pinInfo[pin].port->BRR = pinInfo[pin].pin;
  }
  else
  {
    pinInfo[pin].port->BSRR = pinInfo[pin].pin;
  }
}
 int digitalRead( uint32_t pin )
{
	if((pinInfo[pin].port->IDR & pinInfo[pin].pin) != 0)
		return 1;
	return 0;
}
uint8_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder) {
	uint8_t value = 0;
	uint8_t i;

	for (i = 0; i < 8; ++i) {
		digitalWrite(clockPin, HIGH);
		if (bitOrder == LSBFIRST)
			value |= digitalRead(dataPin) << i;
		else
			value |= digitalRead(dataPin) << (7 - i);
		digitalWrite(clockPin, LOW);
	}
	return value;
}

void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val)
{
	int i;
	for (i = 0; i < 8; i++)  
	{
		if (bitOrder == LSBFIRST)
			digitalWrite(dataPin, !!(val & (1 << i)));
		else	
			digitalWrite(dataPin, !!(val & (1 << (7 - i))));
			digitalWrite(clockPin, HIGH);
			digitalWrite(clockPin, LOW);
	}
		
	
}
