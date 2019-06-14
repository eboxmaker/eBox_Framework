#ifndef __EBOX_GPIO_H
#define __EBOX_GPIO_H
#include "ebox_core.h"
#include "mcu.h"


class mcuGpio : public Gpio
{
public:

    mcuGpio(PIN_ID_t pin_id);		
    virtual void mode(PIN_MODE mode);
    virtual void mode(PIN_MODE mode, uint8_t af_configration);
    virtual void set();
    virtual void reset();
    virtual void write(uint8_t val);
    virtual void toggle();
    virtual void read(uint8_t *val);
    virtual uint8_t read(void);
    //操作符重载
    operator int();
    int operator =(mcuGpio &);
    mcuGpio operator = (int value);
private:
    void af_config(uint8_t af_configration);


    //    uint32_t port_set_reg;
    //    uint32_t port_reset_reg;

    GPIO_TypeDef *port;    /**< 引脚的端口 */
    uint16_t pin;          /**< 引脚的序号 */
};

void port_mode(GPIO_TypeDef* port,uint32_t pin, PIN_MODE mode);

#endif
