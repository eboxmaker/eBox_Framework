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

////´®¿Ú
// tx,rx
Uart uart1(USART1, &PA9, &PA10);
Uart uart2(USART2, &PA2, &PA3);
//Uart uart3(USART3, &PB10, &PB11);

//#if (MCU_PINS >= 100)
//Uart uart4(UART4, &PC10, &PC11);
//Uart uart5(UART5, &PC12, &PD2);
//#endif 

////spi
mcuSpi  spi1(SPI1, &PA5, &PA6, &PA7);
//mcuSpi  spi2(SPI2, &PB13, &PB14, &PB15);
//mcuSpi  spi3(SPI3, &PB3, &PB4, &PB5);
SoftSpi	sspi1(&PA5, &PA6, &PA7);
//SoftSpi	sspi2(&PB13, &PB14, &PB15);

//SlaveSpi slave_spi1(SPI1, &PA5, &PA6, &PA7);

//i2c

mcuI2c  i2c1(I2C1, &PB8, &PB9);
//mcuI2c  i2c2(I2C2, &PB10, &PB11);
//SoftI2c si2c(&PA4, &PA5);
SoftI2c si2c1(&PB8, &PB9);
//SoftI2c si2c2(&PB10, &PB11);


mcuGpio PA0(GPIOA, LL_GPIO_PIN_0);
mcuGpio PA1(GPIOA, LL_GPIO_PIN_1);
mcuGpio PA2(GPIOA, LL_GPIO_PIN_2);
mcuGpio PA3(GPIOA, LL_GPIO_PIN_3);
mcuGpio PA4(GPIOA, LL_GPIO_PIN_4);
mcuGpio PA5(GPIOA, LL_GPIO_PIN_5);
mcuGpio PA6(GPIOA, LL_GPIO_PIN_6);
mcuGpio PA7(GPIOA, LL_GPIO_PIN_7);
mcuGpio PA8(GPIOA, LL_GPIO_PIN_8);
mcuGpio PA9(GPIOA, LL_GPIO_PIN_9);
mcuGpio PA10(GPIOA, LL_GPIO_PIN_10);
mcuGpio PA11(GPIOA, LL_GPIO_PIN_11);
mcuGpio PA12(GPIOA, LL_GPIO_PIN_12);
mcuGpio PA13(GPIOA, LL_GPIO_PIN_13);
mcuGpio PA14(GPIOA, LL_GPIO_PIN_14);
mcuGpio PA15(GPIOA, LL_GPIO_PIN_15);

mcuGpio PB0(GPIOB, LL_GPIO_PIN_0);
mcuGpio PB1(GPIOB, LL_GPIO_PIN_1);
mcuGpio PB2(GPIOB, LL_GPIO_PIN_2);
mcuGpio PB3(GPIOB, LL_GPIO_PIN_3);
mcuGpio PB4(GPIOB, LL_GPIO_PIN_4);
mcuGpio PB5(GPIOB, LL_GPIO_PIN_5);
mcuGpio PB6(GPIOB, LL_GPIO_PIN_6);
mcuGpio PB7(GPIOB, LL_GPIO_PIN_7);
mcuGpio PB8(GPIOB, LL_GPIO_PIN_8);
mcuGpio PB9(GPIOB, LL_GPIO_PIN_9);
mcuGpio PB10(GPIOB, LL_GPIO_PIN_10);
mcuGpio PB11(GPIOB, LL_GPIO_PIN_11);
mcuGpio PB12(GPIOB, LL_GPIO_PIN_12);
mcuGpio PB13(GPIOB, LL_GPIO_PIN_13);
mcuGpio PB14(GPIOB, LL_GPIO_PIN_14);
mcuGpio PB15(GPIOB, LL_GPIO_PIN_15);


#if (MCU_PINS >= 64)
mcuGpio PC0(GPIOC, LL_GPIO_PIN_0);
mcuGpio PC1(GPIOC, LL_GPIO_PIN_1);
mcuGpio PC2(GPIOC, LL_GPIO_PIN_2);
mcuGpio PC3(GPIOC, LL_GPIO_PIN_3);
mcuGpio PC4(GPIOC, LL_GPIO_PIN_4);
mcuGpio PC5(GPIOC, LL_GPIO_PIN_5);
mcuGpio PC6(GPIOC, LL_GPIO_PIN_6);
mcuGpio PC7(GPIOC, LL_GPIO_PIN_7);
mcuGpio PC8(GPIOC, LL_GPIO_PIN_8);
mcuGpio PC9(GPIOC, LL_GPIO_PIN_9);
mcuGpio PC10(GPIOC, LL_GPIO_PIN_10);
mcuGpio PC11(GPIOC, LL_GPIO_PIN_11);
mcuGpio PC12(GPIOC, LL_GPIO_PIN_12);
#endif
mcuGpio PC13(GPIOC, LL_GPIO_PIN_13);
mcuGpio PC14(GPIOC, LL_GPIO_PIN_14);
mcuGpio PC15(GPIOC, LL_GPIO_PIN_15);





