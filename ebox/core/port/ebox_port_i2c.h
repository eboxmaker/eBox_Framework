#ifndef __EBOX_PORT_I2C_H
#define __EBOX_PORT_I2C_H

#include "ebox_type.h"

class I2c
{
public:
    I2c(){};
//    virtual void 		begin(uint32_t speed) = 0;
//    virtual void 		config(uint32_t speed) = 0;
//    virtual uint32_t	read_config() = 0;

//    virtual int8_t		write_byte(uint8_t slave_address, uint8_t reg_address, uint8_t data) = 0;
//    virtual int8_t 		write_byte(uint8_t slave_address, uint8_t reg_address, uint8_t *data, uint16_t num_to_write) = 0;
//    virtual int8_t 		read_byte (uint8_t slave_address, uint8_t reg_address, uint8_t *data) = 0;
//    virtual int8_t 		read_byte (uint8_t slave_address, uint8_t reg_address, uint8_t *data, uint16_t num_to_read) = 0;
//    virtual int8_t		wait_dev_busy (uint8_t slave_address) = 0;
//	
//    virtual int8_t      take_i2c_right(uint32_t speed) = 0;
//    virtual int8_t      release_i2c_right(void) = 0;
        
    virtual void	begin(uint16_t speed) = 0;
	virtual void    config(uint32_t speed) = 0;
	virtual uint32_t    readConfig() = 0;
	// 单字节读写
	virtual uint8_t	write(uint8_t slaveAddr, uint8_t data,uint16_t tOut) = 0;
	virtual uint8_t read(uint8_t slaveAddr,uint16_t tOut) = 0;	
	virtual uint8_t read(uint8_t slaveAddr,uint8_t regAddr,uint16_t tOut) = 0;
	// 读写n字节
	virtual uint8_t writeBuf(uint8_t slaveAddr,uint8_t *data,uint16_t num_to_write,uint16_t tOut) = 0;
	virtual uint8_t	writeBuf(uint8_t slaveAddr,uint8_t regAddr,uint8_t *data, uint16_t num_to_write,uint16_t tOut) = 0;
	
	virtual uint8_t	readBuf(uint8_t slaveAddr,uint8_t *data, uint16_t num_to_read,uint16_t tOut) = 0;
	virtual uint8_t	readBuf(uint8_t slaveAddr, uint8_t regAddr, uint8_t *data,uint16_t num_to_read,uint16_t tOut) = 0;
	// 等待设备响应
	virtual uint8_t waitAck(uint8_t slaveAddr,uint16_t tOut)=0;
	
	// 获取I2C控制权,成功返回E_OK,E_BUSY;需要和releaseRight成对使用
	virtual uint8_t takeRight(uint32_t timing,uint16_t tOut )=0;
	// 释放I2C控制权
	virtual void    releaseRight(void)=0;


	
	
private:
//    virtual int8_t 	start() = 0;
//    virtual int8_t 	stop() = 0;
//    virtual int8_t 	send_ack() = 0;
//    virtual int8_t 	send_no_ack() = 0;

//    virtual int8_t 	send_byte(uint8_t Byte) = 0;
//    virtual int8_t	send_7bits_address(uint8_t slave_address) = 0;
//    virtual int8_t receive_byte(uint8_t *data) = 0;

//    virtual int8_t	wait_ack() = 0;


};

#endif
