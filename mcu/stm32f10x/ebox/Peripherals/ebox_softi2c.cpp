/**
  ******************************************************************************
  * @file    softi2c.cpp
  * @author  shentq
  * @version V1.2
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
#include "ebox_i2c.h"

SoftI2c::SoftI2c(Gpio *scl, Gpio *sda)
{
    this->scl_pin = scl;
    this->sda_pin = sda;
}
void SoftI2c::begin(uint32_t speed)
{
    this->speed = speed;
    config(this->speed);
    sda_pin->mode(OUTPUT_PP);
    scl_pin->mode(OUTPUT_PP);
}
int8_t SoftI2c::config(uint32_t speed)
{
    this->speed = speed;
    switch(this->speed)
    {
    case 400000:
        delay_times = 4;
        break;
    case 300000:
        delay_times = 8;
        break;
    case 200000:
        delay_times = 16;
        break;
    case 100000:
        delay_times = 32;
        break;
    default:
        delay_times = this->speed;
        break;
    }
    return 0;
}
uint32_t SoftI2c::read_config()
{
    return this->speed;
}
void SoftI2c::start()
{
    sda_pin->mode(OUTPUT_PP);
    sda_pin->set();
    scl_pin->set();
    delay_us(delay_times);
    sda_pin->reset();
    delay_us(delay_times);
    scl_pin->reset();

}

void SoftI2c::stop()
{
    sda_pin->mode(OUTPUT_PP);
    scl_pin->reset();
    sda_pin->reset();
    delay_us(delay_times);
    scl_pin->set();
    delay_us(delay_times);
    sda_pin->set();
}
int8_t SoftI2c::wait_ack()
{
    uint8_t cErrTime = 5;
    sda_pin->mode(INPUT_PU);
    scl_pin->set();
    delay_us(delay_times);
    while(sda_pin->read())
    {
        cErrTime--;
        delay_us(delay_times);
        if(cErrTime == 0)
        {
            sda_pin->mode(OUTPUT_PP);
            stop();
            return -1;

        }

    }
    scl_pin->reset();
    delay_us(delay_times);
    return 0;
}
int8_t SoftI2c::send_ack()
{
    sda_pin->mode(OUTPUT_PP);
    sda_pin->reset();
    delay_us(delay_times);
    scl_pin->set();
    delay_us(delay_times);
    scl_pin->reset();
    delay_us(delay_times);

    return 0;
}
int8_t SoftI2c::send_no_ack()
{
    sda_pin->mode(OUTPUT_PP);
    sda_pin->set();
    delay_us(delay_times);
    scl_pin->set();
    delay_us(delay_times);
    scl_pin->reset();
    delay_us(delay_times);
    return 0;
}
int8_t SoftI2c::send_byte(uint8_t byte)
{
    int8_t ret = 0;

    uint8_t ii = 8;
    sda_pin->mode(OUTPUT_PP);
    while( ii-- )
    {
        scl_pin->reset();
        sda_pin->write(byte & 0x80);
        delay_us(delay_times);
        byte += byte;//<<1
        scl_pin->set();
        delay_us(delay_times);
        scl_pin->reset();
        delay_us(delay_times);
    }
    ret = wait_ack();
    return ret;
}
int8_t	SoftI2c::send_7bits_address(uint8_t slave_address)
{
    int8_t ret = 0;
    send_byte(slave_address);
    return ret;
}

uint8_t SoftI2c::receive_byte(void)
{
    uint8_t i = 8;
    uint8_t byte = 0;
    sda_pin->mode(INPUT_PU);
    while(i--)
    {
        byte += byte;
        scl_pin->reset();
        delay_us(delay_times);
        scl_pin->set();
        delay_us(delay_times);
        byte |= sda_pin->read();
    }
    scl_pin->reset();
    delay_us(delay_times);

    return byte;
}
int8_t SoftI2c::write_byte(uint8_t slave_address, uint8_t reg_address, uint8_t data)
{
    int8_t ret = 0;
    start();

    if (send_7bits_address(slave_address) == -1)
        ret = -1;

    if (send_byte(reg_address) == -1)
        ret = -2;

    if (send_byte(data) == -1)
        ret = -3;

    stop();

    delay_us(10);
    return ret;
}
int8_t SoftI2c::write_byte(uint8_t slave_address, uint8_t reg_address, uint8_t *data, uint16_t num_to_write)
{
    int8_t ret = 0;
    start();

    if (send_7bits_address(slave_address) == -1)
        ret = -1;

    if (send_byte(reg_address) == -1)
        ret = -2;

    while(num_to_write--)
    {
        send_byte(*data++);
        if (wait_ack() == -1)
            ret = -3;
    }

    stop();

    delay_us(10);
    return ret;
}
int8_t 	SoftI2c::read_byte(uint8_t slave_address, uint8_t reg_address, uint8_t *data)
{

    int8_t ret = 0;
    start();

    if (send_7bits_address(slave_address) == -1)
        ret = -1;


    if (send_byte(reg_address) == -1)
        ret = -2;

    start();

    if (send_byte(slave_address + 1) == -1)
        ret = -3;

    *data = receive_byte();
    send_no_ack();
    stop();

    return ret;
}
int8_t 	SoftI2c::read_byte(uint8_t slave_address, uint8_t reg_address, uint8_t *data, uint16_t num_to_read)
{
    int8_t ret = 0;
    int i = 0;

    start();

    if (send_7bits_address(slave_address) == -1)
        ret = -1;


    if (send_byte(reg_address) == -1)
        ret = -2;

    start();
    if (send_7bits_address(slave_address + 1) == -1)
        ret = -3;

    while(num_to_read)
    {
        *data++ = receive_byte();
        num_to_read--;
        i++;
        if(num_to_read == 0)
        {
            send_no_ack();
            stop();
            ret = 0;
        }
        else
            send_ack();
    }

    return ret;
}
int8_t SoftI2c::wait_dev_busy(uint8_t slave_address)
{
    int8_t ret;
    uint8_t i = 0;
    do
    {
        start();
        ret = send_7bits_address(slave_address);
        send_ack();
        send_byte(slave_address);
        stop();
        if(i++ == 100)
        {
            return -1;
        }
    }
    while(ret != 0); //�������ֵ����0�������ȴ�
    return 0;
}
int8_t SoftI2c::take_i2c_right(uint32_t speed)
{

    while((busy == 1) && (1))
    {
        delay_ms(1);
    }
    this->speed = speed;
    config(this->speed);
    busy = 1;
    return 0;
}
int8_t SoftI2c::release_i2c_right(void)
{
    busy = 0;
    return 0;
}
