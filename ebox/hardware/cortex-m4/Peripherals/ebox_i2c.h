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

#include "ebox_core.h"
#include "mcu.h"

#define WRITE   0
#define READ    1

/*
	1.支持I2C1和I2C2
	2.暂时不支持remap，后续很快会完成
*/

class mcuI2c : public I2c
{

public:
    mcuI2c(I2C_TypeDef *I2Cx, Gpio *scl_pin, Gpio *sda_pin);
    // 启动I2C, speed - 10,100,400 分别代表10k，100k，400k
    virtual void	begin(Config_t *newConfig);
    virtual void    config(Config_t *newConfig);
    // 单字节读写
    virtual uint8_t	write(uint16_t slaveAddr, uint8_t data);
    virtual uint8_t write(uint16_t slaveAddr, uint16_t regAddr, uint8_t data);
    virtual uint8_t read(uint16_t slaveAddr);
    virtual uint8_t read(uint16_t slaveAddr, uint16_t regAddr);
    // 读写n字节
    virtual uint8_t write_buf(uint16_t slaveAddr, uint8_t *data, uint16_t nWrite);
    virtual uint8_t	write_buf(uint16_t slaveAddr, uint16_t regAddr, uint8_t *data, uint16_t nWrite);

    virtual uint8_t	read_buf(uint16_t slaveAddr, uint8_t *data, uint16_t nRead);
    virtual uint8_t	read_buf(uint16_t slaveAddr, uint16_t regAddr, uint8_t *data, uint16_t nRead);
    // 等待设备响应
    virtual uint8_t check_busy(uint16_t slaveAddr);

    // 获取I2C控制权,成功返回E_OK,E_BUSY;需要和releaseRight成对使用
    virtual uint8_t take(Config_t *newConfig);
    // 释放I2C控制权
    virtual void    release(void);

private:
    int8_t _start();
    void _stop();
    int8_t _sendAck();
    int8_t _sendNack();

    int8_t _sendByte(uint8_t regData);
    int8_t _send7bitsAddress(uint8_t slaveAddr, uint8_t WR);
    int8_t _receiveByte(uint8_t *data);
    //    virtual int8_t wait_ack(){return 0;};//由硬件来完成。已经集成在各个函数内部。

    I2C_TypeDef 	*_i2cx;		// i2c外设
    Config_t        *cfg;	// i2c时序
    uint32_t   	 	_timing;	// i2c时序
    uint8_t     	_busy;
    Gpio            *_sda;
    Gpio            *_scl;
};


/*
	1.支持任何IO引脚；
	2.函数接口和硬件I2C完全一样可以互相替代
	注意：
				1.该类的speed是由delay_us延时函数控制。略有不准
				2.speed设置只能为100000，200000,300k,400k。如果不是此值，则会将speed的值直接传递给delay_us.即delay_us(speed);
				3.初期调试I2C设备建议使用100k。或者大于10的值
*/
class SoftI2c : public I2c
{
public:
    SoftI2c(Gpio *scl, Gpio *sda);
    // 启动I2C, speed - 10,100,400 分别代表10k，100k，400k
    virtual void      begin(Config_t *newConfig );
    virtual void      config(Config_t *newConfig );
    // 单字节读写
    virtual uint8_t   write(uint16_t slaveAddr,uint8_t data);
    virtual uint8_t   write(uint16_t slaveAddr,uint16_t regAddr, uint8_t data);
    virtual uint8_t   read(uint16_t slaveAddr);
    virtual uint8_t   read(uint16_t slaveAddr,uint16_t regAddr);
    // 读写n字节
    virtual uint8_t   write_buf(uint16_t slaveAddr,uint8_t *data, uint16_t nWrite);
    virtual uint8_t	  write_buf(uint16_t slaveAddr,uint16_t regAddr, uint8_t *data, uint16_t nWrite);

    virtual uint8_t	  read_buf(uint16_t slaveAddr,uint8_t *data, uint16_t nRead);
    virtual uint8_t	  read_buf(uint16_t slaveAddr,uint16_t regAddr, uint8_t *data, uint16_t nRead);
    // 等待设备响应
    virtual uint8_t   check_busy(uint16_t slaveAddr);

    // 获取I2C控制权,成功返回E_OK,E_BUSY;需要和releaseRight成对使用
    virtual uint8_t   take(Config_t *newConfig );
    // 释放I2C控制权
    virtual void    release(void);

private:

    int8_t _start();
    void   _stop();
    int8_t _sendAck();
    int8_t _sendNack();

    int8_t _sendByte(uint8_t byte);
    int8_t _send7bitsAddress(uint8_t slaveAddr, uint8_t WR);
    int8_t _receiveByte(uint8_t *data);

    int8_t _waitAck();

private:
    Gpio            *_sda;
    Gpio            *_scl;
    Config_t        *cfg;	// 
	uint32_t   	 	_timing;	// i2c时序
    uint8_t 	    _busy;
};
#endif
