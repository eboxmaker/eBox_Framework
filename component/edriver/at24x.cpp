/**
  ******************************************************************************
  * @file    at24c02.cpp
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


/* Includes ------------------------------------------------------------------*/
#include "at24x.h"

void At24x::begin()
{
    i2c->begin();
    i2c->setClock(I2c::K100);
}
int8_t At24x::write_byte(uint8_t byte_addr, uint8_t byte)
{
    i2c->beginTransmission(slaveAddr);
    i2c->write(byte_addr);
    i2c->write(byte);
    i2c->endTransmission();
    return 0;
}

int8_t At24x::write_byte(uint8_t byte_addr, uint8_t *buf, uint16_t num_to_write)
{
    int8_t ret = 0;
    
    i2c->beginTransmission(slaveAddr);
    ret = i2c->write(byte_addr);
    ret = i2c->write(buf,num_to_write);
    ret = i2c->endTransmission();
    return ret;
}

uint8_t At24x::read_byte(uint8_t byte_addr)
{
    uint8_t num = i2c->requestFrom(slaveAddr,1,byte_addr,1,true);
    if(i2c->available())
    {
        return i2c->read();
    }
    return 0;
}

int8_t	At24x::read_byte(uint8_t byte_addr, uint8_t *buf, uint16_t num_to_read)
{
    int8_t ret = 0;
    
    uint8_t num = i2c->requestFrom(slaveAddr,num_to_read,byte_addr,1,true);
    while(i2c->available())
    {
        *buf = i2c->read();
        buf++;
    }
    
    return ret;
}

int8_t At24x::write_page(uint8_t *buf, uint8_t addr, uint8_t numToWrite)
{

    return 0;
}


