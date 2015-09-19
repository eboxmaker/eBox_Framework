/*
file   : i2c.h
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.

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

class I2C
{

	public:
		I2C(I2C_TypeDef* I2Cx,GPIO* SDAPin,GPIO* SCLPin);
		void 		setSpeed(uint32_t speed);
		uint32_t	readConfig();

		int8_t	writeByte(uint8_t slaveAddress,uint8_t regAddress,uint8_t data);
		int8_t	writeByte(uint8_t slaveAddress,uint8_t regAddress,uint8_t* data,uint16_t numToRead);
		int8_t	readByte (uint8_t slaveAddress,uint8_t regAddress,uint8_t* data);
		int8_t	readByte (uint8_t slaveAddress,uint8_t regAddress,uint8_t* data,uint16_t numToRead);
	protected:
		void   i2cBegin(uint32_t speed);
		int8_t start();
		int8_t stop();
		int8_t sendNoAck();
		int8_t sendAck();

		int8_t sendByte(uint8_t regData);
		int8_t send7BitsAddress(uint8_t address);
		int8_t receiveByte(uint8_t* data);

	private:
		I2C_TypeDef* _I2Cx;
		GPIO* _SDAPin;
		GPIO* _SCLPin;
		uint32_t _speed;
};




/*
	1.支持任何IO引脚；
	注意：1.该类可以当做父类被其他类调用，应用历程详见MPU605的例子，也可以单独实例化后调用。
				2.该类的speed是由delay_us延时函数控制。略有不准
				3.speed设置只能为100000，200000,300k,400k。如果不是此值，则会将speed的值直接传递给delay_us.即delay_us(speed);
				4.初期调试I2C设备建议使用100k。或者大于3的值
*/
class SOFTI2C 
{
  public:
		SOFTI2C(GPIO* SDApin, GPIO* SCLpin);
		int8_t 		setSpeed(uint32_t speed);
		uint32_t	readConfig();
		int8_t		writeByte(uint8_t slaveAddress,uint8_t regAddress,uint8_t data);
		int8_t 		writeByte(uint8_t slaveAddress,uint8_t regAddress,uint8_t* data,uint16_t numToRead);
		int8_t 		readByte (uint8_t slaveAddress,uint8_t regAddress,uint8_t* data);
		int8_t 		readByte (uint8_t slaveAddress,uint8_t regAddress,uint8_t* data,uint16_t numToRead);


  private:
		GPIO* sdaPin;
		GPIO* sclPin;
		uint32_t _speed;
		uint16_t _delayTimes;
	protected:
		void 		i2cBegin(uint32_t speed);
		void 		start();
		void 		stop();
		int8_t	waitAck();
		void 		sendAck();
		void 		sendNoAck();
		void 		sendByte(uint8_t Byte);
		uint8_t receiveByte();
};

#endif
