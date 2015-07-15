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
	1.��ʱֻ֧��I2C1
	2.��ʱ��֧��remap�������ܿ�����
	ע�⣺������Ե������౻��������ã�Ӧ���������MPU605�����ӣ�Ҳ���Ե���ʵ���������
*/

class I2C
{

	public:
		I2C(I2C_TypeDef* I2Cx,GPIO* SDAPin,GPIO* SCLPin);
		void 		setSpeed(uint32_t speed);
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
};




/*
	1.֧���κ�IO���ţ�
	ע�⣺1.������Ե������౻��������ã�Ӧ���������MPU605�����ӣ�Ҳ���Ե���ʵ��������á�
				2.�����speed����delay_us��ʱ�������ơ����в�׼
				3.speed����ֻ��Ϊ100000��200000,300k,400k��������Ǵ�ֵ����Ὣspeed��ֱֵ�Ӵ��ݸ�delay_us.��delay_us(speed);
				4.���ڵ���I2C�豸����ʹ��100k�����ߴ���3��ֵ
*/
class SOFTI2C 
{
  public:
		SOFTI2C(GPIO* SDApin, GPIO* SCLpin);
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
