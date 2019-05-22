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
#include "at24c02.h"

void At24c02::begin()
{
    i2c->begin(&cfg);
}
int8_t At24c02::write_byte(uint8_t byte_addr, uint8_t byte)
{
    i2c->take(&cfg);
    i2c->write(slaveAddr,byte_addr, byte);
    i2c->release();
    return 0;
}

int8_t At24c02::write_byte(uint8_t byte_addr, uint8_t *buf, uint16_t num_to_write)
{
    int8_t ret = 0;
    i2c->take(&cfg);
    i2c->write_buf(slaveAddr,byte_addr, buf, num_to_write);
    i2c->release();
    return ret;
}

uint8_t At24c02::read_byte(uint8_t byte_addr)
{
    uint8_t byte;
    i2c->take(&cfg);
    byte = i2c->read(slaveAddr,byte_addr);
    i2c->release();

    return byte;
}

int8_t	At24c02::read_byte(uint8_t byte_addr, uint8_t *buf, uint16_t num_to_read)
{
    int8_t ret = 0;
    i2c->take(&cfg);
    ret = i2c->read_buf(slaveAddr,byte_addr, buf, num_to_read);
    i2c->release();
    return ret;
}

int8_t At24c02::write_page(uint8_t *buf, uint8_t addr, uint8_t numToWrite)
{

    return 0;
}


