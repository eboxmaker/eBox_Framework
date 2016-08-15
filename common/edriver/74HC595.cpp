/**
  ******************************************************************************
  * @file    74hc595.cpp
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
#include "74hc595.h"

_74hc595::_74hc595(Gpio *data_pin, Gpio *sck_pin, Gpio *rck_pin)
{
    this->data_pin 	= data_pin;
    this->sck_pin 	= sck_pin;
    this->rck_pin 	= rck_pin;
}

void _74hc595::begin()
{
    data_pin->mode(OUTPUT_PP);
    sck_pin->mode(OUTPUT_PP);
    rck_pin->mode(OUTPUT_PP);
}

void _74hc595::write(uint8_t *data, uint8_t data_length, uint8_t bit_oder)
{
    rck_pin->write(LOW);
    for(int i = 0; i < data_length; i++)
        shift_out(data_pin, sck_pin, bit_oder, data[i]);
}

void _74hc595::write(uint8_t data, uint8_t bit_oder)
{
    rck_pin->write(LOW);
    shift_out(data_pin, sck_pin, bit_oder, data);
}

void _74hc595::update()
{
    rck_pin->write(HIGH);

}


