/*
file   : i2c.h
author : shentq
version: V1.0
date   : 2015/7/5

Copyright (c) 2015, eBox by shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/
#ifndef __I2C_H
#define __I2C_H

#include "common.h"
/*
	1.暂时只支持I2C1
	2.暂时不支持remap，后续很快会完成
	注意：该类可以当做父类被其他类调用，应用历程详见MPU605的例子，也可以单独实例化后调用
*/
//默认参数
#define I2C_SPEED 100000

class I2C
{

	public:
		I2C(uint8_t SDAPin,uint8_t SCLPin)
		{
			_SDAPin = SDAPin;
			_SCLPin = SCLPin;
		}
		void i2cBegin();
		void setSpeed(uint32_t speed);
	
		int8_t writeByte(uint8_t slaveAddress,uint8_t regAddress,uint8_t data);
		int8_t writeByte(uint8_t slaveAddress,uint8_t regAddress,uint8_t* data,uint16_t numToRead);
		int8_t readByte (uint8_t slaveAddress,uint8_t regAddress,uint8_t* data);
		int8_t readByte (uint8_t slaveAddress,uint8_t regAddress,uint8_t* data,uint16_t numToRead);

	private:
		uint8_t _SDAPin;
		uint8_t _SCLPin;
		I2C_InitTypeDef I2C_InitStructure; 
	
		int8_t start();
		int8_t stop();
		int8_t sendNoAck();
		int8_t sendAck();

		int8_t sendByte(uint8_t regData);
		int8_t send7BitsAddress(uint8_t address);
		int8_t receiveByte(uint8_t* data);

	
};

#endif
