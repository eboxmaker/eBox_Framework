/*
file   : digital.cpp
author : shentq
version: V1.0
date   : 2015/7/5
brief  : analog read function

Copyright 2015 shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/
#include "common.h"


GPIO::GPIO(GPIO_TypeDef *port,uint16_t pin)
{
	this->port = port;
	this->pin = pin;
}

void GPIO::mode(PIN_MODE mode)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  
	switch((uint32_t)this->port)
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

	if(mode == AF_OD || mode == AF_PP)
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //
	
	GPIO_InitStructure.GPIO_Pin = this->pin;
	GPIO_InitStructure.GPIO_Mode = (GPIOMode_TypeDef)mode;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(this->port, &GPIO_InitStructure);   //³õÊ¼»¯GPIOC¶Ë¿Ú
}

void GPIO::set()
{
    this->port->BSRR = this->pin;
}
void GPIO::reset()
{
    this->port->BRR = this->pin;
}

void GPIO::write(uint8_t val)
{
	if(val == 0)
		this->port->BRR = this->pin;
	else
		this->port->BSRR = this->pin;
}

void GPIO::read(uint8_t *val)
{
	*val = this->port->IDR & this->pin;
}		

uint8_t GPIO::read(void)
{
	if(this->port->IDR & this->pin)
		return 1;
	return  0;
}
void GPIO::toggle()
{
    port->ODR ^= this->pin;
}	



uint8_t shift_in(GPIO *data_pin, GPIO *clock_pin, uint8_t bit_order) 
{
	uint8_t value = 0;
	uint8_t i;

	for (i = 0; i < 8; ++i) 
    {
		clock_pin->write(HIGH);
		if (bit_order == LSB_FIRST)
			value |= data_pin->read() << i;
		else
			value |= data_pin->read() << (7 - i);
		clock_pin->write(LOW);
	}
	return value;
}

void shift_out(GPIO *data_pin, GPIO *clock_pin, uint8_t bit_order, uint8_t val)
{
	int i;
	for (i = 0; i < 8; i++)  
	{
		if (bit_order == LSB_FIRST)
			data_pin->write(!!(val & (1 << i)));
		else	
			data_pin->write(!!(val & (1 << (7 - i))));
		
			clock_pin->write(HIGH);
			clock_pin->write(LOW);

	}
}

