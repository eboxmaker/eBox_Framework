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

void At24c02::begin(uint32_t speed)
{
    i2c->begin(speed);
    _timing = i2c->readConfig();
    
}
int8_t At24c02::write_byte(uint16_t byte_addr, uint8_t byte)
{
//    i2c->take_i2c_right(speed);
//    i2c->takeRight(_timing,300);
    i2c->write(SLAVE_ADDR, byte_addr, byte,200);
//    i2c->releaseRight();
    return 0;
}

int8_t At24c02::write_byte(uint16_t byte_addr, uint8_t *buf, uint16_t num_to_write)
{
    int8_t ret = 0;

//    for(uint16_t i = 0; i < num_to_write; i++)
//    {
//        write_byte(byte_addr++, buf[i]);
//        ret = i2c->checkBusy(SLAVE_ADDR,200);//如果有错误再加此句

//    }
    i2c->writeBuf(SLAVE_ADDR,byte_addr,buf,num_to_write,300);

    //	i2c->writeByte(SLAVE_ADDR,byteAddr,buf,numToWrite);//部分器件个别的位置相应比较慢
    return ret;
}

uint8_t At24c02::read_byte(uint16_t byte_addr)
{
    uint8_t byte;
//    i2c->takeRight(_timing,300);
    byte = i2c->read(SLAVE_ADDR, byte_addr,300);
//    i2c->releaseRight();

    return byte;
}

int8_t	At24c02::read_byte(uint16_t byte_addr, uint8_t *buf, uint16_t num_to_read)
{
    int8_t ret = 0;
//    i2c->takeRight(_timing,300);

    //	for(uint16_t i = 0; i < numToRead; i++)
    //	{
    //			buf[i] = byteRead(byteAddr++);
    //	}
    ret = i2c->checkBusy(SLAVE_ADDR,200);//如果有错误再加此句
    ret = i2c->readBuf(SLAVE_ADDR, byte_addr, buf, num_to_read,300);
//    i2c->releaseRight();
    return ret;
}

int8_t At24c02::write_page(uint8_t *buf, uint32_t addr, uint8_t numToWrite)
{

    return 0;
}


