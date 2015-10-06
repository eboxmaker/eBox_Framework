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


I2C::I2C(I2C_TypeDef* I2Cx,GPIO* p_scl_pin,GPIO* p_sda_pin)
{
	busy = 0;
	_I2Cx = I2Cx;
	scl_pin = p_scl_pin;
	sda_pin = p_sda_pin;
	
}
void  I2C::begin(uint32_t _speed)
{
	speed = _speed;
	I2C_InitTypeDef I2C_InitStructure; 

	if(_I2Cx == I2C1)
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE); 
	else if(_I2Cx == I2C2)
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,ENABLE); 
//	else if(_I2Cx == I2C3)
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C3,ENABLE); 
		
	
	sda_pin->mode(AF_OD);
	scl_pin->mode(AF_OD);
	
	  /* I2C ���� */
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C ; 
	//I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2; 
	//I2C_InitStructure.I2C_OwnAddress1 = SlaveAddress; 
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable; 
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; 
	I2C_InitStructure.I2C_ClockSpeed = speed; 

	/* I2C1 ��ʼ�� */
	I2C_DeInit(_I2Cx);
	I2C_Init(_I2Cx, &I2C_InitStructure);	   
	
	/* ʹ�� I2C1 */
	I2C_Cmd  (_I2Cx,ENABLE); 

}

void I2C::config(uint32_t _speed)
{
	speed = _speed;
	I2C_InitTypeDef I2C_InitStructure; 
	
	  /* I2C ���� */
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C ; 
	//I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2; 
	//I2C_InitStructure.I2C_OwnAddress1 = SlaveAddress; 
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable; 
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; 

	I2C_InitStructure.I2C_ClockSpeed = speed; 
	I2C_Init(_I2Cx, &I2C_InitStructure);	   
	/* ʹ�� I2C1 */
	I2C_Cmd  (_I2Cx,ENABLE); 
	/*����Ӧ��ģʽ*/
	I2C_AcknowledgeConfig(_I2Cx, ENABLE);   

}
uint32_t I2C::read_config()
{
	return speed;
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
int8_t I2C::send_7bits_address(uint8_t slave_address)
{
	uint16_t times = 5000;
	int8_t err = 0;
	if(slave_address&0x01)
	{
		I2C_Send7bitAddress(_I2Cx,slave_address,I2C_Direction_Receiver);
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
		I2C_Send7bitAddress(_I2Cx,slave_address,I2C_Direction_Transmitter);
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
	*data = I2C_ReceiveData(_I2Cx);//�����Ĵ�������
	return err;
}


int8_t I2C::write_byte(uint8_t slave_address,uint8_t reg_address,uint8_t data)
{
	uint16_t err = 0;

	start();
	send_7bits_address(slave_address);
	send_byte(reg_address);
	send_byte(data);
	stop();

	return err;
	
}
int8_t I2C::write_byte(uint8_t slave_address,uint8_t reg_address,uint8_t* data,uint16_t num_to_write)
{
	uint16_t err = 0;

	start();
	send_7bits_address(slave_address);
	send_byte(reg_address);
	while(num_to_write--)
	{
	  send_byte(*data);
		data++;
	}
	stop();

	return err;
	
}
int8_t I2C::read_byte(uint8_t slave_address,uint8_t reg_address,uint8_t* data)
{
	start();
	send_7bits_address(slave_address);
	I2C_Cmd(_I2Cx,ENABLE);
	send_byte(reg_address);
	start();
	send_7bits_address(slave_address + 1);
	send_no_ack();
	stop();
	receive_byte(data);
	send_ack();
	return 0;
}

int8_t I2C::read_byte(uint8_t slave_address,uint8_t reg_address,uint8_t* data,uint16_t num_to_read)
{
	uint8_t i = 0;
	start();
	send_7bits_address(slave_address);
	I2C_Cmd(_I2Cx,ENABLE);
	send_byte(reg_address);
	start();
	send_7bits_address(slave_address + 1);
	
	while(num_to_read)
	{
		if(num_to_read == 1)
		{
			send_no_ack();
			stop();
		}
	  receive_byte(data);
		data++;
		num_to_read--;
		i++;
	}
	send_ack();

	return i;
}

int8_t I2C::wait_dev_busy(uint8_t slave_address)
{
	int8_t ret;
	uint8_t i = 0;
	do
	{
		start();
		ret = send_7bits_address(slave_address);
		send_ack();
		send_byte(slave_address);
		stop();
		if(i++==100)
		{
			return -1;
		}
	}while(ret != 0);//�������ֵ����0�������ȴ�
	return 0;
}
int8_t I2C::take_i2c_right(uint32_t _speed)
{
	while(busy == 1)
	{
		delay_ms(1);
	}
	speed = _speed;
	config(speed);
	busy = 1;
	return 0;
}
int8_t I2C::release_i2c_right(void)
{
	busy = 0;
	return 0;
}
