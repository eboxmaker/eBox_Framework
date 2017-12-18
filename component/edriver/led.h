/**
  ******************************************************************************
  * @file    led.h
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
#ifndef __LED_H
#define __LED_H
#include "ebox.h"

class Led
{

public:
    Led(Gpio *pin,uint8_t high_on);
    void begin();
    void on();
    void off();
    void toggle();
private:
    Gpio *pin;
    uint8_t high_on;
};
#endif
