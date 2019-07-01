#include "ir_encoder.h"
/*
 * ir_encoder.cpp
 *
 * Version 1.0
 *
 * NEC IR encoder library for eBOX.
 *
 * See
 *
 * License:   GPL v3 (http://www.gnu.org/licenses/gpl.html)
 * Datasheet:
 *
 * Modification History:
 * -Link                                  - Version 1.0 (2016/01/06)
 *
 *
 *
 *
 */

IrEncoder::IrEncoder(Gpio &io)
{
    pwm = new Pwm(&io);
}


void IrEncoder::begin()
{
    pwm->begin(38000,0);
}

void IrEncoder::send(uint8_t addr,uint8_t value)
{

    //9ms
    pwm->set_duty(325);
    delay_ms(9);
    //4.5ms
    pwm->set_duty(0);
    delay_ms(4);
    delay_us(500);

    send_byte(addr);	 // addr
    send_byte(~addr);	 // addr/
    send_byte(value);	 // cmd
    send_byte(~value);	 //cmd/

    pwm->set_duty(325);//end of last bit
    delay_us(560);
    pwm->set_duty(0);
    
    
}

void IrEncoder::send(uint32_t data)
{
    //9ms
    pwm->set_duty(325);
    delay_ms(9);
    //4.5ms
    pwm->set_duty(0);
    delay_ms(4);
    delay_us(500);

    send_byte((uint8_t)((data >> 24) & 0xff));	 // addr
    send_byte((uint8_t)((data >> 16) & 0xff));	 // addr/
    send_byte((uint8_t)((data >> 8) & 0xff));	 // cmd
    send_byte((uint8_t)(data & 0xff));	 //cmd/

    pwm->set_duty(325);//end of last bit
    delay_us(560);
    pwm->set_duty(0);
}


void IrEncoder::send_byte(uint8_t data)
{
    uint8_t i;
    for(i = 0; i < 8; i++)
    {
        pwm->set_duty(325);
        delay_us(560);
        if(data & 0x01)
        {
            //1
            pwm->set_duty(0);
            delay_us(1680);
        }
        else
        {
            //0
            pwm->set_duty(0);
            delay_us(560);
        }
        data = data >> 1;
    }
}
