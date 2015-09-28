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
uint8_t SOFTSPI::currentDevNum = 0;

SOFTSPI::SOFTSPI(GPIO* SCKPin,GPIO* MISOPin,GPIO* MOSIPin)
{
	sckPin =  SCKPin;
	misoPin = MISOPin;
	mosiPin = MOSIPin;
	
}
void SOFTSPI::begin(SPICONFIG* spiConfig)
{

	sckPin->mode(OUTPUT_PP);
	misoPin->mode(INPUT);
	mosiPin->mode(OUTPUT_PP);
	
	config(spiConfig);
	switch(mode)
	{
		case SPI_MODE0:
				sckPin->reset();
				break;
		case SPI_MODE1:
				sckPin->reset();		
				break;
		case SPI_MODE2:
				sckPin->set();		
				break;
		case SPI_MODE3:
				sckPin->set();		
				break;			
	}
}
void SOFTSPI::config(SPICONFIG* spiConfig)
{
	currentDevNum = spiConfig->devNum;
	mode = spiConfig->mode;
	bitOrder = spiConfig->bitOrder;
	switch(spiConfig->prescaler)
	{
		case SPI_BaudRatePrescaler_2:
			spidelay = 0;
			break;
		case SPI_BaudRatePrescaler_4:
			spidelay = 1;
			break;
		case SPI_BaudRatePrescaler_8:
			spidelay = 2;
			break;
		case SPI_BaudRatePrescaler_16:
			spidelay = 4;
			break;
		case SPI_BaudRatePrescaler_32:
			spidelay = 8;
			break;
		case SPI_BaudRatePrescaler_64:
			spidelay = 16;
			break;
		case SPI_BaudRatePrescaler_128:
			spidelay = 32;
			break;
		case SPI_BaudRatePrescaler_256:
			spidelay = 64;
			break;
		default:
			spidelay = spiConfig->prescaler;
			break;
	}
}
uint8_t SOFTSPI::readConfig(void)
{
	return currentDevNum; 
}

uint8_t SOFTSPI::transfer0(uint8_t data)
{
	uint8_t i;
	uint8_t RcvData = 0 ;

	//时钟空闲输出：0；
	//第一个是上升沿：读取数据；
	//第二个是下降沿：输出数据；
	for (i = 0; i < 8; i++)  {
		if (bitOrder == SPI_BITODER_LSB)
		{
			RcvData |= misoPin->read()<<i;
			mosiPin->write(!!(data & (1 << i)));
		}
		else
		{				
			RcvData |= (misoPin->read()<<(7-i));
			mosiPin->write(!!(data & (1 << (7 - i))));
		}
		delay_us(spidelay);
		sckPin->set();
		delay_us(spidelay);
		sckPin->reset();
	}
	return RcvData;
}
uint8_t SOFTSPI::transfer1(uint8_t data)
{
	uint8_t i;
	uint8_t RcvData = 0 ;

	//时钟空闲输出：0；
	//第一个是上升沿：输出数据；
	//第二个是下降沿：读取数据；
	for (i = 0; i < 8; i++)  {
		///////////////////上升沿输出//////////
		if (bitOrder == SPI_BITODER_LSB)
		{
			mosiPin->write(!!(data & (1 << i)));
		}
		else
		{				
			RcvData |= (misoPin->read()<<(7-i));
			mosiPin->write(!!(data & (1 << (7 - i))));
		}
		delay_us(spidelay);
		sckPin->set();
		/////////////////下降沿采样////////////
		delay_us(spidelay);
		sckPin->reset();
		if (bitOrder == LSBFIRST)
		{
			mosiPin->write(!!(data & (1 << i)));
		}
		else
		{				
			RcvData |= (misoPin->read()<<(7-i));
			mosiPin->write(!!(data & (1 << (7 - i))));
		}	
	}

	return RcvData;
}
uint8_t SOFTSPI::transfer2(uint8_t data)
{
	uint8_t i;
	uint8_t RcvData = 0 ;

	//时钟空闲输出：1；
	//第一个是下降沿：读取数据；
	//第二个是上升沿：输出数据；
	for (i = 0; i < 8; i++)  {
		sckPin->reset();
		delay_us(spidelay);
		if (bitOrder == SPI_BITODER_LSB)
		{
			RcvData |= misoPin->read()<<i;
			mosiPin->write(!!(data & (1 << i)));
		}
		else
		{				
			RcvData |= (misoPin->read()<<(7-i));
			mosiPin->write(!!(data & (1 << (7 - i))));
		}
		delay_us(spidelay);
		sckPin->set();
	}

	return RcvData;
}
uint8_t SOFTSPI::transfer3(uint8_t data)
{
	uint8_t i;
	uint8_t RcvData = 0 ;

	//时钟空闲输出：1；
	//第一个是下降沿：输出数据；
	//第二个是上升沿：读取数据；
	for (i = 0; i < 8; i++)  {
		///////////////////下降沿沿输出
		sckPin->reset();
		delay_us(spidelay);
		if (bitOrder == SPI_BITODER_LSB)
		{
			mosiPin->write(!!(data & (1 << i)));
		}
		else
		{				
			RcvData |= (misoPin->read()<<(7-i));
			mosiPin->write(!!(data & (1 << (7 - i))));
		}
		/////////////////上升沿采样////////////
		sckPin->set();
		delay_us(spidelay);
		if (bitOrder == LSBFIRST)
		{
			mosiPin->write(!!(data & (1 << i)));
		}
		else
		{				
			RcvData |= (misoPin->read()<<(7-i));
			mosiPin->write(!!(data & (1 << (7 - i))));
		}	
	}
		
	return RcvData;
}
uint8_t SOFTSPI::transfer(uint8_t data)
{
	uint8_t i;
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
			return 0xff;
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
int8_t  SOFTSPI::read(uint8_t dummyByte,uint8_t *rcvdata,uint16_t dataln)
{
	if(dataln == 0)
		return -1;
	while(dataln--)
	{
		*rcvdata++ = transfer(0xff);
	}
	return 0;
}






