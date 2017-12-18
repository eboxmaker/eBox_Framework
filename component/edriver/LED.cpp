/**
  ******************************************************************************
  * @file    led.cpp
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
#include "led.h"

Led::Led(Gpio *pin,uint8_t high_on) 
{
    this->pin       = pin;
    this->high_on   = high_on;  
};
void Led::begin()
{
    pin->mode(OUTPUT_PP);
}
void Led::on()
{
    if(high_on)
        pin->set();
    else
        pin->reset();
}
void Led::off()
{
    if(high_on)
        pin->reset();
    else
        pin->set();

}
void Led::toggle()
{
    pin->toggle();
}
