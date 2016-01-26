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

/**
 *@name     GPIO(GPIO_TypeDef *port,uint16_t pin)
 *@brief    GPIO���캯��
 *@param    port:   GPIOA,B,C,D,E,F,G
 *          pin:    GPIO_Pin_0...15
 *@retval   None
*/
GPIO::GPIO(GPIO_TypeDef *port,uint16_t pin)
{
	this->port = port;
	this->pin = pin;
}
/**
 *@name     void GPIO::mode(PIN_MODE mode)
 *@brief    GPIOģʽ����
 *@param    mode:   PIN_MODEö�ٱ�������
 *@retval   None
*/
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
		case (uint32_t)GPIOG:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);
			break;
	}

	if(mode == AF_OD || mode == AF_PP)
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //
	
	GPIO_InitStructure.GPIO_Pin = this->pin;
	GPIO_InitStructure.GPIO_Mode = (GPIOMode_TypeDef)mode;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(this->port, &GPIO_InitStructure);   //��ʼ��GPIOC�˿�
}
/**
 *@name     void GPIO::set()
 *@brief    GPIO����ߵ�ƽ
 *@param    NONE
 *@retval   NONE
*/
void GPIO::set()
{
    this->port->BSRR = this->pin;
}

/**
 *@name     void GPIO::reset()
 *@brief    GPIO����͵�ƽ
 *@param    NONE
 *@retval   NONE
*/
void GPIO::reset()
{
    this->port->BRR = this->pin;
}


/**
 *@name     void GPIO::write(uint8_t val)
 *@brief    GPIO�������
 *@param    val��1������ߵ�ƽ��0������͵�ƽ
 *@retval   NONE
*/
void GPIO::write(uint8_t val)
{
	if(val == 0)
		this->port->BRR = this->pin;
	else
		this->port->BSRR = this->pin;
}


/**
 *@name     void GPIO::set()
 *@brief    GPIO����ߵ�ƽ
 *@param    NONE
 *@retval   NONE
*/
void GPIO::read(uint8_t *val)
{
	*val = this->port->IDR & this->pin;
}		


/**
 *@name     uint8_t GPIO::read(void)
 *@brief    ��ȡGPIO����״̬
 *@param    NONE
 *@retval   �������ŵ�ǰ�ĵ�ƽ״̬
*/
uint8_t GPIO::read(void)
{
	if(this->port->IDR & this->pin)
		return 1;
	return  0;
}

/**
 *@name     void GPIO::toggle()
 *@brief    GPIO�����ƽ��ת
 *@param    NONE
 *@retval   NONE
*/
void GPIO::toggle()
{
    port->ODR ^= this->pin;
}	

/**
 *@name     void PARALLEL_GPIO::all_mode(PIN_MODE mode)
 *@brief    GPIO��ģʽ����
 *@param    mode:   PIN_MODEö�ٱ�������
 *@retval   None
*/
void PARALLEL_GPIO::all_mode(PIN_MODE mode)
{
    bit[0]->mode(mode);
    bit[1]->mode(mode);
    bit[2]->mode(mode);
    bit[3]->mode(mode);
    bit[4]->mode(mode);
    bit[5]->mode(mode);
    bit[6]->mode(mode);
    bit[7]->mode(mode);
}

/**
 *@name     void PARALLEL_GPIO::write(uint8_t data)
 *@brief    GPIO���������
 *@param    data���������
 *@retval   NONE
*/
void PARALLEL_GPIO::write(uint8_t data)
{
    bit[0]->write(data&(0x01 << 0));
    bit[1]->write(data&(0x01 << 1));
    bit[2]->write(data&(0x01 << 2));
    bit[3]->write(data&(0x01 << 3));
    bit[4]->write(data&(0x01 << 4));
    bit[5]->write(data&(0x01 << 5));
    bit[6]->write(data&(0x01 << 6));
    bit[7]->write(data&(0x01 << 7));
}

/**
 *@name     void PARALLEL_GPIO::write(uint8_t data)
 *@brief    ��ȡGPIO����������
 *@param    NONE
 *@retval   ��ȡGPIO�������
*/
uint8_t PARALLEL_GPIO::read()
{
    uint8_t data = 0;
    data |= *bit[0]<<0;
    data |= *bit[1]<<1;
    data |= *bit[2]<<2;
    data |= *bit[3]<<3;
    data |= *bit[4]<<4;
    data |= *bit[5]<<5;
    data |= *bit[6]<<6;
    data |= *bit[7]<<7;
    return data;
}

/**
 *@name     void PARALLEL_GPIO::write(uint8_t data)
 *@brief    GPIO�����λ�������
 *@param    data����Ҫ��������ݣ�����λ��Ч
 *@retval   NONE
*/
void PARALLEL_GPIO::write_low_4_4bit(uint8_t data)
{
    bit[0]->write(data&(0x01 << 0));
    bit[1]->write(data&(0x01 << 1));
    bit[2]->write(data&(0x01 << 2));
    bit[3]->write(data&(0x01 << 3));
}

/**
 *@name     void PARALLEL_GPIO::write(uint8_t data)
 *@brief    ��ȡGPIO�����λ��������
 *@param    NONE
 *@retval   ��ȡGPIO�����λ�����ݣ�����λΪ0��
*/
uint8_t PARALLEL_GPIO::read_low_4_bit()
{
    uint8_t data = 0;
    data |= *bit[0]<<0;
    data |= *bit[1]<<1;
    data |= *bit[2]<<2;
    data |= *bit[3]<<3;
    return data;
}

/**
 *@name     uint8_t shift_in(GPIO *data_pin, GPIO *clock_pin, uint8_t bit_order) 
 *@brief    GPIO����ʱ���������һ��u8���͵����ݣ����ڴ���ͨ�š�
 *@param    data_pin��  ������������
            clock_pin:  ʱ���źŵ�����
            bit_order:  ���ݴ�С�˿���LSB_FIRST����λ�ȷ��ͣ�MSB_FIRST��λ�ȷ���
 *@retval   ���յ�������
*/
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

/**
 *@name     uint8_t shift_in(GPIO *data_pin, GPIO *clock_pin, uint8_t bit_order) 
 *@brief    GPIO����ʱ���������һ��u8���͵����ݣ����ڴ���ͨ�š�
 *@param    data_pin��  �����������
            clock_pin:  ʱ���źŵ�����
            bit_order:  ���ݴ�С�˿���LSB_FIRST����λ�ȷ��ͣ�MSB_FIRST��λ�ȷ���
            val��       Ҫ���͵�����
 *@retval   NONE
*/
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

