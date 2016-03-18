/*
file   : 74hc595.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/

#ifndef  __74HC595_H
#define  __74HC595_H

#include "ebox.h"

class _74hc595
{
public:

    _74hc595(GPIO *data_pin, GPIO *sck_pin, GPIO *rck_pin);
    void begin();
    void write(uint8_t *data, uint8_t data_length, uint8_t bit_oder);
    void write(uint8_t data, uint8_t bit_oder);
    void update();

private:
    GPIO *data_pin;
    GPIO *sck_pin;
    GPIO *rck_pin;


};

#endif
