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
	1.֧��I2C1��I2C2
	2.��ʱ��֧��remap�������ܿ�����
*/

class I2C
{

	public:
		I2C(I2C_TypeDef* I2Cx,GPIO* SCLPin,GPIO* SDAPin);
		void			begin(uint32_t speed);
		void 			config(uint32_t speed);
		uint32_t	read_config();

		int8_t	write_byte(uint8_t slaveAddress,uint8_t regAddress,uint8_t data);
		int8_t	write_byte(uint8_t slaveAddress,uint8_t regAddress,uint8_t* data,uint16_t numToWrite);
		int8_t	read_byte (uint8_t slaveAddress,uint8_t regAddress,uint8_t* data);
		int8_t	read_byte (uint8_t slaveAddress,uint8_t regAddress,uint8_t* data,uint16_t numToRead);
	  int8_t	wait_dev_busy(uint8_t slaveAddress);
	public:
		int8_t take_i2c_right(uint32_t speed);
		int8_t release_i2c_right(void);

	private:
		int8_t start();
		int8_t stop();
		int8_t send_no_ack();
		int8_t send_ack();

		int8_t send_byte(uint8_t regData);
		int8_t send_7bits_address(uint8_t slaveAddress);
		int8_t receive_byte(uint8_t* data);

	private:
		I2C_TypeDef* _I2Cx;
		GPIO* sda_pin;
		GPIO* scl_pin;
		uint32_t _speed;
		uint8_t busy;

};




/*
	1.֧���κ�IO���ţ�
	2.�����ӿں�Ӳ��I2C��ȫһ�����Ի������
	ע�⣺
				1.�����speed����delay_us��ʱ�������ơ����в�׼
				2.speed����ֻ��Ϊ100000��200000,300k,400k��������Ǵ�ֵ����Ὣspeed��ֱֵ�Ӵ��ݸ�delay_us.��delay_us(speed);
				3.���ڵ���I2C�豸����ʹ��100k�����ߴ���10��ֵ
*/
class SOFTI2C 
{
  public:
		SOFTI2C(GPIO* SCLpin, GPIO* SDApin);
		void 		  begin(uint32_t _speed);
		int8_t 		config(uint32_t _speed);
		uint32_t	read_config();
		int8_t		write_byte(uint8_t slaveAddress,uint8_t regAddress,uint8_t data);
		int8_t 		write_byte(uint8_t slaveAddress,uint8_t regAddress,uint8_t* data,uint16_t numToWrite);
		int8_t 		read_byte (uint8_t slaveAddress,uint8_t regAddress,uint8_t* data);
		int8_t 		read_byte (uint8_t slaveAddress,uint8_t regAddress,uint8_t* data,uint16_t numToRead);
	  int8_t		wait_busy(uint8_t slaveAddress);
	public:
		int8_t take_i2c_right(uint32_t speed);
		int8_t release_i2c_right(void);
	


	private:
		void 		start();
		void 		stop();
		int8_t 	send_ack();
		int8_t 	send_no_ack();
	
		int8_t 	send_byte(uint8_t Byte);
		int8_t	send_7bits_address(uint8_t slaveAddress);
		uint8_t receive_byte();
	
		int8_t	wait_ack();
	
  private:
		GPIO* 		sda_pin;
		GPIO* 		scl_pin;
		uint32_t	speed;
		uint16_t	delay_times;
		uint8_t 	busy;
};

#endif
