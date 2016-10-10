/**
  ******************************************************************************
  * @file    analog.h
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
#ifndef __ANALOG_H
#define __ANALOG_H
#include "ebox_gpio.h"

void        ADC1_init(void);

uint16_t	analog_read(Gpio *pin);
uint16_t	analog_read_voltage(Gpio *pin);

#endif
