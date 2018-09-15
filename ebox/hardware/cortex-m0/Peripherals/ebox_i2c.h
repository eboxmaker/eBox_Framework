/**
  ******************************************************************************
  * @file    i2c.h
  * @author  shentq
  * @version V2.1
  * @date    2016/08/14
  * @brief   
  ******************************************************************************
  * @attention
  *
  * No part of this software may be used for any commercial activities by any form 
  * or means, without the prior written consent of shentq. This specification is 
  * preliminary and is subject to change at any time without notice. shentq assumes
  * no responsibility for any errors contained herein.
  * <h2><center>&copy; Copyright 2015 shentq. All Rights Reserved.</center></h2>
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __I2C_H
#define __I2C_H

#include "ebox_core.h"
#include "mcu.h"

/*
	1.֧��I2C1��I2C2
	2.��ʱ��֧��remap�������ܿ�����
*/

class mcuI2c :public I2c
{

public:
    mcuI2c(I2C_TypeDef *I2Cx, Gpio *scl_pin, Gpio *sda_pin);
    virtual void		begin(uint32_t _speed);
    virtual void        config(uint32_t speed);
    virtual uint32_t    read_config();

    int8_t	write_byte(uint8_t slave_address, uint8_t data);
    virtual int8_t	write_byte(uint8_t slave_address, uint8_t reg_address, uint8_t data);
    virtual int8_t	write_byte(uint8_t slave_address, uint8_t reg_address, uint8_t *data, uint16_t len);
    virtual int8_t	read_byte (uint8_t slave_address, uint8_t reg_address, uint8_t *data);
    virtual int8_t	read_byte (uint8_t slave_address, uint8_t reg_address, uint8_t *data, uint16_t len);
    virtual int8_t	wait_dev_busy(uint8_t slave_address);
    int8_t	wait_dev_busy1(uint8_t slave_address,uint16_t timeout_us = 300);
public:
    virtual int8_t take_i2c_right(uint32_t speed);
    virtual int8_t release_i2c_right(void);

private:
    virtual int8_t start();
    virtual int8_t stop();
    virtual int8_t send_ack();
    virtual int8_t send_no_ack();

    virtual int8_t send_byte(uint8_t regData);
    virtual int8_t send_7bits_address(uint8_t slave_address);
    virtual int8_t receive_byte(uint8_t *data);
    virtual int8_t	wait_ack(){return 0;};//��Ӳ������ɡ��Ѿ������ڸ��������ڲ���

private:
    I2C_TypeDef *I2Cx;
    Gpio        *sda_pin;
    Gpio        *scl_pin;
    uint32_t    speed;
    uint8_t     busy;
};


/*
	1.֧���κ�IO���ţ�
	2.�����ӿں�Ӳ��I2C��ȫһ�����Ի������
	ע�⣺
				1.�����speed����delay_us��ʱ�������ơ����в�׼
				2.speed����ֻ��Ϊ100000��200000,300k,400k��������Ǵ�ֵ����Ὣspeed��ֱֵ�Ӵ��ݸ�delay_us.��delay_us(speed);
				3.���ڵ���I2C�豸����ʹ��100k�����ߴ���10��ֵ
*/
class SoftI2c :public I2c
{
public:
    SoftI2c(Gpio *scl, Gpio *sda);
    virtual void 		begin(uint32_t speed);
    virtual void 		config(uint32_t speed);
    virtual uint32_t	read_config();

    virtual int8_t		write_byte(uint8_t slave_address, uint8_t reg_address, uint8_t data);
    virtual int8_t 		write_byte(uint8_t slave_address, uint8_t reg_address, uint8_t *data, uint16_t len);
    virtual int8_t 		read_byte (uint8_t slave_address, uint8_t reg_address, uint8_t *data);
    virtual int8_t 		read_byte (uint8_t slave_address, uint8_t reg_address, uint8_t *data, uint16_t len);
    virtual int8_t		wait_dev_busy (uint8_t slave_address);
public:
    virtual int8_t      take_i2c_right(uint32_t speed);
    virtual int8_t      release_i2c_right(void);

private:
    virtual int8_t 	start();
    virtual int8_t 	stop();
    virtual int8_t 	send_ack();
    virtual int8_t 	send_no_ack();

    virtual int8_t 	send_byte(uint8_t Byte);
    virtual int8_t	send_7bits_address(uint8_t slave_address);
    virtual int8_t  receive_byte(uint8_t *data);

    virtual int8_t	wait_ack();

private:
    Gpio        *sda_pin;
    Gpio        *scl_pin;
    uint32_t	speed;
    uint16_t	delay_times;
    uint8_t 	busy;
};

#endif
