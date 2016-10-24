/**
  ******************************************************************************
  * @file    bsp.cpp
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


/* Includes ------------------------------------------------------------------*/
#include "bsp.h"

Led     led1(&PB8,1);
Led     led2(&PB9,1);
Button  btn(&PA8, 1);
W5500   w5500(&PC13, &PC14, &PC15, &spi2);
SD      sd(&PB12, &spi2);
Lcd     lcd(&PB5, &PB6, &PB4, &PB3, &spi1);
//W25x16  flash(&PA15, &spi1);
At24c02 eeprom(&i2c2);

