/*
file   : iodigital.c
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

#include "ebox.h"


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
		GPIO_Init(gpio_port, &GPIO_InitStructure);   //³õÊ¼»¯GPIOC¶Ë¿Ú

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
