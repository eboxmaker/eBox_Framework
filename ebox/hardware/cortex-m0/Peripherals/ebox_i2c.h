/**
  ******************************************************************************
  * @file    ebox_i2c.h
  * @author  cat_li
  * @brief   ����������ģʽ
		1    2017/5/30  ���ӳ�ʱ����ֹ������������д�������ӷ���״̬
		2    2017/7/24  �޸ĳ�ʱģʽ,I2C2��֤ok,���Ӷ��ֽ�д�ֽں���
  *	ע�⣺��ȡ�ͷ�Ȩ�޳���Ҫ�Ż���
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

#ifndef __EBOX_I2C_H_
#define __EBOX_I2C_H_

#include "i2c.h"
#include "mcu.h"

//#define WRITE   0
//#define READ    1

///*
//	1.֧��I2C1��I2C2,������ʹ��PCLKclock
//	2.֧��remap,��stm32f072define�ж���
//*/

//class mcuI2c : public I2c
//{

//public:
//    mcuI2c(I2C_TypeDef *I2Cx, Gpio *scl_pin, Gpio *sda_pin);
//    // ����I2C, speed - 10,100,400 �ֱ����10k��100k��400k
//    virtual void	begin(Config_t *cfg);
//    virtual void    config(Config_t *cfg);
//    // ���ֽڶ�д
//    virtual uint8_t	write(uint16_t slaveAddr, uint8_t data);
//    virtual uint8_t write(uint16_t slaveAddr, uint16_t regAddr, uint8_t data);
//    virtual uint8_t read(uint16_t slaveAddr);
//    virtual uint8_t read(uint16_t slaveAddr, uint16_t regAddr);
//    // ��дn�ֽ�
//    virtual uint8_t write_buf(uint16_t slaveAddr, uint8_t *data, uint16_t nWrite);
//    virtual uint8_t	write_buf(uint16_t slaveAddr, uint16_t regAddr, uint8_t *data, uint16_t nWrite);

//    virtual uint8_t	read_buf(uint16_t slaveAddr, uint8_t *data, uint16_t nRead);
//    virtual uint8_t	read_buf(uint16_t slaveAddr, uint16_t regAddr, uint8_t *data, uint16_t nRead);
//    // �ȴ��豸��Ӧ
//    virtual uint8_t check_busy(uint16_t slaveAddr);

//    // ��ȡI2C����Ȩ,�ɹ�����E_OK,E_BUSY;��Ҫ��releaseRight�ɶ�ʹ��
//    virtual uint8_t take(Config_t *cfg);
//    // �ͷ�I2C����Ȩ
//    virtual void    release(void);

//private:
//    I2C_TypeDef     *_i2cx;		// i2c����
//    Config_t        *_cfg;	// i2cʱ��
//    uint32_t   	 	_timing;	// i2cʱ��
//    uint8_t     	_busy;
//    Gpio            *_sda;
//    Gpio            *_scl;
//};


class mcuI2c : public I2c
{
public:
    // public methods
    mcuI2c();
    mcuI2c(I2C_TypeDef *I2Cx, Gpio *sclPin, Gpio *sdaPin);

    virtual void begin(uint8_t address);
    virtual void begin(int address);
    virtual void begin();
    virtual void setClock(Speed_t speed = K100);

private:
  // per object data
    I2C_TypeDef 	*_i2cx;		// i2c����
    Gpio            *_sda;
    Gpio            *_scl;
    uint32_t         _speed;
    
    // private methods
    virtual i2c_err_t _write(uint8_t address,const uint8_t *data, size_t quantity, int sendStop);
    virtual size_t _read(uint8_t address,uint8_t *data, uint16_t quantity,uint8_t sendStop);
};

#endif
