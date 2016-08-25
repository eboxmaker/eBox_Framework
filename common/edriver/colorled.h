/**
  ******************************************************************************
  * @file    colorled.h
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
#ifndef __COLORLED_H
#define __COLORLED_H
#include "ebox.h"
#include "color_convert.h"


class COLORLED
{

public:
    COLORLED(Gpio *r, Gpio *g, Gpio *b)
    {
        this->r_pin = r;
        this->g_pin = g;
        this->b_pin = b;
    }
    void begin();
    void color_rgb(uint8_t r, uint8_t g, uint8_t b);
    void color_hsl(int h, float s, float l);
    void color_hsl(COLOR_HSL &hsl);

    void color_hsv(int h, float s, float v);
    void color_hsv(COLOR_HSV &hsv);
    void color_change_to();
private:
    Gpio *r_pin;
    Gpio *g_pin;
    Gpio *b_pin;
    Pwm *r;
    Pwm *g;
    Pwm *b;



};
#endif
