/*
file   : i2c.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright (c) 2015, eBox by shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/

#include "i2c.h"

#define DEBUG 1

//#define I2C_NUM 2
I2C::I2C(I2C_TypeDef* I2Cx,GPIO* SDAPin,GPIO* SCLPin)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);  //如果外部引脚没有上拉，必须加这一句
	SDAPin->mode(AF_OD);
	SCLPin->mode(AF_OD);
}
void  I2C::i2cBegin(uint32_t speed)
{
	_speed = speed;
	I2C_InitTypeDef I2C_InitStructure; 

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);  
	
	
	  /* I2C 配置 */
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C ; 
	//I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2; 
	//I2C_InitStructure.I2C_OwnAddress1 = SlaveAddress; 
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable; 
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; 
	I2C_InitStructure.I2C_ClockSpeed = _speed; 

	/* I2C1 初始化 */
	I2C_Init(I2C1, &I2C_InitStructure);	   
	
	/* 使能 I2C1 */
	I2C_Cmd  (I2C1,ENABLE); 

}

void I2C::setSpeed(uint32_t speed)
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
	I2C_Init(I2C1, &I2C_InitStructure);	   
	/* 使能 I2C1 */
	I2C_Cmd  (I2C1,ENABLE); 
	/*允许应答模式*/
	I2C_AcknowledgeConfig(I2C1, ENABLE);   

}
uint32_t I2C::readConfig()
{
	return _speed;
}
int8_t I2C::start()
{
	uint16_t times=1000;
	int8_t err = 0;
	I2C_GenerateSTART(I2C1,ENABLE);

	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT))
	{
		times--;
		if(times == 0)
		{
			err = -1;
			#if  DEBUG
				//uart3.printf("err = %d",err);
			#endif
			break;
		}
	}
	return err;
}
int8_t I2C::stop()
{
	int8_t err = 0;
	I2C_GenerateSTOP(I2C1,ENABLE);
	return err;
}
int8_t I2C::sendNoAck()
{
	int8_t err = 0;
	I2C_AcknowledgeConfig(I2C1,DISABLE);
	return err;
}
int8_t I2C::sendAck()
{
	int8_t err = 0;
	I2C_AcknowledgeConfig(I2C1,ENABLE);
	return err;
}


int8_t I2C::sendByte(uint8_t data)
{
	uint16_t times = 1000;
	int8_t err = 0;
	I2C_SendData(I2C1,data);
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED))
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
int8_t I2C::send7BitsAddress(uint8_t slaveAddress)
{
	uint16_t times = 1000;
	int8_t err;
	if(slaveAddress&0x01)
	{
		I2C_Send7bitAddress(I2C1,slaveAddress,I2C_Direction_Receiver);
		while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
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
		I2C_Send7bitAddress(I2C1,slaveAddress,I2C_Direction_Transmitter);
		while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
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
int8_t I2C::receiveByte(uint8_t* data)
{	
	uint16_t times = 1000;
	int8_t err;
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED))
	{
		times--;
		if(times == 0)
		{
			err = -5;
			break;
		}
	}
	*data = I2C_ReceiveData(I2C1);//读出寄存器数据
	return err;
}


int8_t I2C::writeByte(uint8_t slaveAddress,uint8_t regAddress,uint8_t data)
{
	uint16_t err;

	start();
	send7BitsAddress(slaveAddress);
	sendByte(regAddress);
	sendByte(data);
	stop();

	return err;
	
}
int8_t I2C::writeByte(uint8_t slaveAddress,uint8_t regAddress,uint8_t* data,uint16_t numToRead)
{
	uint16_t err;

	start();
	send7BitsAddress(slaveAddress);
	sendByte(regAddress);
	while(numToRead--)
	{
	  sendByte(*data);
		data++;
	}
	stop();

	return err;
	
}
int8_t I2C::readByte(uint8_t slaveAddress,uint8_t regAddress,uint8_t* data)
{
	start();
	send7BitsAddress(slaveAddress);
	I2C_Cmd(I2C1,ENABLE);
	sendByte(regAddress);
	start();
	send7BitsAddress(slaveAddress + 1);
	sendNoAck();
	stop();
	receiveByte(data);
	sendAck();
	return 0;
}

int8_t I2C::readByte(uint8_t slaveAddress,uint8_t regAddress,uint8_t* data,uint16_t numToRead)
{
	uint8_t i;
	start();
	send7BitsAddress(slaveAddress);
	I2C_Cmd(I2C1,ENABLE);
	sendByte(regAddress);
	start();
	send7BitsAddress(slaveAddress + 1);
	
	while(numToRead)
	{
		if(numToRead == 1)
		{
			sendNoAck();
			stop();
		}
	  receiveByte(data);
		data++;
		numToRead--;
		i++;
	}
	sendAck();

	return i;
}

