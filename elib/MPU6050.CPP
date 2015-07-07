/*
file   : mpu6050.c
author : shentq
version: V1.0
date   : 2015/7/5

Copyright (c) 2015, eBox by shentqlf@163.com. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentqlf@163.com.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentqlf@163.com assumes no responsibility for any errors contained herein.
*/

#include "mpu6050.h"



void MPU6050::begin()
{
	uint8_t table[] = {0x00,0x07,0x06,0x18,0x01};
	i2cBegin();
//	writeByte(SlaveAddress,PWR_MGMT_1,0);
//	writeByte(SlaveAddress,SMPLRT_DIV,0x07);
//	writeByte(SlaveAddress,CONFIG,0x06);
//	writeByte(SlaveAddress,GYRO_CONFIG,0x18);
//	writeByte(SlaveAddress,ACCEL_CONFIG,0x01);
	writeByte(SlaveAddress,PWR_MGMT_1,&table[0],1);
	writeByte(SlaveAddress,SMPLRT_DIV,&table[1],1);
	writeByte(SlaveAddress,CONFIG,&table[2],1);
	writeByte(SlaveAddress,GYRO_CONFIG,&table[3],1);
	writeByte(SlaveAddress,ACCEL_CONFIG,&table[4],1);

}
void MPU6050::getID(uint8_t* id)
{
	readByte(SlaveAddress,WHO_AM_I,id,1);
};

int16_t  MPU6050::getData(uint8_t regAddress)
{
	uint8_t tmp[2];
	
	readByte(SlaveAddress,regAddress,tmp,2);

	return ((tmp[0]<<8) + tmp[1]);

}
int8_t  MPU6050::getData(uint8_t regAddress,int16_t* buf,uint8_t numToRead)
{
	uint8_t i,readnum;
	uint8_t tmpbuf[20];
	
	readnum = readByte(SlaveAddress,regAddress,tmpbuf,numToRead*2);
	for(i=0;i<readnum/2;i++)
	{
		*buf++ = (tmpbuf[i*2 + 0]<<8) + (tmpbuf[i*2 + 1]);
	}
	return readnum/2;
}

