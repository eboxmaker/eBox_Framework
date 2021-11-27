/**
  ******************************************************************************
  * @file    incapture.h
  * @author  shentq
  * @version V2.1
  * @date    2017/07/23
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
#ifndef __EBOX_ENCODER_H
#define __EBOX_ENCODER_H

#include "ebox_core.h"
#include "mcu.h"


class Encoder
{
public:
    Encoder(TIM_TypeDef *timer, Gpio *a, Gpio *b);
    Encoder(TIM_TypeDef *timer, Gpio *a);
    bool begin(uint32_t pulse_per_circle, uint8_t mode = TIM_EncoderMode_TI12);
    double read_speed();//µ¥Î»£ºpulse/second;
    uint16_t read_counter();
    bool read_direction();

private:
    void base_init(uint16_t period, uint16_t prescaler);
    void set_encoder(uint8_t mode = TIM_EncoderMode_TI12);

private:
    TIM_TypeDef *TIMx;
    Gpio        *a;
    Gpio        *b;
    uint16_t    period;
    uint64_t    last_read_speed_time;
    uint32_t    pulse_per_circle;
};

#endif
