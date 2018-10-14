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

#include "ebox_config.h"

#define WRITE   0
#define READ    1

/*
	1.֧��I2C1��I2C2
	2.��ʱ��֧��remap�������ܿ�����
*/

class mcuI2c :public I2c
{

public:
	mcuI2c(I2C_TypeDef *I2Cx,Gpio *scl_pin, Gpio *sda_pin);
	// ����I2C, speed - 10,100,400 �ֱ����10k��100k��400k
	virtual void	begin(uint16_t speed);
    virtual void    config(uint32_t speed);
	virtual uint32_t    readConfig();
	// ���ֽڶ�д
	virtual uint8_t	write(uint8_t slaveAddr, uint8_t data);
    virtual uint8_t write(uint8_t slaveAddr,uint16_t regAddr, uint8_t data,uint16_t tOut= 300);
	virtual uint8_t read(uint8_t slaveAddr);	
	virtual uint8_t read(uint8_t slaveAddr,uint16_t regAddr,uint16_t tOut= 300);
	// ��дn�ֽ�
	virtual uint8_t writeBuf(uint8_t slaveAddr,uint8_t *data,uint16_t nWrite,uint16_t tOut = 300);
	virtual uint8_t	writeBuf(uint8_t slaveAddr,uint16_t regAddr,uint8_t *data, uint16_t nWrite,uint16_t tOut = 300);
	
	virtual uint8_t	readBuf(uint8_t slaveAddr,uint8_t *data,uint16_t nRead,uint16_t tOut = 300);
	virtual uint8_t	readBuf(uint8_t slaveAddr,uint16_t regAddr,uint8_t *data, uint16_t nRead,uint16_t tOut = 300);
	// �ȴ��豸��Ӧ
	virtual uint8_t checkBusy(uint8_t slaveAddr,uint16_t tOut = 200);
	
	// ��ȡI2C����Ȩ,�ɹ�����E_OK,E_BUSY;��Ҫ��releaseRight�ɶ�ʹ��
	virtual uint8_t takeRight(uint32_t timing,uint16_t tOut = 300);
	// �ͷ�I2C����Ȩ
	virtual void    releaseRight(void);

private:
    int8_t _start(uint16_t tOut);
    void _stop();
    int8_t _sendAck();
    int8_t _sendNack();

    int8_t _sendByte(uint8_t regData,uint16_t tOut);
    int8_t _send7bitsAddress(uint8_t slaveAddr,uint8_t WR,uint16_t tOut);
    int8_t _receiveByte(uint8_t *data,uint16_t tOut);
//    virtual int8_t wait_ack(){return 0;};//��Ӳ������ɡ��Ѿ������ڸ��������ڲ���  

	I2C_TypeDef 	*_i2cx;		// i2c����
	uint32_t   	 	_timing;	// i2cʱ��
	uint8_t     	_busy;
    Gpio            *_sda;
    Gpio            *_scl;
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
    // ����I2C, speed - 10,100,400 �ֱ����10k��100k��400k
  virtual void	    begin(uint16_t speed);
  virtual void      config(uint32_t speed);
  virtual uint32_t  readConfig();
  // ���ֽڶ�д
  virtual uint8_t   write(uint8_t slaveAddr,uint8_t data);
  virtual uint8_t   write(uint8_t slaveAddr,uint16_t regAddr, uint8_t data,uint16_t tOut= 300);
  virtual uint8_t   read(uint8_t slaveAddr);
  virtual uint8_t   read(uint8_t slaveAddr,uint16_t regAddr,uint16_t tOut= 300);
  // ��дn�ֽ�
  virtual uint8_t   writeBuf(uint8_t slaveAddr,uint8_t *data,uint16_t nWrite,uint16_t tOut = 300);
  virtual uint8_t	writeBuf(uint8_t slaveAddr,uint16_t regAddr,uint8_t *data, uint16_t nWrite,uint16_t tOut = 300);

  virtual uint8_t	readBuf(uint8_t slaveAddr,uint8_t *data,uint16_t nRead,uint16_t tOut = 300);
  virtual uint8_t	readBuf(uint8_t slaveAddr,uint16_t regAddr,uint8_t *data, uint16_t nRead,uint16_t tOut = 300);
  // �ȴ��豸��Ӧ
  virtual uint8_t   checkBusy(uint8_t slaveAddr,uint16_t tOut = 200);

  // ��ȡI2C����Ȩ,�ɹ�����E_OK,E_BUSY;��Ҫ��releaseRight�ɶ�ʹ��
  virtual uint8_t   takeRight(uint32_t timing,uint16_t tOut = 300);
  // �ͷ�I2C����Ȩ
  virtual void    releaseRight(void);

private:
 
    int8_t _start(uint16_t tOut);
    void _stop();
    int8_t _sendAck();
    int8_t _sendNack();

    int8_t _sendByte(uint8_t byte,uint16_t tOut);
    int8_t _send7bitsAddress(uint8_t slaveAddr,uint8_t WR,uint16_t tOut);
    int8_t _receiveByte(uint8_t *data,uint16_t tOut);

    int8_t _waitAck();

private:
  Gpio            *_sda;
  Gpio            *_scl;
  uint32_t   	 	_timing;	// i2cʱ��
  uint8_t 	_busy;
};

#endif
