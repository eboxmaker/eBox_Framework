/**
  ******************************************************************************
  * @file    l3g4200.cpp
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
#include "L3G4200D.h"
#include "math.h"


void L3G4200D::begin(uint32_t speed)
{
    // 0x0F = 0b00001111
    // Normal power mode, all axes enabled
    this->speed = speed;
    i2c->begin(speed);

    write_reg(L3G4200D_CTRL_REG1, 0x0F);
    write_reg(L3G4200D_CTRL_REG2, 0X00);
    write_reg(L3G4200D_CTRL_REG3, 0X08);
    write_reg(L3G4200D_CTRL_REG4, 0X00);
    write_reg(L3G4200D_CTRL_REG5, 0X00);
}
void L3G4200D::test()
{
    DBG("id = %d\r\n", read_reg(L3G4200D_WHO_AM_I));
}

// Writes a gyro register
void L3G4200D::write_reg(uint8_t reg, uint8_t value)
{

    i2c->take_i2c_right(speed);
    i2c->write_byte(GYR_ADDRESS, reg, value);
    i2c->release_i2c_right();
}

// Reads a gyro register
uint8_t L3G4200D::read_reg(uint8_t reg)
{
    uint8_t value;
    i2c->take_i2c_right(speed);
    i2c->read_byte(GYR_ADDRESS, reg, &value);
    i2c->release_i2c_right();

    return value;
}
// Reads the 3 gyro channels and stores them in vector g
void L3G4200D::read()
{

    int8_t xla ;
    int8_t xha;
    int8_t yla;
    int8_t yha;
    int8_t zla;
    int8_t zha;

    //uint8_t addr = L3G4200D_OUT_X_L;

    xla = read_reg(L3G4200D_OUT_X_L);
    xha = read_reg(L3G4200D_OUT_X_H);

    yla = read_reg(L3G4200D_OUT_Y_L);
    yha = read_reg(L3G4200D_OUT_Y_H);

    zla = read_reg(L3G4200D_OUT_Z_L);
    zha = read_reg(L3G4200D_OUT_Z_H);


    g.x = xha << 8 | xla;
    g.y = yha << 8 | yla;
    g.z = zha << 8 | zla;
}

void L3G4200D::vector_cross(const vector *a, const vector *b, vector *out)
{
    out->x = a->y * b->z - a->z * b->y;
    out->y = a->z * b->x - a->x * b->z;
    out->z = a->x * b->y - a->y * b->x;
}

float L3G4200D::vector_dot(const vector *a, const vector *b)
{
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

void L3G4200D::vector_normalize(vector *a)
{
    float mag = sqrt(vector_dot(a, a));
    a->x /= mag;
    a->y /= mag;
    a->z /= mag;
}
