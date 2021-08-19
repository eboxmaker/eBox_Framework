/**
  ******************************************************************************
  * @file    ebox_i2c.h
  * @author  cat_li
  * @brief   仅工作在主模式
		1    2017/5/30  增加超时，防止程序死掉。读写函数增加返回状态
		2    2017/7/24  修改超时模式,I2C2验证ok,增加读字节写字节函数
  *	注意：获取释放权限出需要优化。
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
//	1.支持I2C1和I2C2,两个均使用PCLKclock
//	2.支持remap,在stm32f072define中定义
//*/

//class mcuI2c : public I2c
//{

//public:
//    mcuI2c(I2C_TypeDef *I2Cx, Gpio *scl_pin, Gpio *sda_pin);
//    // 启动I2C, speed - 10,100,400 分别代表10k，100k，400k
//    virtual void	begin(Config_t *cfg);
//    virtual void    config(Config_t *cfg);
//    // 单字节读写
//    virtual uint8_t	write(uint16_t slaveAddr, uint8_t data);
//    virtual uint8_t write(uint16_t slaveAddr, uint16_t regAddr, uint8_t data);
//    virtual uint8_t read(uint16_t slaveAddr);
//    virtual uint8_t read(uint16_t slaveAddr, uint16_t regAddr);
//    // 读写n字节
//    virtual uint8_t write_buf(uint16_t slaveAddr, uint8_t *data, uint16_t nWrite);
//    virtual uint8_t	write_buf(uint16_t slaveAddr, uint16_t regAddr, uint8_t *data, uint16_t nWrite);

//    virtual uint8_t	read_buf(uint16_t slaveAddr, uint8_t *data, uint16_t nRead);
//    virtual uint8_t	read_buf(uint16_t slaveAddr, uint16_t regAddr, uint8_t *data, uint16_t nRead);
//    // 等待设备响应
//    virtual uint8_t check_busy(uint16_t slaveAddr);

//    // 获取I2C控制权,成功返回E_OK,E_BUSY;需要和releaseRight成对使用
//    virtual uint8_t take(Config_t *cfg);
//    // 释放I2C控制权
//    virtual void    release(void);

//private:
//    I2C_TypeDef     *_i2cx;		// i2c外设
//    Config_t        *_cfg;	// i2c时序
//    uint32_t   	 	_timing;	// i2c时序
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
    I2C_TypeDef 	*_i2cx;		// i2c外设
    Gpio            *_sda;
    Gpio            *_scl;
    uint32_t         _speed;
    
    // private methods
    virtual i2c_err_t _write(uint8_t address,const uint8_t *data, size_t quantity, int sendStop);
    virtual size_t _read(uint8_t address,uint8_t *data, uint16_t quantity,uint8_t sendStop);
};

#endif
