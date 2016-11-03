/**
  ******************************************************************************
  * @file    gpio.h
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

#ifndef __GPIO_H
#define __GPIO_H
#include "stm32f10x.h"
/*
typedef enum
{
    OUTPUT_PP    = 0x0,    
    OUTPUT_PP_PU    ,//NONE
    OUTPUT_PP_PD    ,//NONE
    
    OUTPUT_OD       ,
    OUTPUT_OD_PU    ,//NONE
    OUTPUT_OD_PD    ,//NONE
    
    AIN             ,
    
    INPUT           ,
    INPUT_PD        ,
    INPUT_PU        ,
    
    AF_OD           ,
    AF_OD_PU        ,//NONE 
    AF_OD_PD        ,//NONE
    
    AF_PP           ,      
    AF_PP_PU        ,//NONE
    AF_PP_PD        ,//NONE
} PIN_MODE;///<gpio的模式
*/
typedef enum
{
    OUTPUT_PP    = 0x0,    
    OUTPUT_PP_PU    ,    
    OUTPUT_PP_PD    ,
    
    OUTPUT_OD       ,
    OUTPUT_OD_PU    ,
    OUTPUT_OD_PD    ,
    
    AIN             ,
    
    INPUT           ,
    INPUT_PD        ,
    INPUT_PU        ,
    
    AF_OD           ,
    AF_OD_PU        ,   
    AF_OD_PD        ,
    
    AF_PP           ,      
    AF_PP_PU        ,
    AF_PP_PD        ,
}PIN_MODE;

typedef enum
{
    PA0_ID = 0,     PA1_ID,     PA2_ID,     PA3_ID,     PA4_ID,     PA5_ID,     PA6_ID,     PA7_ID,
    PA8_ID,         PA9_ID,     PA10_ID,    PA11_ID,    PA12_ID,    PA13_ID,    PA14_ID,    PA15_ID,
    
    PB0_ID = 0x10,  PB1_ID,     PB2_ID,     PB3_ID,     PB4_ID,     PB5_ID,     PB6_ID,     PB7_ID,
    PB8_ID,         PB9_ID,     PB10_ID,    PB11_ID,    PB12_ID,    PB13_ID,    PB14_ID,    PB15_ID,
            
    PC0_ID = 0x20,  PC1_ID,     PC2_ID,     PC3_ID,     PC4_ID,     PC5_ID,     PC6_ID,     PC7_ID,
    PC8_ID,         PC9_ID,     PC10_ID,    PC11_ID,    PC12_ID,    PC13_ID,    PC14_ID,    PC15_ID,
            
    PD0_ID = 0x30,  PD1_ID,     PD2_ID,     PD3_ID,     PD4_ID,     PD5_ID,     PD6_ID,     PD7_ID,
    PD8_ID,         PD9_ID,     PD10_ID,    PD11_ID,    PD12_ID,    PD13_ID,    PD14_ID,    PD15_ID,
    
    PE0_ID = 0x40,  PE1_ID,     PE2_ID,     PE3_ID,     PE4_ID,     PE5_ID,     PE6_ID,     PE7_ID,
    PE8_ID,         PE9_ID,     PE10_ID,    PE11_ID,    PE12_ID,    PE13_ID,    PE14_ID,    PE15_ID,
    
    PF0_ID = 0x50,  PF1_ID,     PF2_ID,     PF3_ID,     PF4_ID,     PF5_ID,     PF6_ID,     PF7_ID,
    PF8_ID,         PF9_ID,     PF10_ID,    PF11_ID,    PF12_ID,    PF13_ID,    PF14_ID,    PF15_ID,
    
    PG0_ID = 0x60,  PG1_ID,     PG2_ID,     PG3_ID,     PG4_ID,     PG5_ID,     PG6_ID,     PG7_ID,
    PG8_ID,         PG9_ID,     PG10_ID,    PG11_ID,    PG12_ID,    PG13_ID,    PG14_ID,    PG15_ID,
}PIN_ID_t;///<引脚的名字
class Gpio
{
public:
    Gpio(GPIO_TypeDef *port, uint16_t pin);
    void mode(PIN_MODE mode);
    void set();
    void reset();
    void write(uint8_t val);
    void toggle();
    void read(uint8_t *val);
    uint8_t read(void);
    //操作符重载

    operator int();
    int operator =(Gpio&);
    Gpio operator = (int value);

    PIN_ID_t id;
public:
    GPIO_TypeDef *port; /**< 引脚的端口 */
    uint16_t pin; /**< 引脚的序号 */
};


#endif
