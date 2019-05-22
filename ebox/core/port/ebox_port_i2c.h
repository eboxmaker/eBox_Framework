#ifndef __EBOX_PORT_I2C_H
#define __EBOX_PORT_I2C_H

#include "ebox_type.h"

class I2c
{
public:
    
    typedef enum 
    {
        WRITE = 0,
        READ = 1
    }RW_t;
    
    typedef enum 
    {
        K400,
        K300,
        K200,
        K100,
        K10,
    }Speed_t;
    typedef enum 
    {
        BIT8,
        BIT16,
    }Bits_t;
    
    typedef struct 
    {
        Speed_t     speed;
        Bits_t      regAddrBits;
    } Config_t;

public:
    I2c(): timeout(200) {};

    virtual void	begin(Config_t *cfg) = 0;
    virtual void    config(Config_t *cfg) = 0;
    // 单字节读写
    virtual uint8_t	write(uint16_t slaveAddr,uint8_t data) = 0;
    virtual uint8_t write(uint16_t slaveAddr,uint16_t regAddr, uint8_t data) = 0;
    virtual uint8_t read(uint16_t slaveAddr) = 0;
    virtual uint8_t read(uint16_t slaveAddr,uint16_t regAddr) = 0;
    // 读写n字节
    virtual uint8_t write_buf(uint16_t slaveAddr,uint8_t *data, uint16_t num_to_write) = 0;
    virtual uint8_t	write_buf(uint16_t slaveAddr,uint16_t regAddr, uint8_t *data, uint16_t num_to_write) = 0;

    virtual uint8_t	read_buf(uint16_t slaveAddr,uint8_t *data, uint16_t num_to_read) = 0;
    virtual uint8_t	read_buf(uint16_t slaveAddr,uint16_t regAddr, uint8_t *data, uint16_t num_to_read) = 0;
    // 等待设备响应
    virtual uint8_t check_busy(uint16_t slaveAddr) = 0;

    // 获取I2C控制权,成功返回E_OK,E_BUSY;需要和releaseRight成对使用
    virtual uint8_t take(Config_t *cfg ) = 0;
    // 释放I2C控制权
    virtual void    release(void) = 0;
protected:
    uint16_t timeout;
};

#endif
