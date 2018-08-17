#ifndef __EBOX_GPIO_H
#define __EBOX_GPIO_H
#include "ebox_core.h"
#include "mcu.h"


class mcuGpio :public Gpio
{
public:
    
    mcuGpio(GPIO_TypeDef *port, uint16_t pin);

    virtual void mode(PIN_MODE mode);
    virtual void mode(PIN_MODE mode,uint8_t af_configration);
    virtual void set();
    virtual void reset();
    virtual void write(uint8_t val);
    virtual void toggle();
    virtual void read(uint8_t *val);
    virtual uint8_t read(void);
    //����������
    operator int();
    int operator =(mcuGpio&);
    mcuGpio operator = (int value);
private:
    void af_config(uint8_t af_configration);

    
//    uint32_t port_set_reg;
//    uint32_t port_reset_reg;

    GPIO_TypeDef *port; /**< ���ŵĶ˿� */
    uint16_t pin; /**< ���ŵ���� */
};


#endif
