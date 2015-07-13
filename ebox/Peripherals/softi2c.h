/*
file   : softi2c.h
author : shentq
version: V1.0
date   : 2015/7/5

Copyright (c) 2015, eBox by shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/
#ifndef __SOFTI2c_H
#define __SOFTI2c_H

#include "common.h"

/*
	1.支持任何IO引脚；
	注意：1.该类可以当做父类被其他类调用，应用历程详见MPU605的例子，也可以单独实例化后调用。
				2.该类的speed是由delay_us延时函数控制。略有不准
				3.speed设置只能为100k，200k,300k,400k。如果不是此值，则会将speed的值直接传递给delay_us.即delay_us(speed);
				4.初期调试I2C设备建议使用100k。或者大于3的值
*/

//默认参数
#define SOFT_I2C_SPEED 100000

class Softi2c 
{

  public:
		Softi2c(GPIO* SDApin, GPIO* SCLpin);
		int8_t 		setSpeed(uint32_t speed);
		int8_t		writeByte(uint8_t slaveAddress,uint8_t regAddress,uint8_t data);
		int8_t 		writeByte(uint8_t slaveAddress,uint8_t regAddress,uint8_t* data,uint16_t numToRead);
		int8_t 		readByte (uint8_t slaveAddress,uint8_t regAddress,uint8_t* data);
		int8_t 		readByte (uint8_t slaveAddress,uint8_t regAddress,uint8_t* data,uint16_t numToRead);


  private:
		GPIO* sdaPin;
		GPIO* sclPin;
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