#if (MCU_PINS >= 64)
mcuGpio PD0(GPIOD, LL_GPIO_PIN_0);
mcuGpio PD1(GPIOD, LL_GPIO_PIN_1);
#endif
#if (MCU_PINS >= 100)
mcuGpio PD2(GPIOD, LL_GPIO_PIN_2);
mcuGpio PD3(GPIOD, LL_GPIO_PIN_3);
mcuGpio PD4(GPIOD, LL_GPIO_PIN_4);
mcuGpio PD5(GPIOD, LL_GPIO_PIN_5);
mcuGpio PD6(GPIOD, LL_GPIO_PIN_6);
mcuGpio PD7(GPIOD, LL_GPIO_PIN_7);
mcuGpio PD8(GPIOD, LL_GPIO_PIN_8);
mcuGpio PD9(GPIOD, LL_GPIO_PIN_9);
mcuGpio PD10(GPIOD, LL_GPIO_PIN_10);
mcuGpio PD11(GPIOD, LL_GPIO_PIN_11);
mcuGpio PD12(GPIOD, LL_GPIO_PIN_12);
mcuGpio PD13(GPIOD, LL_GPIO_PIN_13);
mcuGpio PD14(GPIOD, LL_GPIO_PIN_14);
mcuGpio PD15(GPIOD, LL_GPIO_PIN_15);


mcuGpio PE0(GPIOE, LL_GPIO_PIN_0);
mcuGpio PE1(GPIOE, LL_GPIO_PIN_1);
mcuGpio PE2(GPIOE, LL_GPIO_PIN_2);
mcuGpio PE3(GPIOE, LL_GPIO_PIN_3);
mcuGpio PE4(GPIOE, LL_GPIO_PIN_4);
mcuGpio PE5(GPIOE, LL_GPIO_PIN_5);
mcuGpio PE6(GPIOE, LL_GPIO_PIN_6);
mcuGpio PE7(GPIOE, LL_GPIO_PIN_7);
mcuGpio PE8(GPIOE, LL_GPIO_PIN_8);
mcuGpio PE9(GPIOE, LL_GPIO_PIN_9);
mcuGpio PE10(GPIOE, LL_GPIO_PIN_10);
mcuGpio PE11(GPIOE, LL_GPIO_PIN_11);
mcuGpio PE12(GPIOE, LL_GPIO_PIN_12);
mcuGpio PE13(GPIOE, LL_GPIO_PIN_13);
mcuGpio PE14(GPIOE, LL_GPIO_PIN_14);
mcuGpio PE15(GPIOE, LL_GPIO_PIN_15);


#endif

#if (MCU_PINS >= 144)
mcuGpio PF0(GPIOF, LL_GPIO_PIN_0);
mcuGpio PF1(GPIOF, LL_GPIO_PIN_1);
mcuGpio PF2(GPIOF, LL_GPIO_PIN_2);
mcuGpio PF3(GPIOF, LL_GPIO_PIN_3);
mcuGpio PF4(GPIOF, LL_GPIO_PIN_4);
mcuGpio PF5(GPIOF, LL_GPIO_PIN_5);
mcuGpio PF6(GPIOF, LL_GPIO_PIN_6);
mcuGpio PF7(GPIOF, LL_GPIO_PIN_7);
mcuGpio PF8(GPIOF, LL_GPIO_PIN_8);
mcuGpio PF9(GPIOF, LL_GPIO_PIN_9);
mcuGpio PF10(GPIOF, LL_GPIO_PIN_10);
mcuGpio PF11(GPIOF, LL_GPIO_PIN_11);
mcuGpio PF12(GPIOF, LL_GPIO_PIN_12);
mcuGpio PF13(GPIOF, LL_GPIO_PIN_13);
mcuGpio PF14(GPIOF, LL_GPIO_PIN_14);
mcuGpio PF15(GPIOF, LL_GPIO_PIN_15);

mcuGpio PG0(GPIOG, LL_GPIO_PIN_0);
mcuGpio PG1(GPIOG, LL_GPIO_PIN_1);
mcuGpio PG2(GPIOG, LL_GPIO_PIN_2);
mcuGpio PG3(GPIOG, LL_GPIO_PIN_3);
mcuGpio PG4(GPIOG, LL_GPIO_PIN_4);
mcuGpio PG5(GPIOG, LL_GPIO_PIN_5);
mcuGpio PG6(GPIOG, LL_GPIO_PIN_6);
mcuGpio PG7(GPIOG, LL_GPIO_PIN_7);
mcuGpio PG8(GPIOG, LL_GPIO_PIN_8);
mcuGpio PG9(GPIOG, LL_GPIO_PIN_9);
mcuGpio PG10(GPIOG, LL_GPIO_PIN_10);
mcuGpio PG11(GPIOG, LL_GPIO_PIN_11);
mcuGpio PG12(GPIOG, LL_GPIO_PIN_12);
mcuGpio PG13(GPIOG, LL_GPIO_PIN_13);
mcuGpio PG14(GPIOG, LL_GPIO_PIN_14);
mcuGpio PG15(GPIOG, LL_GPIO_PIN_15);

#endif 
