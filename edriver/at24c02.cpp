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
int8_t EEPROM::write_byte(uint16_t byte_addr,uint8_t byte)
{
	i2c->take_i2c_right(speed);
	i2c->write_byte(SLAVE_ADDR,byte_addr,byte);
	i2c->release_i2c_right();
	return 0;
}

int8_t EEPROM::write_byte(uint16_t byte_addr,uint8_t* buf,uint16_t num_to_write)
{
	int8_t ret = 0;

	for(uint16_t i = 0; i < num_to_write; i++)
	{
			write_byte(byte_addr++,buf[i]);
		  //ret = i2c->wait_dev_busy(SLAVE_ADDR);//����д����ټӴ˾�
	
	}

//	i2c->writeByte(SLAVE_ADDR,byteAddr,buf,numToWrite);//�������������λ����Ӧ�Ƚ���
	return ret;
}

uint8_t EEPROM::read_byte(uint16_t byte_addr)
{
	uint8_t byte;	
	i2c->take_i2c_right(speed);
	i2c->read_byte(SLAVE_ADDR,byte_addr,&byte);
	i2c->release_i2c_right();
	
	return byte;
}

int8_t	EEPROM::read_byte(uint16_t byte_addr,uint8_t* buf,uint16_t num_to_read)
{
	int8_t ret = 0;
	i2c->take_i2c_right(speed);

//	for(uint16_t i = 0; i < numToRead; i++)
//	{
//			buf[i] = byteRead(byteAddr++);
//	}
	ret = i2c->read_byte(SLAVE_ADDR,byte_addr,buf,num_to_read);
	i2c->release_i2c_right();
	if(ret != 0)
		uart1.printf("\r\nerr = %d\r\n",ret);
	return 0;
}

int8_t EEPROM::write_page(uint8_t* buf,uint32_t addr,uint8_t numToWrite)
{

	return 0;
}


