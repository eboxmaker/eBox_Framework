/*
file   : dac.h
author : shentq
version: V1.1
date   : 2016/4/6

Copyright 2015 shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/
#ifndef __DAC_H
#define __DAC_H
#include "common.h"
class DACCLASS
{

public:
    DACCLASS(GPIO *pin);
    void begin(uint16_t *buf1, uint16_t *buf2, uint16_t buf_size);
private:
    GPIO *pin;
};
#endif
