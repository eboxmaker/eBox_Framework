/**
  ******************************************************************************
  * @file    ltc1446.h
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

#ifndef __LTC1446_H
#define __LTC1446_H

#include "ebox.h"

class LTCX
{
public:
    LTCX(Gpio *p_cs_pin, Spi *p_SPI)
    {
        cs  = p_cs_pin;
        spi = p_SPI;
    }
    void begin(uint8_t dev_num);
    void write(uint16_t _DACA, uint16_t _DACB);

private:
    SPI_CONFIG_TYPE spi_dev_LTC1446;
    Gpio *cs;
    Spi *spi;
};
#endif
