/**
  ******************************************************************************
  * @file    bsp_ebox.cpp
  * @author  cat_li	
  * @version V1.0
  * @date    2018/07/31
  * @brief   硬件相关信息声明
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
#include "bsp_ebox.h"

mcuGpio 		LED1(GPIOB,GPIO_Pin_8);

/*
Led     led2(&PB9,1);
Button  btn(&PA8, 1);
W5500   w5500(&PC13, &PC14, &PC15, &spi2);
SD      sd(&PB12, &spi2);
//Lcd     lcd(&PB5, &PB6, &PB4, &PB3, &spi1);
//W25x16  flash(&PA15, &spi1);
At24c02 eeprom(&i2c2);
*/



