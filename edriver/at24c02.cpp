/*
file   : at24c02.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/

#include "at24c02.h"

void EEPROM::begin(uint32_t _speed)
{
	speed = _speed;
	i2c->begin(speed);
}
int8_t EEPROM::write_byte(uint16_t byteAddr,uint8_t byte)
{
	i2c->take_i2c_right(speed);
	i2c->write_byte(SLAVE_ADDR,byteAddr,byte);
	i2c->release_i2c_right();
	return 0;
}

int8_t EEPROM::write_byte(uint16_t byteAddr,uint8_t* buf,uint16_t numToWrite)
{
	int8_t ret = 0;

	for(uint16_t i = 0; i < numToWrite; i++)
	{
			write_byte(byteAddr++,buf[i]);
		  //ret = i2c->waitBusy(SLAVE_ADDR);//如果有错误再加此句
	
	}

//	i2c->writeByte(SLAVE_ADDR,byteAddr,buf,numToWrite);//部分器件个别的位置相应比较慢
	return ret;
}

uint8_t EEPROM::read_byte(uint16_t byteAddr)
{
	uint8_t byte;	
	i2c->take_i2c_right(speed);
	i2c->read_byte(SLAVE_ADDR,byteAddr,&byte);
	i2c->release_i2c_right();
	
	return byte;
}

int8_t	EEPROM::read_byte(uint16_t byteAddr,uint8_t* buf,uint16_t numToRead)
{
	int8_t ret = 0;
	i2c->take_i2c_right(speed);

//	for(uint16_t i = 0; i < numToRead; i++)
//	{
//			buf[i] = byteRead(byteAddr++);
//	}
	ret = i2c->read_byte(SLAVE_ADDR,byteAddr,buf,numToRead);
	i2c->release_i2c_right();
	if(ret != 0)
		uart1.printf("\r\nerr = %d\r\n",ret);
	return 0;
}




int8_t EEPROM::write_page(uint8_t* buf,uint32_t addr,uint8_t numToWrite)
{

	return 0;
}


