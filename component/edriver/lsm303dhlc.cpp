/**
  ******************************************************************************
  * @file    lsm303dhk.cpp
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
#include "lsm303dhlc.h"

void LSM303DLHC::begin()
{
    i2c->begin();
    write_reg(LSM303_ACC_ADDR, LSM303A_CTRL_REG1, 0x37);
    write_reg(LSM303_ACC_ADDR, LSM303A_CTRL_REG4, 0x10);

    write_reg(LSM303_MAG_ADDR, LSM303M_CRB_REG, 0x20);
    write_reg(LSM303_MAG_ADDR, LSM303M_MR_REG, 0x00);

}
void LSM303DLHC::write_reg(uint8_t slave_addr, uint8_t reg, uint8_t value)
{
    
    i2c->beginTransmission(slave_addr);
    i2c->write(reg);
    i2c->write(value);
    i2c->endTransmission();
}
uint8_t LSM303DLHC::read_reg(uint8_t slave_addr, uint8_t reg)
{ 
    uint8_t value;
    
    i2c->requestFrom(slave_addr,1,reg,1,true);
    if(i2c->available())
    {
        value = i2c->read();
    }
    return value;
}
void LSM303DLHC::read_acc(void)
{
    //  uint8_t buf[6];

//    acc_x = ((read_reg(LSM303_ACC_ADDR, LSM303A_OUT_X_H) << 8) + read_reg(LSM303_ACC_ADDR,LSM303A_OUT_X_L ));
//    acc_y = ((read_reg(LSM303_ACC_ADDR, LSM303A_OUT_Y_H) << 8) + read_reg(LSM303_ACC_ADDR, LSM303A_OUT_Y_L));
//    acc_z = ((read_reg(LSM303_ACC_ADDR, LSM303A_OUT_Z_H) << 8) + read_reg(LSM303_ACC_ADDR, LSM303A_OUT_Z_L));

    uint8_t buf[6];
    i2c->requestFrom(LSM303_ACC_ADDR,6,LSM303A_OUT_X_L,1,true);
    for(int i = 0; i < 6; i++)
    {
        buf[i] = i2c->read();
    }
    acc_x = (buf[1] << 8) + buf[0];
    acc_y = (buf[3] << 8) + buf[2];
    acc_z = (buf[5] << 8) + buf[4];

}
void LSM303DLHC::read_mag(void)
{
    uint8_t buf[6];
    i2c->requestFrom(LSM303_MAG_ADDR,6,LSM303M_OUT_X_H,1,true);
    for(int i = 0; i < 6; i++)
    {
        buf[i] = i2c->read();
    }
    mag_x = (buf[0] << 8) + buf[1];
    mag_y = (buf[2] << 8) + buf[3];
    mag_z = (buf[4] << 8) + buf[5];

}
