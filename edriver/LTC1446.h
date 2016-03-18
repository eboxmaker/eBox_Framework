/*
file   : LTC1446.cpp
author : cat_li
version: V1.0
date   : 2015/11/8

Copyright 2015 cat_li. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/

#ifndef __LTC1446_H
#define __LTC1446_H

#include "ebox.h"

class LTCX
{
public:
    LTCX(GPIO *p_cs_pin, SPI *p_SPI)
    {
        cs  = p_cs_pin;
        spi = p_SPI;
    }
    void begin(uint8_t dev_num);
    void write(uint16_t _DACA, uint16_t _DACB);

private:
    SPI_CONFIG_TYPE spi_dev_LTC1446;
    GPIO *cs;
    SPI *spi;
};
#endif
