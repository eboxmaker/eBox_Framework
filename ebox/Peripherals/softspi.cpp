/*
file   : softspi.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright (c) 2015, eBox by shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/

#include "spi.h"

SOFTSPI::SOFTSPI(GPIO* SCKPin,GPIO* MISOPin,GPIO* MOSIPin)
{
	sck_pin =  SCKPin;
	miso_pin = MISOPin;
	mosi_pin = MOSIPin;
	
}
void SOFTSPI::begin(SPI_CONFIG_TYPE* spiConfig)
{

	sck_pin->mode(OUTPUT_PP);
	miso_pin->mode(INPUT);
	mosi_pin->mode(OUTPUT_PP);
	
	config(spiConfig);
	switch(mode)
	{
		case SPI_MODE0:
				sck_pin->reset();
				break;
		case SPI_MODE1:
				sck_pin->reset();		
				break;
		case SPI_MODE2:
				sck_pin->set();		
				break;
		case SPI_MODE3:
				sck_pin->set();		
				break;			
	}
}
void SOFTSPI::config(SPI_CONFIG_TYPE* spiConfig)
{
	currentDevNum = spiConfig->devNum;
	mode = spiConfig->mode;
	bit_order = spiConfig->bit_order;
	switch(spiConfig->prescaler)
	{
		case SPI_BaudRatePrescaler_2:
			spi_delay = 0;
			break;
		case SPI_BaudRatePrescaler_4:
			spi_delay = 1;
			break;
		case SPI_BaudRatePrescaler_8:
			spi_delay = 2;
			break;
		case SPI_BaudRatePrescaler_16:
			spi_delay = 4;
			break;
		case SPI_BaudRatePrescaler_32:
			spi_delay = 8;
			break;
		case SPI_BaudRatePrescaler_64:
			spi_delay = 16;
			break;
		case SPI_BaudRatePrescaler_128:
			spi_delay = 32;
			break;
		case SPI_BaudRatePrescaler_256:
			spi_delay = 64;
			break;
		default:
			spi_delay = spiConfig->prescaler;
			break;
	}
}
uint8_t SOFTSPI::read_config(void)
{
	return currentDevNum; 
}

uint8_t SOFTSPI::transfer0(uint8_t data)
{
	uint8_t i;
	uint8_t RcvData = 0 ;

	//ʱ�ӿ��������0��
	//��һ���������أ���ȡ���ݣ�
	//�ڶ������½��أ�������ݣ�
	for (i = 0; i < 8; i++)  {
		if (bit_order == SPI_BITODER_LSB)
		{
			RcvData |= miso_pin->read()<<i;
			mosi_pin->write(!!(data & (1 << i)));
		}
		else
		{				
			RcvData |= (miso_pin->read()<<(7-i));
			mosi_pin->write(!!(data & (1 << (7 - i))));
		}
		delay_us(spi_delay);
		sck_pin->set();
		delay_us(spi_delay);
		sck_pin->reset();
	}
	return RcvData;
}
uint8_t SOFTSPI::transfer1(uint8_t data)
{
	uint8_t i;
	uint8_t RcvData = 0 ;

	//ʱ�ӿ��������0��
	//��һ���������أ�������ݣ�
	//�ڶ������½��أ���ȡ���ݣ�
	for (i = 0; i < 8; i++)  {
		///////////////////���������//////////
		if (bit_order == SPI_BITODER_LSB)
		{
			mosi_pin->write(!!(data & (1 << i)));
		}
		else
		{				
			RcvData |= (miso_pin->read()<<(7-i));
			mosi_pin->write(!!(data & (1 << (7 - i))));
		}
		delay_us(spi_delay);
		sck_pin->set();
		/////////////////�½��ز���////////////
		delay_us(spi_delay);
		sck_pin->reset();
		if (bit_order == LSB_FIRST)
		{
			mosi_pin->write(!!(data & (1 << i)));
		}
		else
		{				
			RcvData |= (miso_pin->read()<<(7-i));
			mosi_pin->write(!!(data & (1 << (7 - i))));
		}	
	}

	return RcvData;
}
uint8_t SOFTSPI::transfer2(uint8_t data)
{
	uint8_t i;
	uint8_t RcvData = 0 ;

	//ʱ�ӿ��������1��
	//��һ�����½��أ���ȡ���ݣ�
	//�ڶ����������أ�������ݣ�
	for (i = 0; i < 8; i++)  {
		sck_pin->reset();
		delay_us(spi_delay);
		if (bit_order == SPI_BITODER_LSB)
		{
			RcvData |= miso_pin->read()<<i;
			mosi_pin->write(!!(data & (1 << i)));
		}
		else
		{				
			RcvData |= (miso_pin->read()<<(7-i));
			mosi_pin->write(!!(data & (1 << (7 - i))));
		}
		delay_us(spi_delay);
		sck_pin->set();
	}

	return RcvData;
}
uint8_t SOFTSPI::transfer3(uint8_t data)
{
	uint8_t i;
	uint8_t RcvData = 0 ;

	//ʱ�ӿ��������1��
	//��һ�����½��أ�������ݣ�
	//�ڶ����������أ���ȡ���ݣ�
	for (i = 0; i < 8; i++)  {
		///////////////////�½��������
		sck_pin->reset();
		delay_us(spi_delay);
		if (bit_order == SPI_BITODER_LSB)
		{
			mosi_pin->write(!!(data & (1 << i)));
		}
		else
		{				
			RcvData |= (miso_pin->read()<<(7-i));
			mosi_pin->write(!!(data & (1 << (7 - i))));
		}
		/////////////////�����ز���////////////
		sck_pin->set();
		delay_us(spi_delay);
		if (bit_order == LSB_FIRST)
		{
			mosi_pin->write(!!(data & (1 << i)));
		}
		else
		{				
			RcvData |= (miso_pin->read()<<(7-i));
			mosi_pin->write(!!(data & (1 << (7 - i))));
		}	
	}
		
	return RcvData;
}
uint8_t SOFTSPI::transfer(uint8_t data)
{
	uint8_t RcvData = 0 ;
	switch(mode)
	{
		case SPI_MODE0:
			RcvData = transfer0(data);
		break;
		case SPI_MODE1:
			RcvData = transfer1(data);
		break;
		case SPI_MODE2:
			RcvData = transfer2(data);
		break;
		case SPI_MODE3:
			RcvData = transfer3(data);
		break;
		default :
			//return 0xff;
			break;
	}
	return RcvData;

}

int8_t  SOFTSPI::write(uint8_t data)
{
	transfer(data);
	return 0;
}
int8_t  SOFTSPI::write(uint8_t *data,uint16_t dataln)
{
	if(dataln == 0)
		return -1;
	while(dataln--)
	{
		transfer(*data++);
	}
	return 0;
}

uint8_t SOFTSPI::read()
{
	return transfer(0xff);
}
int8_t  SOFTSPI::read(uint8_t* data)
{
	*data = transfer(0xff);
return 0;

}
int8_t  SOFTSPI::read(uint8_t *rcvdata,uint16_t dataln)
{
	if(dataln == 0)
		return -1;
	while(dataln--)
	{
		*rcvdata++ = transfer(0xff);
	}
	return 0;
}

int8_t SOFTSPI::take_spi_right(SPI_CONFIG_TYPE* spi_config)
{
	while((busy == 1)&&(spi_config->devNum != read_config()))
		delay_ms(1);
	if(spi_config->devNum == read_config())
	{
		busy = 1;
		return 0;
	}
	config(spi_config);
	busy = 1;
	return 0;
}
int8_t SOFTSPI::release_spi_right(void)
{
	busy = 0;
	return 0;
}




