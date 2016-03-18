/*
file   : encoder.h
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentqlf@163.com.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentqlf@163.com assumes no responsibility for any errors contained herein.
*/

#ifndef __ENCODER_H
#define __ENCODER_H
#include "ebox.h"
#include "button.h"
class ENCODER
{
public:
    ENCODER(GPIO *Apin, GPIO *Bpin);
    int read_encoder();


private:

    GPIO *a_pin;
    GPIO *b_pin;

    uint8_t a_state;
    uint8_t a_last_state;

    uint8_t b_state;
    uint8_t b_last_state;

};
#endif
