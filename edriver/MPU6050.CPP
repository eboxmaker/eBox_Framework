/*
file   : mpu6050.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentqlf@163.com.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentqlf@163.com assumes no responsibility for any errors contained herein.
*/

#include "mpu6050.h"


void MPU6050::begin(uint32_t speed)
{
    this->speed = speed;
    i2c->take_i2c_right(this->speed);
    i2c->begin(this->speed);
    i2c->write_byte(SLAVEADDRESS, PWR_MGMT_1, 0);
    i2c->write_byte(SLAVEADDRESS, SMPLRT_DIV, 0x07);
    i2c->write_byte(SLAVEADDRESS, CONFIG, 0x06);
    i2c->write_byte(SLAVEADDRESS, GYRO_CONFIG, 0x18);
    i2c->write_byte(SLAVEADDRESS, ACCEL_CONFIG, 0x01);
    i2c->write_byte(SLAVEADDRESS, PWR_MGMT_1, 0);
    i2c->write_byte(SLAVEADDRESS, SMPLRT_DIV, 0x07);
    i2c->write_byte(SLAVEADDRESS, CONFIG, 0x06);
    i2c->write_byte(SLAVEADDRESS, GYRO_CONFIG, 0x18);
    i2c->write_byte(SLAVEADDRESS, ACCEL_CONFIG, 0x01);
    i2c->release_i2c_right();
}
void MPU6050::get_id(uint8_t *id)
{
    i2c->take_i2c_right(speed);
    i2c->read_byte(SLAVEADDRESS, WHO_AM_I, id, 1);
    i2c->release_i2c_right();
};

int16_t  MPU6050::get_data(uint8_t reg_address)
{
    uint8_t tmp[2];
    i2c->take_i2c_right(speed);
    i2c->read_byte(SLAVEADDRESS, reg_address, tmp, 2);
    i2c->release_i2c_right();

    return ((tmp[0] << 8) + tmp[1]);

}
int8_t  MPU6050::get_data(uint8_t reg_address, int16_t *buf, uint8_t num_to_read)
{
    uint8_t i, readnum;
    uint8_t tmpbuf[20];

    i2c->take_i2c_right(speed);
    readnum = i2c->read_byte(SLAVEADDRESS, reg_address, tmpbuf, num_to_read * 2);
    i2c->release_i2c_right();

    for(i = 0; i < readnum / 2; i++)
    {
        *buf++ = (tmpbuf[i * 2 + 0] << 8) + (tmpbuf[i * 2 + 1]);
    }
    return readnum / 2;
}

