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

// 如果不使用object.cpp, 使用下列方式创建IO对象
//mcuGpio 		LED1(GPIOB,GPIO_Pin_8);
//mcuGpio 		LED2(GPIOB,GPIO_Pin_9);
//mcuGpio 		LED3(GPIOB,GPIO_Pin_10);

//mcuGpio			BtnPin(GPIOA,GPIO_Pin_8);
//Uart 				UART(USART1, &PA9, &PA10);


// 使用object.cpp,使用下列方式重命名IO对象，可以减少code区消耗
//#define LED1 PB8
//#define LED2 PB9
//#define LED3 PB10

//#define BtnPin	PA8
//#define	UART		uart1

/*
Led     led2(&PB9,1);
Button  btn(&PA8, 1);
W5500   w5500(&PC13, &PC14, &PC15, &spi2);
SD      sd(&PB12, &spi2);
//Lcd     lcd(&PB5, &PB6, &PB4, &PB3, &spi1);
//W25x16  flash(&PA15, &spi1);
At24c02 eeprom(&i2c2);
*/



