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
#include "74hc595.h"

class LedSegment: public _74hc595
{

public:
    LedSegment(Gpio *dataPin, Gpio *sckPin, Gpio *rckPin): _74hc595(dataPin, sckPin, rckPin) {};
    void begin();
    void show_time_HMS(uint8_t hour, uint8_t min, uint8_t sec);
    void show_date_time(uint16_t year, uint8_t month, uint8_t date);
    void show_date(uint8_t *buf);
    void show_time(uint8_t *buf);

private:
    uint8_t led_buf[8];

};
#endif
