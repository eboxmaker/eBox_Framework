/**
  ******************************************************************************
  * @file    lsm303dhlc.h
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LSM303DLHC_H
#define __LSM303DLHC_H

#include "ebox.h"

#define LSM303_ACC_ADDR				0x32
#define LSM303_MAG_ADDR				0x3C
#define LSM303A_CTRL_REG1				((uint8_t)0x20)
#define LSM303A_CTRL_REG2				((uint8_t)0x21)
#define LSM303A_CTRL_REG3				((uint8_t)0x22)
#define LSM303A_CTRL_REG4				((uint8_t)0x23)
#define LSM303A_CTRL_REG5				((uint8_t)0x24)
#define LSM303A_HP_FILTER_RESET	((uint8_t)0x25)
#define LSM303A_REFERENCE				((uint8_t)0x26)
#define LSM303A_STATUS_REG			((uint8_t)0x27)
#define LSM303A_OUT_X_L					((uint8_t)0x28)
#define LSM303A_OUT_X_H					((uint8_t)0x29)
#define LSM303A_OUT_Y_L					((uint8_t)0x2A)
#define LSM303A_OUT_Y_H					((uint8_t)0x2B)
#define LSM303A_OUT_Z_L					((uint8_t)0x2C)
#define LSM303A_OUT_Z_H					((uint8_t)0x2D)
#define LSM303A_INT1_CFG				((uint8_t)0x30)
#define LSM303A_INT1_SOURCE			((uint8_t)0x31)
#define LSM303A_INT1_INT1_THS		((uint8_t)0x32)
#define LSM303A_INT1_DURATION		((uint8_t)0x33)
#define LSM303A_INT2_CFG				((uint8_t)0x34)
#define LSM303A_INT2_SOURCE			((uint8_t)0x35)
#define LSM303A_INT2_THS				((uint8_t)0x36)
#define LSM303A_INT2_DURATION		((uint8_t)0x37)
#define LSM303A_MULTIPLE				((uint8_t)0xA8)	// ??????

#define LSM303M_CRA_REG					((uint8_t)0x00)
#define LSM303M_CRB_REG					((uint8_t)0x01)
#define LSM303M_MR_REG					((uint8_t)0x02)
#define LSM303M_OUT_X_H					((uint8_t)0x03)
#define LSM303M_OUT_X_L					((uint8_t)0x04)
#define LSM303M_OUT_Y_H					((uint8_t)0x05)
#define LSM303M_OUT_Y_L					((uint8_t)0x06)
#define LSM303M_OUT_Z_H					((uint8_t)0x07)
#define LSM303M_OUT_Z_L					((uint8_t)0x08)
#define LSM303M_SR_REG					((uint8_t)0x09)
#define LSM303M_IRA_REG					((uint8_t)0x0A)
#define LSM303M_IRB_REG					((uint8_t)0x0B)
#define LSM303M_IRC_REG					((uint8_t)0x0C)
#define LSM303M_MULTIPLE				((uint8_t)0x83)	// ??????


class LSM303DLHC
{
public:
    int16_t acc_x, acc_y, acc_z;
    int16_t mag_x, mag_y, mag_z;
public:
    LSM303DLHC(SoftI2c *i2c)
    {
        this->i2c = i2c;
    }
    void begin(uint32_t speed);
    void write_reg(uint8_t slave_addr, uint8_t reg, uint8_t value);
    uint8_t read_reg(uint8_t slave_addr, uint8_t reg);

    void read_acc(void);
    void read_mag(void);
private:
    SoftI2c *i2c;
    uint32_t speed;

};
#endif
