/**
  ******************************************************************************
  * @file    object.cpp
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
#include "ebox.h"


Gpio PA0(GPIOA, GPIO_Pin_0);
Gpio PA1(GPIOA, GPIO_Pin_1);
Gpio PA2(GPIOA, GPIO_Pin_2);
Gpio PA3(GPIOA, GPIO_Pin_3);
Gpio PA4(GPIOA, GPIO_Pin_4);
Gpio PA5(GPIOA, GPIO_Pin_5);
Gpio PA6(GPIOA, GPIO_Pin_6);
Gpio PA7(GPIOA, GPIO_Pin_7);
Gpio PA8(GPIOA, GPIO_Pin_8);
Gpio PA9(GPIOA, GPIO_Pin_9);
Gpio PA10(GPIOA, GPIO_Pin_10);
Gpio PA11(GPIOA, GPIO_Pin_11);
Gpio PA12(GPIOA, GPIO_Pin_12);
Gpio PA13(GPIOA, GPIO_Pin_13);
Gpio PA14(GPIOA, GPIO_Pin_14);
Gpio PA15(GPIOA, GPIO_Pin_15);

Gpio PB0(GPIOB, GPIO_Pin_0);
Gpio PB1(GPIOB, GPIO_Pin_1);
Gpio PB2(GPIOB, GPIO_Pin_2);
Gpio PB3(GPIOB, GPIO_Pin_3);
Gpio PB4(GPIOB, GPIO_Pin_4);
Gpio PB5(GPIOB, GPIO_Pin_5);
Gpio PB6(GPIOB, GPIO_Pin_6);
Gpio PB7(GPIOB, GPIO_Pin_7);
Gpio PB8(GPIOB, GPIO_Pin_8);
Gpio PB9(GPIOB, GPIO_Pin_9);
Gpio PB10(GPIOB, GPIO_Pin_10);
Gpio PB11(GPIOB, GPIO_Pin_11);
Gpio PB12(GPIOB, GPIO_Pin_12);
Gpio PB13(GPIOB, GPIO_Pin_13);
Gpio PB14(GPIOB, GPIO_Pin_14);
Gpio PB15(GPIOB, GPIO_Pin_15);


Gpio PC0(GPIOC, GPIO_Pin_0);
Gpio PC1(GPIOC, GPIO_Pin_1);
Gpio PC2(GPIOC, GPIO_Pin_2);
Gpio PC3(GPIOC, GPIO_Pin_3);
Gpio PC4(GPIOC, GPIO_Pin_4);
Gpio PC5(GPIOC, GPIO_Pin_5);
Gpio PC6(GPIOC, GPIO_Pin_6);
Gpio PC7(GPIOC, GPIO_Pin_7);
Gpio PC8(GPIOC, GPIO_Pin_8);
Gpio PC9(GPIOC, GPIO_Pin_9);
Gpio PC10(GPIOC, GPIO_Pin_10);
Gpio PC11(GPIOC, GPIO_Pin_11);
Gpio PC12(GPIOC, GPIO_Pin_12);
Gpio PC13(GPIOC, GPIO_Pin_13);
Gpio PC14(GPIOC, GPIO_Pin_14);
Gpio PC15(GPIOC, GPIO_Pin_15);


Gpio PD0(GPIOD, GPIO_Pin_0);
Gpio PD1(GPIOD, GPIO_Pin_1);
Gpio PD2(GPIOD, GPIO_Pin_2);
Gpio PD3(GPIOD, GPIO_Pin_3);
Gpio PD4(GPIOD, GPIO_Pin_4);
Gpio PD5(GPIOD, GPIO_Pin_5);
Gpio PD6(GPIOD, GPIO_Pin_6);
Gpio PD7(GPIOD, GPIO_Pin_7);
Gpio PD8(GPIOD, GPIO_Pin_8);
Gpio PD9(GPIOD, GPIO_Pin_9);
Gpio PD10(GPIOD, GPIO_Pin_10);
Gpio PD11(GPIOD, GPIO_Pin_11);
Gpio PD12(GPIOD, GPIO_Pin_12);
Gpio PD13(GPIOD, GPIO_Pin_13);
Gpio PD14(GPIOD, GPIO_Pin_14);
Gpio PD15(GPIOD, GPIO_Pin_15);

Gpio PE0(GPIOE, GPIO_Pin_0);
Gpio PE1(GPIOE, GPIO_Pin_1);
Gpio PE2(GPIOE, GPIO_Pin_2);
Gpio PE3(GPIOE, GPIO_Pin_3);
Gpio PE4(GPIOE, GPIO_Pin_4);
Gpio PE5(GPIOE, GPIO_Pin_5);
Gpio PE6(GPIOE, GPIO_Pin_6);
Gpio PE7(GPIOE, GPIO_Pin_7);
Gpio PE8(GPIOE, GPIO_Pin_8);
Gpio PE9(GPIOE, GPIO_Pin_9);
Gpio PE10(GPIOE, GPIO_Pin_10);
Gpio PE11(GPIOE, GPIO_Pin_11);
Gpio PE12(GPIOE, GPIO_Pin_12);
Gpio PE13(GPIOE, GPIO_Pin_13);
Gpio PE14(GPIOE, GPIO_Pin_14);
Gpio PE15(GPIOE, GPIO_Pin_15);

Gpio PF0(GPIOF, GPIO_Pin_0);
Gpio PF1(GPIOF, GPIO_Pin_1);
Gpio PF2(GPIOF, GPIO_Pin_2);
Gpio PF3(GPIOF, GPIO_Pin_3);
Gpio PF4(GPIOF, GPIO_Pin_4);
Gpio PF5(GPIOF, GPIO_Pin_5);
Gpio PF6(GPIOF, GPIO_Pin_6);
Gpio PF7(GPIOF, GPIO_Pin_7);
Gpio PF8(GPIOF, GPIO_Pin_8);
Gpio PF9(GPIOF, GPIO_Pin_9);
Gpio PF10(GPIOF, GPIO_Pin_10);
Gpio PF11(GPIOF, GPIO_Pin_11);
Gpio PF12(GPIOF, GPIO_Pin_12);
Gpio PF13(GPIOF, GPIO_Pin_13);
Gpio PF14(GPIOF, GPIO_Pin_14);
Gpio PF15(GPIOF, GPIO_Pin_15);

Gpio PG0(GPIOG, GPIO_Pin_0);
Gpio PG1(GPIOG, GPIO_Pin_1);
Gpio PG2(GPIOG, GPIO_Pin_2);
Gpio PG3(GPIOG, GPIO_Pin_3);
Gpio PG4(GPIOG, GPIO_Pin_4);
Gpio PG5(GPIOG, GPIO_Pin_5);
Gpio PG6(GPIOG, GPIO_Pin_6);
Gpio PG7(GPIOG, GPIO_Pin_7);
Gpio PG8(GPIOG, GPIO_Pin_8);
Gpio PG9(GPIOG, GPIO_Pin_9);
Gpio PG10(GPIOG, GPIO_Pin_10);
Gpio PG11(GPIOG, GPIO_Pin_11);
Gpio PG12(GPIOG, GPIO_Pin_12);
Gpio PG13(GPIOG, GPIO_Pin_13);
Gpio PG14(GPIOG, GPIO_Pin_14);
Gpio PG15(GPIOG, GPIO_Pin_15);

//´®¿Ú
Uart uart1(USART1, &PA9, &PA10);
Uart uart2(USART2, &PA2, &PA3);
Uart uart3(USART3, &PB10, &PB11);
Uart uart4(UART4, &PC10, &PC11);
Uart uart5(UART5, &PC12, &PD2);

//spi
Spi     spi1(SPI1, &PA5, &PA6, &PA7);
Spi     spi2(SPI2, &PB13, &PB14, &PB15);
Spi     spi3(SPI3, &PB3, &PB4, &PB5);
SoftSpi	sspi1(&PA5, &PA6, &PA7);

//i2c
I2c     i2c1(I2C1, &PB6, &PB7);
I2c     i2c2(I2C2, &PB10, &PB11);
SoftI2c si2c(&PA4, &PA5);
SoftI2c si2c1(&PB6, &PB7);
SoftI2c si2c2(&PB10, &PB11);

//can
Can can1(&PB8, &PB9);
