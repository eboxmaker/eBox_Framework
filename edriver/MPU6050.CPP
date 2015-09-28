/*
file   : mpu6050.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentqlf@163.com.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentqlf@163.com assumes no responsibility for any errors contained herein.
*/

#include "mpu6050.h"



void MPU6050::begin(uint32_t speed)
{
	i2c->begin(speed);
	i2c->writeByte(SlaveAddress,PWR_MGMT_1,0);
	i2c->writeByte(SlaveAddress,SMPLRT_DIV,0x07);
	i2c->writeByte(SlaveAddress,CONFIG,0x06);
	i2c->writeByte(SlaveAddress,GYRO_CONFIG,0x18);
	i2c->writeByte(SlaveAddress,ACCEL_CONFIG,0x01);
	i2c->writeByte(SlaveAddress,PWR_MGMT_1,0);
	i2c->writeByte(SlaveAddress,SMPLRT_DIV,0x07);
	i2c->writeByte(SlaveAddress,CONFIG,0x06);
	i2c->writeByte(SlaveAddress,GYRO_CONFIG,0x18);
	i2c->writeByte(SlaveAddress,ACCEL_CONFIG,0x01);
}
void MPU6050::getID(uint8_t* id)
{
	i2c->readByte(SlaveAddress,WHO_AM_I,id,1);
};

int16_t  MPU6050::getData(uint8_t regAddress)
{
	uint8_t tmp[2];
	
	i2c->readByte(SlaveAddress,regAddress,tmp,2);

	return ((tmp[0]<<8) + tmp[1]);

}
int8_t  MPU6050::getData(uint8_t regAddress,int16_t* buf,uint8_t numToRead)
{
	uint8_t i,readnum;
	uint8_t tmpbuf[20];
	
	readnum = i2c->readByte(SlaveAddress,regAddress,tmpbuf,numToRead*2);
	for(i=0;i<readnum/2;i++)
	{
		*buf++ = (tmpbuf[i*2 + 0]<<8) + (tmpbuf[i*2 + 1]);
	}
	return readnum/2;
}

