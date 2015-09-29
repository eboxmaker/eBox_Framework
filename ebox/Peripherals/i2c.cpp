/*
file   : i2c.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/

#include "i2c.h"


I2C::I2C(I2C_TypeDef* I2Cx,GPIO* SCLPin,GPIO* SDAPin)
{
	_I2Cx = I2Cx;
	scl_pin = SCLPin;
	sda_pin = SDAPin;
	
}
void  I2C::begin(uint32_t speed)
{
	_speed = speed;
	I2C_InitTypeDef I2C_InitStructure; 

	if(_I2Cx == I2C1)
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE); 
	else if(_I2Cx == I2C2)
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,ENABLE); 
//	else if(_I2Cx == I2C3)
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C3,ENABLE); 
		
	
	sda_pin->mode(AF_OD);
	scl_pin->mode(AF_OD);
	
	  /* I2C 配置 */
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C ; 
	//I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2; 
	//I2C_InitStructure.I2C_OwnAddress1 = SlaveAddress; 
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable; 
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; 
	I2C_InitStructure.I2C_ClockSpeed = _speed; 

	/* I2C1 初始化 */
	I2C_Init(_I2Cx, &I2C_InitStructure);	   
	
	/* 使能 I2C1 */
	I2C_Cmd  (_I2Cx,ENABLE); 

}

void I2C::set_speed(uint32_t speed)
{
	_speed = speed;
	I2C_InitTypeDef I2C_InitStructure; 
	
	  /* I2C 配置 */
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C ; 
	//I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2; 
	//I2C_InitStructure.I2C_OwnAddress1 = SlaveAddress; 
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable; 
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; 

	I2C_InitStructure.I2C_ClockSpeed = _speed; 
	I2C_Init(_I2Cx, &I2C_InitStructure);	   
	/* 使能 I2C1 */
	I2C_Cmd  (_I2Cx,ENABLE); 
	/*允许应答模式*/
	I2C_AcknowledgeConfig(_I2Cx, ENABLE);   

}
uint32_t I2C::read_config()
{
	return _speed;
}
int8_t I2C::start()
{
	uint16_t times=1000;
	int8_t err = 0;
	I2C_GenerateSTART(_I2Cx,ENABLE);

	while(!I2C_CheckEvent(_I2Cx,I2C_EVENT_MASTER_MODE_SELECT))
	{
		times--;
		if(times == 0)
		{
			err = -1;
			break;
		}
	}
	return err;
}
int8_t I2C::stop()
{
	int8_t err = 0;
	I2C_GenerateSTOP(_I2Cx,ENABLE);
	return err;
}
int8_t I2C::send_no_ack()
{
	int8_t err = 0;
	I2C_AcknowledgeConfig(_I2Cx,DISABLE);
	return err;
}
int8_t I2C::send_ack()
{
	int8_t err = 0;
	I2C_AcknowledgeConfig(_I2Cx,ENABLE);
	return err;
}


int8_t I2C::send_byte(uint8_t data)
{
	uint16_t times = 1000;
	int8_t err = 0;
	I2C_SendData(_I2Cx,data);
	while(!I2C_CheckEvent(_I2Cx,I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		times--;
		if(times == 0)
		{
			err = -2;
			break;
		}
	}
	return err;
}
int8_t I2C::send_7bits_address(uint8_t slaveAddress)
{
	uint16_t times = 1000;
	int8_t err = 0;
	if(slaveAddress&0x01)
	{
		I2C_Send7bitAddress(_I2Cx,slaveAddress,I2C_Direction_Receiver);
		while(!I2C_CheckEvent(_I2Cx,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
		{
			times--;
			if(times == 0)
			{
				err = -3;
				break;
			}
		}
	}
	else
	{
		I2C_Send7bitAddress(_I2Cx,slaveAddress,I2C_Direction_Transmitter);
		while(!I2C_CheckEvent(_I2Cx,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
		{
			times--;
			if(times == 0)
			{
				err = -4;
				break;
			}
		}
	}
	return err;

}
int8_t I2C::receive_byte(uint8_t* data)
{	
	uint16_t times = 1000;
	int8_t err = 0;
	while(!I2C_CheckEvent(_I2Cx,I2C_EVENT_MASTER_BYTE_RECEIVED))
	{
		times--;
		if(times == 0)
		{
			err = -5;
			break;
		}
	}
	*data = I2C_ReceiveData(_I2Cx);//读出寄存器数据
	return err;
}


int8_t I2C::write_byte(uint8_t slaveAddress,uint8_t regAddress,uint8_t data)
{
	uint16_t err = 0;

	start();
	send_7bits_address(slaveAddress);
	send_byte(regAddress);
	send_byte(data);
	stop();

	return err;
	
}
int8_t I2C::write_byte(uint8_t slaveAddress,uint8_t regAddress,uint8_t* data,uint16_t numToRead)
{
	uint16_t err = 0;

	start();
	send_7bits_address(slaveAddress);
	send_byte(regAddress);
	while(numToRead--)
	{
	  send_byte(*data);
		data++;
	}
	stop();

	return err;
	
}
int8_t I2C::read_byte(uint8_t slaveAddress,uint8_t regAddress,uint8_t* data)
{
	start();
	send_7bits_address(slaveAddress);
	I2C_Cmd(_I2Cx,ENABLE);
	send_byte(regAddress);
	start();
	send_7bits_address(slaveAddress + 1);
	send_no_ack();
	stop();
	receive_byte(data);
	send_ack();
	return 0;
}

int8_t I2C::read_byte(uint8_t slaveAddress,uint8_t regAddress,uint8_t* data,uint16_t numToRead)
{
	uint8_t i = 0;
	start();
	send_7bits_address(slaveAddress);
	I2C_Cmd(_I2Cx,ENABLE);
	send_byte(regAddress);
	start();
	send_7bits_address(slaveAddress + 1);
	
	while(numToRead)
	{
		if(numToRead == 1)
		{
			send_no_ack();
			stop();
		}
	  receive_byte(data);
		data++;
		numToRead--;
		i++;
	}
	send_ack();

	return i;
}

int8_t I2C::wait_busy(uint8_t slaveAddress)
{
	int8_t ret;
	uint8_t i = 0;
	do
	{
		start();
		ret = send_7bits_address(slaveAddress);
		send_ack();
		send_byte(slaveAddress);
		stop();
		if(i++==100)
		{
			return -1;
		}
	}while(ret != 0);//如果返回值不是0，继续等待
	return 0;
}
