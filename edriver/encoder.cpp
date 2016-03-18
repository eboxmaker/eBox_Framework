/*
file   : encoder.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentqlf@163.com.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentqlf@163.com assumes no responsibility for any errors contained herein.
*/

#include "encoder.h"





ENCODER::ENCODER(GPIO *Apin, GPIO *Bpin)
{

    this->a_pin = Apin;
    this->b_pin = Bpin;

    a_pin->mode(INPUT_PU);
    b_pin->mode(INPUT_PU);


}
int ENCODER::read_encoder()
{
    a_last_state = a_state;
    b_last_state = b_state;
    a_state = a_pin->read();
    b_state = b_pin->read();

    if((a_state == 0) && a_last_state && b_state)
    {
        return 1;
    }
    if((b_state == 0) && b_last_state && a_state)
    {
        return 2;
    }

    return 0;
}
