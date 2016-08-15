/**
  ******************************************************************************
  * @file    74hc595.h
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
#ifndef  __74HC595_H
#define  __74HC595_H

#include "ebox.h"

class _74hc595
{
public:

    _74hc595(Gpio *data_pin, Gpio *sck_pin, Gpio *rck_pin);
    void begin();
    void write(uint8_t *data, uint8_t data_length, uint8_t bit_oder);
    void write(uint8_t data, uint8_t bit_oder);
    void update();

private:
    Gpio *data_pin;
    Gpio *sck_pin;
    Gpio *rck_pin;


};

#endif
