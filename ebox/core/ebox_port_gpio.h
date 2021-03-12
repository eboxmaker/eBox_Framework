#ifndef __EBOX_PORT_GPIO_H
#define __EBOX_PORT_GPIO_H
#include "ebox_type.h"

typedef enum
{
    OUTPUT      = 0x00,

    OUTPUT_PP,
    OUTPUT_PP_PU,
    OUTPUT_PP_PD,

    OUTPUT_OD,
    OUTPUT_OD_PU,
    OUTPUT_OD_PD,

    AIN,

    INPUT,
    INPUT_PD,
    INPUT_PU,

    AF_OD,
    AF_OD_PU,
    AF_OD_PD,

    AF_PP,
    AF_PP_PU,
    AF_PP_PD,
} PinMode_t;

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

    PH0_ID = 0x70,  PH1_ID,     PH2_ID,     PH3_ID,     PH4_ID,     PH5_ID,     PH6_ID,     PH7_ID,
    PH8_ID,         PH9_ID,     PH10_ID,    PH11_ID,    PH12_ID,    PH13_ID,    PH14_ID,    PH15_ID,

    PI0_ID = 0x80,  PI1_ID,     PI2_ID,     PI3_ID,     PI4_ID,     PI5_ID,     PI6_ID,     PI7_ID,
    PI8_ID,         PI9_ID,     PI10_ID,    PI11_ID,    PI12_ID,    PI13_ID,    PI14_ID,    PI15_ID,
    P_NC = 0xff
} PinId_t; ///<引脚的名字

class Gpio
{
public:
    Gpio() {};
    virtual void mode(PinMode_t mode) = 0;
    virtual void mode(PinMode_t mode, uint8_t af_configration) = 0;
    virtual void set() = 0;
    virtual void reset() = 0;

    virtual void write(uint8_t val) = 0;
    virtual void read(uint8_t *val) = 0;
    virtual uint8_t read(void) = 0;
    virtual void toggle() = 0;

    PinId_t id;
};

#endif
