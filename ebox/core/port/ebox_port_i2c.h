#ifndef __EBOX_PORT_I2C_H
#define __EBOX_PORT_I2C_H

#include "ebox_type.h"

class I2c
{
public:
    I2c(): timeout(200) {};

    virtual void	begin(uint16_t speed) = 0;
    virtual void    config(uint32_t speed) = 0;
    virtual uint32_t    read_config() = 0;
    // ���ֽڶ�д
    virtual uint8_t	write(uint8_t slaveAddr, uint8_t data) = 0;
    virtual uint8_t write(uint8_t slaveAddr, uint8_t regAddr, uint8_t data) = 0;
    virtual uint8_t write(uint8_t slaveAddr, uint16_t regAddr, uint8_t data) = 0;
    virtual uint8_t read(uint8_t slaveAddr) = 0;
    virtual uint8_t read(uint8_t slaveAddr, uint8_t regAddr) = 0;
    virtual uint8_t read(uint8_t slaveAddr, uint16_t regAddr) = 0;
    // ��дn�ֽ�
    virtual uint8_t write_buf(uint8_t slaveAddr, uint8_t *data, uint16_t num_to_write) = 0;
    virtual uint8_t	write_buf(uint8_t slaveAddr, uint8_t regAddr, uint8_t *data, uint16_t num_to_write) = 0;
    virtual uint8_t	write_buf(uint8_t slaveAddr, uint16_t regAddr, uint8_t *data, uint16_t num_to_write) = 0;

    virtual uint8_t	read_buf(uint8_t slaveAddr, uint8_t *data, uint16_t num_to_read) = 0;
    virtual uint8_t	read_buf(uint8_t slaveAddr, uint8_t regAddr, uint8_t *data, uint16_t num_to_read) = 0;
    virtual uint8_t	read_buf(uint8_t slaveAddr, uint16_t regAddr, uint8_t *data, uint16_t num_to_read) = 0;
    // �ȴ��豸��Ӧ
    virtual uint8_t check_busy(uint8_t slaveAddr) = 0;

    // ��ȡI2C����Ȩ,�ɹ�����E_OK,E_BUSY;��Ҫ��releaseRight�ɶ�ʹ��
    virtual uint8_t take(uint32_t timing ) = 0;
    // �ͷ�I2C����Ȩ
    virtual void    release(void) = 0;
protected:
    uint16_t timeout;
};

#endif
