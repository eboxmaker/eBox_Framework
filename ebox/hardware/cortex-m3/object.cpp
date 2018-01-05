/**
  ******************************************************************************
  * @file    object.cpp
  * @author  shentq
  * @version V2.1
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


mcuGpio PA0(GPIOA, GPIO_Pin_0);
mcuGpio PA1(GPIOA, GPIO_Pin_1);
mcuGpio PA2(GPIOA, GPIO_Pin_2);
mcuGpio PA3(GPIOA, GPIO_Pin_3);
mcuGpio PA4(GPIOA, GPIO_Pin_4);
mcuGpio PA5(GPIOA, GPIO_Pin_5);
mcuGpio PA6(GPIOA, GPIO_Pin_6);
mcuGpio PA7(GPIOA, GPIO_Pin_7);
mcuGpio PA8(GPIOA, GPIO_Pin_8);
mcuGpio PA9(GPIOA, GPIO_Pin_9);
mcuGpio PA10(GPIOA, GPIO_Pin_10);
mcuGpio PA11(GPIOA, GPIO_Pin_11);
mcuGpio PA12(GPIOA, GPIO_Pin_12);
mcuGpio PA13(GPIOA, GPIO_Pin_13);
mcuGpio PA14(GPIOA, GPIO_Pin_14);
mcuGpio PA15(GPIOA, GPIO_Pin_15);

mcuGpio PB0(GPIOB, GPIO_Pin_0);
mcuGpio PB1(GPIOB, GPIO_Pin_1);
mcuGpio PB2(GPIOB, GPIO_Pin_2);
mcuGpio PB3(GPIOB, GPIO_Pin_3);
mcuGpio PB4(GPIOB, GPIO_Pin_4);
mcuGpio PB5(GPIOB, GPIO_Pin_5);
mcuGpio PB6(GPIOB, GPIO_Pin_6);
mcuGpio PB7(GPIOB, GPIO_Pin_7);
mcuGpio PB8(GPIOB, GPIO_Pin_8);
mcuGpio PB9(GPIOB, GPIO_Pin_9);
mcuGpio PB10(GPIOB, GPIO_Pin_10);
mcuGpio PB11(GPIOB, GPIO_Pin_11);
mcuGpio PB12(GPIOB, GPIO_Pin_12);
mcuGpio PB13(GPIOB, GPIO_Pin_13);
mcuGpio PB14(GPIOB, GPIO_Pin_14);
mcuGpio PB15(GPIOB, GPIO_Pin_15);


#if (MCU_PIN_NUM >= 64)
mcuGpio PC0(GPIOC, GPIO_Pin_0);
mcuGpio PC1(GPIOC, GPIO_Pin_1);
mcuGpio PC2(GPIOC, GPIO_Pin_2);
mcuGpio PC3(GPIOC, GPIO_Pin_3);
mcuGpio PC4(GPIOC, GPIO_Pin_4);
mcuGpio PC5(GPIOC, GPIO_Pin_5);
mcuGpio PC6(GPIOC, GPIO_Pin_6);
mcuGpio PC7(GPIOC, GPIO_Pin_7);
mcuGpio PC8(GPIOC, GPIO_Pin_8);
mcuGpio PC9(GPIOC, GPIO_Pin_9);
mcuGpio PC10(GPIOC, GPIO_Pin_10);
mcuGpio PC11(GPIOC, GPIO_Pin_11);
mcuGpio PC12(GPIOC, GPIO_Pin_12);
#endif
mcuGpio PC13(GPIOC, GPIO_Pin_13);
mcuGpio PC14(GPIOC, GPIO_Pin_14);
mcuGpio PC15(GPIOC, GPIO_Pin_15);





#if (MCU_PIN_NUM >= 64)
mcuGpio PD0(GPIOD, GPIO_Pin_0);
mcuGpio PD1(GPIOD, GPIO_Pin_1);
#endif
#if (MCU_PIN_NUM >= 100)
mcuGpio PD2(GPIOD, GPIO_Pin_2);
mcuGpio PD3(GPIOD, GPIO_Pin_3);
mcuGpio PD4(GPIOD, GPIO_Pin_4);
mcuGpio PD5(GPIOD, GPIO_Pin_5);
mcuGpio PD6(GPIOD, GPIO_Pin_6);
mcuGpio PD7(GPIOD, GPIO_Pin_7);
mcuGpio PD8(GPIOD, GPIO_Pin_8);
mcuGpio PD9(GPIOD, GPIO_Pin_9);
mcuGpio PD10(GPIOD, GPIO_Pin_10);
mcuGpio PD11(GPIOD, GPIO_Pin_11);
mcuGpio PD12(GPIOD, GPIO_Pin_12);
mcuGpio PD13(GPIOD, GPIO_Pin_13);
mcuGpio PD14(GPIOD, GPIO_Pin_14);
mcuGpio PD15(GPIOD, GPIO_Pin_15);


mcuGpio PE0(GPIOE, GPIO_Pin_0);
mcuGpio PE1(GPIOE, GPIO_Pin_1);
mcuGpio PE2(GPIOE, GPIO_Pin_2);
mcuGpio PE3(GPIOE, GPIO_Pin_3);
mcuGpio PE4(GPIOE, GPIO_Pin_4);
mcuGpio PE5(GPIOE, GPIO_Pin_5);
mcuGpio PE6(GPIOE, GPIO_Pin_6);
mcuGpio PE7(GPIOE, GPIO_Pin_7);
mcuGpio PE8(GPIOE, GPIO_Pin_8);
mcuGpio PE9(GPIOE, GPIO_Pin_9);
mcuGpio PE10(GPIOE, GPIO_Pin_10);
mcuGpio PE11(GPIOE, GPIO_Pin_11);
mcuGpio PE12(GPIOE, GPIO_Pin_12);
mcuGpio PE13(GPIOE, GPIO_Pin_13);
mcuGpio PE14(GPIOE, GPIO_Pin_14);
mcuGpio PE15(GPIOE, GPIO_Pin_15);
#endif

#if (MCU_PIN_NUM >= 144)
mcuGpio PF0(GPIOF, GPIO_Pin_0);
mcuGpio PF1(GPIOF, GPIO_Pin_1);
mcuGpio PF2(GPIOF, GPIO_Pin_2);
mcuGpio PF3(GPIOF, GPIO_Pin_3);
mcuGpio PF4(GPIOF, GPIO_Pin_4);
mcuGpio PF5(GPIOF, GPIO_Pin_5);
mcuGpio PF6(GPIOF, GPIO_Pin_6);
mcuGpio PF7(GPIOF, GPIO_Pin_7);
mcuGpio PF8(GPIOF, GPIO_Pin_8);
mcuGpio PF9(GPIOF, GPIO_Pin_9);
mcuGpio PF10(GPIOF, GPIO_Pin_10);
mcuGpio PF11(GPIOF, GPIO_Pin_11);
mcuGpio PF12(GPIOF, GPIO_Pin_12);
mcuGpio PF13(GPIOF, GPIO_Pin_13);
mcuGpio PF14(GPIOF, GPIO_Pin_14);
mcuGpio PF15(GPIOF, GPIO_Pin_15);

mcuGpio PG0(GPIOG, GPIO_Pin_0);
mcuGpio PG1(GPIOG, GPIO_Pin_1);
mcuGpio PG2(GPIOG, GPIO_Pin_2);
mcuGpio PG3(GPIOG, GPIO_Pin_3);
mcuGpio PG4(GPIOG, GPIO_Pin_4);
mcuGpio PG5(GPIOG, GPIO_Pin_5);
mcuGpio PG6(GPIOG, GPIO_Pin_6);
mcuGpio PG7(GPIOG, GPIO_Pin_7);
mcuGpio PG8(GPIOG, GPIO_Pin_8);
mcuGpio PG9(GPIOG, GPIO_Pin_9);
mcuGpio PG10(GPIOG, GPIO_Pin_10);
mcuGpio PG11(GPIOG, GPIO_Pin_11);
mcuGpio PG12(GPIOG, GPIO_Pin_12);
mcuGpio PG13(GPIOG, GPIO_Pin_13);
mcuGpio PG14(GPIOG, GPIO_Pin_14);
mcuGpio PG15(GPIOG, GPIO_Pin_15);
#endif

//´®¿Ú
Uart uart1(USART1, &PA9, &PA10);
Uart uart2(USART2, &PA2, &PA3);
Uart uart3(USART3, &PB10, &PB11);

#if (MCU_PIN_NUM >= 100)
Uart uart4(UART4, &PC10, &PC11);
Uart uart5(UART5, &PC12, &PD2);
#endif 
//spi
mcuSpi  spi1(SPI1, &PA5, &PA6, &PA7);
mcuSpi  spi2(SPI2, &PB13, &PB14, &PB15);
mcuSpi  spi3(SPI3, &PB3, &PB4, &PB5);
SoftSpi	sspi1(&PA5, &PA6, &PA7);
SoftSpi	sspi2(&PB13, &PB14, &PB15);

SlaveSpi slave_spi1(SPI1, &PA5, &PA6, &PA7);

//i2c
mcuI2c  i2c1(I2C1, &PB6, &PB7);
mcuI2c  i2c2(I2C2, &PB10, &PB11);
SoftI2c si2c(&PA4, &PA5);
SoftI2c si2c1(&PB6, &PB7);
SoftI2c si2c2(&PB10, &PB11);

//can
Can can1(&PB8, &PB9);

//dma
