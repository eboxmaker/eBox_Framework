/**
  ******************************************************************************
  * @file    encoder.h
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

#ifndef __ENCODER_H
#define __ENCODER_H
#include "ebox.h"
#include "button.h"
class Encoder
{
public:
    Encoder(Gpio *Apin, Gpio *Bpin);
    int read_encoder();


private:

    Gpio *a_pin;
    Gpio *b_pin;

    uint8_t a_state;
    uint8_t a_last_state;

    uint8_t b_state;
    uint8_t b_last_state;

};
#endif
