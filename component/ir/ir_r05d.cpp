#include "ir_r05d.h"



IrR05D::IrR05D(Gpio &io)
{
    pwm = new Pwm(&io);
}


void IrR05D::begin()
{
    pwm->begin(38000,0);
}
void IrR05D::send_l()
{
    pwm->set_duty(325);
    delay_us(CODE_L_L);
    pwm->set_duty(0);
    delay_us(CODE_L_H);

}
void IrR05D::send_s()
{
    pwm->set_duty(325);
    delay_us(CODE_S_L);
    pwm->set_duty(0);
    delay_us(CODE_S_H);
}
void IrR05D::send_e()
{
    //E
    pwm->set_duty(325);
    delay_us(CODE_E_L);
    pwm->set_duty(0);
    delay_us(CODE_E_H);
}

void IrR05D::send_byte(uint8_t data)
{
    uint8_t i;
    for(i = 0; i < 8; i++)
    {
        pwm->set_duty(325);
        delay_us(540);
        if(data & 0x80)
        {
            //1
            pwm->set_duty(0);
            delay_us(CODE_1_H);
        }
        else
        {
            //0
            pwm->set_duty(0);
            delay_us(CODE_0_H);
        }
        data = data << 1;
    }
}
