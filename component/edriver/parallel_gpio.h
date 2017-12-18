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

#include "ebox.h"

class ParallelGpio
{
public:
    Gpio *bit[8];
public:
    void    mode(PIN_MODE mode);
    void    write(uint8_t data);
    uint8_t read();
};

#endif
