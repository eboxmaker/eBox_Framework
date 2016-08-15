/**
  ******************************************************************************
  * @file    button.h
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

#ifndef __Button_h
#define __Button_h

#include "ebox.h"

class Button
{
public:
    Button(Gpio *pin, uint8_t pull_up);
    void begin();
    uint8_t loop(void);
    uint8_t release(void);
    uint8_t click(void);
    uint8_t pressed_for(uint32_t ms, uint8_t times);

private:
    Gpio    *pin;           //arduino pin number
    uint8_t pull_up;      //internal pullup resistor enabled
    uint8_t state;         //current button state
    uint8_t last_state;     //previous button state
    uint8_t changed;       //state changed since last read
    uint8_t long_press_flag;
    uint8_t long_press_times;
    uint32_t time;         //time of current state (all times are in ms)
    uint32_t last_change;   //time of last state change
    //uint32_t _dbTime;       //debounce time

};
#endif
