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
#include "ebox_gpio.h"
#include "ebox_port_i2c.h"

#define WRITE   0
#define READ    1

// 分频，建立，保持，高电平，低电平
#define	C16M10K	 	__LL_I2C_CONVERT_TIMINGS(0x3, 0xc7, 0xc3, 0x02, 0x04)		//10k@16M
#define	C16M100K	__LL_I2C_CONVERT_TIMINGS(0x3, 0x13, 0xf, 0x02, 0x04)		//100k@16M
#define	C16M400K	__LL_I2C_CONVERT_TIMINGS(0x1, 0x9, 0x3, 0x2, 0x3)			//400k@16M

#define	C48M10K	 	__LL_I2C_CONVERT_TIMINGS(0xb, 0xc7, 0xc3, 0x02, 0x04)		//10k@48M
#define	C48M100K	__LL_I2C_CONVERT_TIMINGS(0xb, 0x13, 0xf, 0x02, 0x04)		//100k@48M
#define	C48M400K	__LL_I2C_CONVERT_TIMINGS(0x5, 0x9, 0x3, 0x3, 0x3)			//400k@48M

#define	C8M10K	 	__LL_I2C_CONVERT_TIMINGS(0x1, 0xc7, 0xc3, 0x02, 0x04)		//10k@48M
#define	C8M100K	 	__LL_I2C_CONVERT_TIMINGS(0x1, 0x13, 0xf, 0x02, 0x04)		//100k@48M
#define	C8M400K	 	__LL_I2C_CONVERT_TIMINGS(0x0, 0x9, 0x3, 0x1, 0x3)			//400k@48M


class mcuI2c :public I2c
{

public:
  mcuI2c(I2C_TypeDef *I2Cx,Gpio *scl_pin, Gpio *sda_pin);
  // 启动I2C, speed - 10,100,400 分别代表10k，100k，400k
  virtual void	begin(uint16_t speed);
  virtual void    config(uint32_t speed);
  virtual uint32_t    readConfig();
  // 单字节读写
  virtual uint8_t	write(uint8_t slaveAddr, uint8_t data,uint16_t tOut= 300);
  virtual uint8_t read(uint8_t slaveAddr,uint16_t tOut= 300);
  virtual uint8_t read(uint8_t slaveAddr,uint8_t regAddr,uint16_t tOut= 300);
  // 读写n字节
  virtual uint8_t writeBuf(uint8_t slaveAddr,uint8_t *data,uint16_t nWrite,uint16_t tOut = 300);
  virtual uint8_t	writeBuf(uint8_t slaveAddr,uint8_t regAddr,uint8_t *data, uint16_t nWrite,uint16_t tOut = 300);

  virtual uint8_t	readBuf(uint8_t slaveAddr,uint8_t *data,uint16_t nRead,uint16_t tOut = 300);
  virtual uint8_t	readBuf(uint8_t slaveAddr,uint8_t regAddr,uint8_t *data, uint16_t nRead,uint16_t tOut = 300);
  // 等待设备响应
  virtual uint8_t waitAck(uint8_t slaveAddr,uint16_t tOut = 2000);

  // 获取I2C控制权,成功返回E_OK,E_BUSY;需要和releaseRight成对使用
  virtual uint8_t takeRight(uint32_t timing,uint16_t tOut = 300);
  // 释放I2C控制权
  virtual void    releaseRight(void);

private:
//    virtual int8_t start();
//    virtual int8_t stop();
//    virtual int8_t send_ack();
//    virtual int8_t send_no_ack();

//    virtual int8_t sendByte(uint8_t regData);
//    virtual int8_t send_7bits_address(uint8_t slave_address);
//    virtual int8_t receiveByte(uint8_t *data);
//    virtual int8_t wait_ack(){return 0;};//由硬件来完成。已经集成在各个函数内部。

  I2C_TypeDef 	*_i2cx;		// i2c外设
  uint32_t   	 _timing;	// i2c时序
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
class SoftI2c :public I2c
{
public:
  SoftI2c(Gpio *scl, Gpio *sda);
    // 启动I2C, speed - 10,100,400 分别代表10k，100k，400k
  virtual void	begin(uint16_t speed);
  virtual void    config(uint32_t speed);
  virtual uint32_t    readConfig();
  // 单字节读写
  virtual uint8_t	write(uint8_t slaveAddr, uint8_t data,uint16_t tOut= 300);
  virtual uint8_t read(uint8_t slaveAddr,uint16_t tOut= 300);
  virtual uint8_t read(uint8_t slaveAddr,uint8_t regAddr,uint16_t tOut= 300);
  // 读写n字节
  virtual uint8_t writeBuf(uint8_t slaveAddr,uint8_t *data,uint16_t nWrite,uint16_t tOut = 300);
  virtual uint8_t	writeBuf(uint8_t slaveAddr,uint8_t regAddr,uint8_t *data, uint16_t nWrite,uint16_t tOut = 300);

  virtual uint8_t	readBuf(uint8_t slaveAddr,uint8_t *data,uint16_t nRead,uint16_t tOut = 300);
  virtual uint8_t	readBuf(uint8_t slaveAddr,uint8_t regAddr,uint8_t *data, uint16_t nRead,uint16_t tOut = 300);
  // 等待设备响应
  virtual uint8_t waitAck(uint8_t slaveAddr,uint16_t tOut = 2000);

  // 获取I2C控制权,成功返回E_OK,E_BUSY;需要和releaseRight成对使用
  virtual uint8_t takeRight(uint32_t timing,uint16_t tOut = 300);
  // 释放I2C控制权
  virtual void    releaseRight(void);
  
//  virtual void 		begin(uint32_t speed);
//  virtual void 		config(uint32_t speed);
//  virtual uint32_t	read_config();

//  virtual int8_t		write_byte(uint8_t slave_address, uint8_t reg_address, uint8_t data);
//  virtual int8_t 		write_byte(uint8_t slave_address, uint8_t reg_address, uint8_t *data, uint16_t len);
//  virtual int8_t 		read_byte (uint8_t slave_address, uint8_t reg_address, uint8_t *data);
//  virtual int8_t 		read_byte (uint8_t slave_address, uint8_t reg_address, uint8_t *data, uint16_t len);
//  virtual int8_t		wait_dev_busy (uint8_t slave_address);
//public:
//  virtual int8_t      take_i2c_right(uint32_t speed);
//  virtual int8_t      release_i2c_right(void);

private:
//  virtual int8_t 	start();
//  virtual int8_t 	stop();
//  virtual int8_t 	send_ack();
//  virtual int8_t 	send_no_ack();

//  virtual int8_t 	send_byte(uint8_t Byte);
//  virtual int8_t	send_7bits_address(uint8_t slave_address);
//  virtual int8_t  receive_byte(uint8_t *data);
  
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
  uint32_t   	 	_timing;	// i2c时序
  uint16_t	delay_times;
  uint8_t 	_busy;
};

#endif
