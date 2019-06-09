/**
  ******************************************************************************
  * @file    parallel_gpio.h
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

#ifndef  __PARALLEL_GPIO_H
#define  __PARALLEL_GPIO_H

#include "ebox_core.h"

class ParallelGpio
{
public:
    ParallelGpio() {};
    virtual void mode(PIN_MODE mode) = 0;

    virtual void write(uint16_t val) = 0;
    virtual void read(uint16_t *val) = 0;
    virtual uint16_t read(void) = 0;
};

class Gpio8 : public ParallelGpio
{
public:
    Gpio8(Gpio *pin0,Gpio *pin1 = 0);
    Gpio8(Gpio *pin[]);
    virtual void mode(PIN_MODE mode);
    virtual void write(uint16_t val);
    virtual void read(uint16_t *val);
    virtual uint16_t read(void);
private:
    Gpio *_bit[8];
};

#endif
