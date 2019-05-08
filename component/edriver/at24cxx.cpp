/**
  ******************************************************************************
  * @file    at24cxx.cpp
  * @author  wele
  * @version V1.0
  * @date    2018/12/14
  * @brief
  ******************************************************************************
  * @attention
  *
  * No part of this software may be used for any commercial activities by any form
  * or means, without the prior written consent of wele. This specification is
  * preliminary and is subject to change at any time without notice. wele assumes
  * no responsibility for any errors contained herein.
  * <h2><center>&copy; Copyright 2018 wele. All Rights Reserved.</center></h2>
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "at24cxx.h"
#include "bsp_ebox.h"

At24cxx ee(&I2C);

void At24cxx::begin(uint32_t speed,uint16_t eetype)
{
    i2c->begin(speed);
    _timing = i2c->read_config();
    ee_type = eetype;
    is_add8 = 0;
    //初始化EEPROM 类型参数
    switch(ee_type)
    {
        case 1:                          //24c01
            page_num = 16;
            page_size = 8;
            add_max = E2P_01_TSIZE;
            is_add8 =1;
        case 2:                          //24c02
            page_num = 16;
            page_size = 8;
            add_max = E2P_02_TSIZE;
            is_add8 =1;
        break;
        case 4:                          //24c04
            page_num = 32;
            page_size = 8;
            add_max = E2P_04_TSIZE;
        break;
        case 8:                          //24c08
            page_num = 64;
            page_size = 16;
            add_max = E2P_08_TSIZE;
        break;
        case 16:                          //24c16
            page_num = 128;
            page_size = 16;
            add_max = E2P_16_TSIZE;
        break; 
        case 32:                           //24c32
            page_num = 128;
            page_size = 32;
            add_max = E2P_32_TSIZE;
        break;
        case 64:                           //24c64
            page_num = 256;
            page_size = 32;
            add_max = E2P_64_TSIZE;
        break;
        case 128:                          //24c128
            page_num = 256;
            page_size = 64;
            add_max = E2P_128_TSIZE;
        break;
        case 256:                          //24c256
            page_num = 512;
            page_size = 64;
            add_max = E2P_256_TSIZE;
        break;
        case 512:                          //24c512
            page_num = 512;
            page_size = 128;
            add_max = E2P_512_TSIZE;
        break;
        case 1024:                         //24c128
            page_num = 512;
            page_size = 128;
            add_max = E2P_1024_TSIZE;
        break;
        case 2048:                         //24c256
            page_num = 512;
            page_size = 256;
            add_max = E2P_2048_TSIZE;
        break;
        default:

        break;
    }
}

int8_t At24cxx::slaveadj(uint16_t byte_addr)
{
    int8_t temp;
    slave_add = SLAVE_ADDR;

    if (page_size < 32)
    {
        slave_add = SLAVE_ADDR | ((byte_addr >> 7) & 0x0E);    /* 数据地址为：A2+A1+A0+8位数据 */    

    }
    else if ((page_size >= 32)&&(page_size <= 128))            /* 获取器件地址：A2+A1+A0+0+16位数据 */
    {
        slave_add = SLAVE_ADDR | ((byte_addr >> 16) & 0x0E);        
    }
    else
    {
        temp = (byte_addr >> 16) << 1;
        slave_add = SLAVE_ADDR | (temp & 0x0E); 
    }
    return slave_add;
}
// 一个页内进行写操作,内部使用 不能越界写数据
int8_t At24cxx::write_page(uint8_t *buf, uint16_t addr, uint16_t numToWrite)
{
    int8_t ret = 0,slave_addr;
    slave_addr = slaveadj(addr);
    if(is_add8)
    {
        reg_add8 = addr;
        i2c->write_buf(slave_addr, reg_add8, buf, numToWrite);
    }
    else
    {
        reg_add16 = addr;
        i2c->write_buf(slave_addr, reg_add16, buf, numToWrite);
    }
    delay_ms(5);
    return ret;
}

//可以实现跨页写  跨页写时候需要延时
int8_t At24cxx::write_byte(uint16_t byte_addr, uint8_t *buf, uint16_t num_to_write)
{
    int8_t ret = 0;
    uint16_t length,Tmp;    
           
    while (num_to_write > 0)
    {
        Tmp = byte_addr & (page_size - 1);          /* 计算得到当前要写的页剩余写空间 */             

        if (Tmp != 0)
        {
            length = page_size - Tmp;
        }
        else
        {
            length = page_size;
        }

        if (num_to_write < length)                  /* 如果要写长度在该页之内 */                                         
        {
            length = num_to_write;
        }
        ret += write_page(buf, byte_addr, length);
        
        byte_addr += length;
        buf += length;
        num_to_write -= length;        
    }
    
    return ret;
}

//可以实现跨页读
int8_t At24cxx::read_byte(uint16_t byte_addr, uint8_t *buf, uint16_t num_to_read)
{
    int8_t ret = 0,slave_addr;
    slave_addr = slaveadj(byte_addr);
    
    if(is_add8)
    {
        reg_add8 = byte_addr;
        i2c->read_buf(slave_addr, reg_add8, buf, num_to_read);
    }
    else
    {
        reg_add16 = byte_addr;
        i2c->read_buf(slave_addr, reg_add16, buf, num_to_read);
    }
    return ret;
}


