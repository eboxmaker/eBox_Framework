/**
  ******************************************************************************
  * @file    colorled.cpp
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
#include "colorled.h"


void COLORLED::begin()
{
    r = new Pwm(r_pin);
    g = new Pwm(g_pin);
    b = new Pwm(b_pin);
    r->begin(1000, 0);
    g->begin(1000, 0);
    b->begin(1000, 0);
    r->set_oc_polarity(0);
    g->set_oc_polarity(0);
    b->set_oc_polarity(0);
}

void COLORLED::color_rgb(uint8_t r, uint8_t g, uint8_t b)
{

    this->r->set_duty(r * 3);
    this->g->set_duty(g * 3);
    this->b->set_duty(b * 3);

}
void COLORLED::color_hsl(int h, float s, float l)
{
    COLOR_HSL hsl;
    COLOR_RGB rgb;
    hsl.h = h;
    hsl.s = s;
    hsl.l = l;

    HSL_to_RGB(hsl, rgb);

    this->r->set_duty(rgb.r * 1.9);
    this->g->set_duty(rgb.g * 3.9);
    this->b->set_duty(rgb.b * 0.6);
}
void COLORLED::color_hsl(COLOR_HSL &hsl)
{
    COLOR_RGB rgb;

    HSL_to_RGB(hsl, rgb);

    this->r->set_duty(rgb.r * 1.9);
    this->g->set_duty(rgb.g * 3.9);
    this->b->set_duty(rgb.b * 0.6);

}
void COLORLED::color_hsv(int h, float s, float v)
{
    COLOR_HSV hsv;
    COLOR_RGB rgb;
    hsv.h = h;
    hsv.s = s;
    hsv.v = v;

    HSV_to_RGB(hsv, rgb);
    this->r->set_duty(rgb.r * 1.9);
    this->g->set_duty(rgb.g * 3.9);
    this->b->set_duty(rgb.b * 0.6);
}
void COLORLED::color_hsv(COLOR_HSV &hsv)
{
    COLOR_RGB rgb;

    HSV_to_RGB(hsv, rgb);

    this->r->set_duty(rgb.r * 1.9);
    this->g->set_duty(rgb.g * 3.9);
    this->b->set_duty(rgb.b * 0.6);
}
