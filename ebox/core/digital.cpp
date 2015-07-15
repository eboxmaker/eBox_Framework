/*
file   : digital.cpp
author : shentq
version: V1.0
date   : 2015/7/5
brief  : analog read function

Copyright (c) 2015, eBox by shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/#include "common.h"


GPIO::GPIO(GPIO_TypeDef* _port,uint16_t _pin)
{
	port = _port;
	pin = _pin;
	switch((uint32_t)port)
	{
		case (uint32_t)GPIOA:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
			break;

		case (uint32_t)GPIOB:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
			break;

		case (uint32_t)GPIOC:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
			break;

		case (uint32_t)GPIOD:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
			break;

		case (uint32_t)GPIOE:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
			break;
		
		case (uint32_t)GPIOF:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);
			break;
	}
}

void GPIO::mode(PINMODE modeVal)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	if(modeVal == AF_OD || modeVal == AF_PP)
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //
	
	GPIO_InitStructure.GPIO_Pin = pin;
	GPIO_InitStructure.GPIO_Mode = (GPIOMode_TypeDef)modeVal;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(port, &GPIO_InitStructure);   //³õÊ¼»¯GPIOC¶Ë¿Ú
}

void GPIO::set()
{
		port->BSRR = pin;
}
void GPIO::reset()
{
		port->BRR = pin;
}

void GPIO::write(uint8_t val)
{
	if(val == 0)
		port->BRR = pin;
	else
		port->BSRR = pin;
}

void GPIO::read(uint8_t* val)
{
	*val = port->IDR & pin;
}		

uint8_t GPIO::read(void)
{
	if(port->IDR & pin)
		return 1;
	return  0;
}

uint8_t shiftIn(GPIO* dataPin, GPIO* clockPin, uint8_t bitOrder) {
	uint8_t value = 0;
	uint8_t i;

	for (i = 0; i < 8; ++i) {
		clockPin->write(HIGH);
		if (bitOrder == LSBFIRST)
			value |= dataPin->read() << i;
		else
			value |= dataPin->read() << (7 - i);
		clockPin->write(LOW);
	}
	return value;
}

void shiftOut(GPIO* dataPin, GPIO* clockPin, uint8_t bitOrder, uint8_t val)
{
	int i;
	for (i = 0; i < 8; i++)  
	{
		if (bitOrder == LSBFIRST)
			dataPin->write(!!(val & (1 << i)));
		else	
			dataPin->write(!!(val & (1 << (7 - i))));
			clockPin->write(HIGH);
			clockPin->write(LOW);

	}
}

