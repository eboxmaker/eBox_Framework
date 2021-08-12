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

#ifndef __EBOX_MCU_I2C_H_
#define __EBOX_MCU_I2C_H_

#include "ebox_core.h"
#include "mcu.h"
#include "i2c.h"



/*
	1.֧��I2C1��I2C2
	2.��ʱ��֧��remap�������ܿ�����
*/

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
    uint8_t         _err;
    uint8_t         _err_at;
    uint32_t         _speed;
    I2C_InitTypeDef  I2C_InitStructure;

    // private methods
    virtual i2c_err_t _write(const uint8_t *data, size_t);
    virtual i2c_err_t _write(uint8_t address,const uint8_t *data, size_t quantity, int sendStop);
    virtual size_t _read(uint8_t address,uint8_t *data, uint16_t quantity,uint8_t sendStop);

    
    i2c_err_t _start(void);
    i2c_err_t _stop(void);
    i2c_err_t _waitAck();
    i2c_err_t _sendByte( uint8_t data);
    i2c_err_t  _send7bitsAddress(uint8_t slaveAddr, uint8_t WR);
    i2c_err_t _receiveByte(uint8_t *data);
    i2c_err_t _sendAck();
    i2c_err_t _sendNack();
};


#endif
