/*
file   : object.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/


#include "ebox.h"

Uart uart1(USART1, &PA9, &PA10);
Uart uart2(USART2, &PA2, &PA3);
Uart uart3(USART3, &PB10, &PB11);
////spi
//mcuSpi  spi1(SPI1, &PA5, &PA6, &PA7);
//mcuSpi  spi2(SPI2, &PB13, &PB14, &PB15);
//mcuSpi  spi3(SPI3, &PB3, &PB4, &PB5);
//SoftSpi	sspi1(&PA5, &PA6, &PA7);
//SoftSpi	sspi2(&PB13, &PB14, &PB15);

////SlaveSpi slave_spi1(SPI1, &PA5, &PA6, &PA7);

////i2c
mcuI2c  mcuI2c1(I2C1, &PB8, &PB9);
//SoftI2c si2c(&PA4, &PA5);
//SoftI2c si2c1(&PB6, &PB7);
//SoftI2c si2c2(&PB10, &PB11);


mcuGpio PA0(PA0_ID);
mcuGpio PA1(PA1_ID);
mcuGpio PA2(PA2_ID);
mcuGpio PA3(PA3_ID);
mcuGpio PA4(PA4_ID);
mcuGpio PA5(PA5_ID);
mcuGpio PA6(PA6_ID);
mcuGpio PA7(PA7_ID);
mcuGpio PA8(PA8_ID);
mcuGpio PA9(PA9_ID);
mcuGpio PA10(PA10_ID);
mcuGpio PA11(PA11_ID);
mcuGpio PA12(PA12_ID);
mcuGpio PA13(PA13_ID);
mcuGpio PA14(PA14_ID);
mcuGpio PA15(PA15_ID);

mcuGpio PB0(PB0_ID);
mcuGpio PB1(PB1_ID);
mcuGpio PB2(PB2_ID);
mcuGpio PB3(PB3_ID);
mcuGpio PB4(PB4_ID);
mcuGpio PB5(PB5_ID);
mcuGpio PB6(PB6_ID);
mcuGpio PB7(PB7_ID);
mcuGpio PB8(PB8_ID);
mcuGpio PB9(PB9_ID);
mcuGpio PB10(PB10_ID);
mcuGpio PB11(PB11_ID);
mcuGpio PB12(PB12_ID);
mcuGpio PB13(PB13_ID);
mcuGpio PB14(PB14_ID);
mcuGpio PB15(PB15_ID);


#if (MCU_PINS >= 64)
mcuGpio PC0(PC0_ID);
mcuGpio PC1(PC1_ID);
mcuGpio PC2(PC2_ID);
mcuGpio PC3(PC3_ID);
mcuGpio PC4(PC4_ID);
mcuGpio PC5(PC5_ID);
mcuGpio PC6(PC6_ID);
mcuGpio PC7(PC7_ID);
mcuGpio PC8(PC8_ID);
mcuGpio PC9(PC9_ID);
mcuGpio PC10(PC10_ID);
mcuGpio PC11(PC11_ID);
mcuGpio PC12(PC12_ID);
#endif
mcuGpio PC13(PC13_ID);
mcuGpio PC14(PC14_ID);
mcuGpio PC15(PC15_ID);





#if (MCU_PINS >= 64)
mcuGpio PD0(PD0_ID);
mcuGpio PD1(PD1_ID);
#endif
#if (MCU_PINS >= 100)
mcuGpio PD2(PD2_ID);
mcuGpio PD3(PD3_ID);
mcuGpio PD4(PD4_ID);
mcuGpio PD5(PD5_ID);
mcuGpio PD6(PD6_ID);
mcuGpio PD7(PD7_ID);
mcuGpio PD8(PD8_ID);
mcuGpio PD9(PD9_ID);
mcuGpio PD10(PD10_ID);
mcuGpio PD11(PD11_ID);
mcuGpio PD12(PD12_ID);
mcuGpio PD13(PD13_ID);
mcuGpio PD14(PD14_ID);
mcuGpio PD15(PD15_ID);


mcuGpio PE0(PE0_ID);
mcuGpio PE1(PE1_ID);
mcuGpio PE2(PE2_ID);
mcuGpio PE3(PE3_ID);
mcuGpio PE4(PE4_ID);
mcuGpio PE5(PE5_ID);
mcuGpio PE6(PE6_ID);
mcuGpio PE7(PE7_ID);
mcuGpio PE8(PE8_ID);
mcuGpio PE9(PE9_ID);
mcuGpio PE10(PE10_ID);
mcuGpio PE11(PE11_ID);
mcuGpio PE12(PE12_ID);
mcuGpio PE13(PE13_ID);
mcuGpio PE14(PE14_ID);
mcuGpio PE15(PE15_ID);


#endif

#if (MCU_PINS >= 144)
mcuGpio PF0(PF0_ID);
mcuGpio PF1(PF1_ID);
mcuGpio PF2(PF2_ID);
mcuGpio PF3(PF3_ID);
mcuGpio PF4(PF4_ID);
mcuGpio PF5(PF5_ID);
mcuGpio PF6(PF6_ID);
mcuGpio PF7(PF7_ID);
mcuGpio PF8(PF8_ID);
mcuGpio PF9(PF9_ID);
mcuGpio PF10(PF10_ID);
mcuGpio PF11(PF11_ID);
mcuGpio PF12(PF12_ID);
mcuGpio PF13(PF13_ID);
mcuGpio PF14(PF14_ID);
mcuGpio PF15(PF15_ID);

mcuGpio PG0(PG0_ID);
mcuGpio PG1(PG1_ID);
mcuGpio PG2(PG2_ID);
mcuGpio PG3(PG3_ID);
mcuGpio PG4(PG4_ID);
mcuGpio PG5(PG5_ID);
mcuGpio PG6(PG6_ID);
mcuGpio PG7(PG7_ID);
mcuGpio PG8(PG8_ID);
mcuGpio PG9(PG9_ID);
mcuGpio PG10(PG10_ID);
mcuGpio PG11(PG11_ID);
mcuGpio PG12(PG12_ID);
mcuGpio PG13(PG13_ID);
mcuGpio PG14(PG14_ID);
mcuGpio PG15(PG15_ID);
#endif

#if (MCU_PINS >= 176)

mcuGpio PH0(PH0_ID);
mcuGpio PH1(PH1_ID);
mcuGpio PH2(PH2_ID);
mcuGpio PH3(PH3_ID);
mcuGpio PH4(PH4_ID);
mcuGpio PH5(PH5_ID);
mcuGpio PH6(PH6_ID);
mcuGpio PH7(PH7_ID);
mcuGpio PH8(PH8_ID);
mcuGpio PH9(PH9_ID);
mcuGpio PH10(PH10_ID);
mcuGpio PH11(PH11_ID);
mcuGpio PH12(PH12_ID);
mcuGpio PH13(PH13_ID);
mcuGpio PH14(PH14_ID);
mcuGpio PH15(PH15_ID);

mcuGpio PI5(PI5_ID);
mcuGpio PI6(PI6_ID);
mcuGpio PI7(PI7_ID);
#endif

