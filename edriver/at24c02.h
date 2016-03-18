/*
file   : at24c02.h
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/

#ifndef __AT24C02_H
#define __AT24C02_H
#include "ebox.h"

#define AT24C02
#define SLAVE_ADDR 0xA0

#if defined (AT24C02)
#define PAGE_SIZE    8
#elif defined (AT24C04)||(AT24C08)||(AT24C016)
#define PAGE_SIZE    16
#elif defined (AT24C32)
#define PAGE_SIZE    32
#endif

class EEPROM
{
public:
    EEPROM(I2C *i2c)
    {
        this->i2c = i2c;
    }
    void begin(uint32_t speed);

    int8_t write_byte(uint16_t byte_addr, uint8_t byte);
    int8_t write_byte(uint16_t byte_addr, uint8_t *buf, uint16_t num_to_write);

    uint8_t read_byte(uint16_t byte_addr);
    int8_t	read_byte(uint16_t byte_addr, uint8_t *buf, uint16_t num_to_read);

    int8_t write_page(uint8_t *buf, uint32_t addr, uint8_t num_to_write);
private:
    I2C *i2c;
    uint32_t speed;



};

#endif
