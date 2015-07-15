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
uint8_t SOFTSPI::transfer(uint8_t data)
{
	uint8_t i;
	uint8_t RcvData = 0 ;
	switch(mode)
	{
		case SPI_MODE0:
			//ʱ�ӿ��������0��
			//��һ���������أ���ȡ���ݣ�
			//�ڶ������½��أ�������ݣ�
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
				break;
		case SPI_MODE1:
			//ʱ�ӿ��������0��
			//��һ���������أ�������ݣ�
			//�ڶ������½��أ���ȡ���ݣ�
				for (i = 0; i < 8; i++)  {
					///////////////////���������//////////
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
					/////////////////�½��ز���////////////
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
				break;

		case SPI_MODE2:
			//ʱ�ӿ��������1��
			//��һ�����½��أ���ȡ���ݣ�
			//�ڶ����������أ�������ݣ�
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
				break;
				
		case SPI_MODE3:
			//ʱ�ӿ��������1��
			//��һ�����½��أ�������ݣ�
			//�ڶ����������أ���ȡ���ݣ�
				for (i = 0; i < 8; i++)  {
					///////////////////�½��������
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
					/////////////////�����ز���////////////
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
				break;
		}
				return RcvData;
}








