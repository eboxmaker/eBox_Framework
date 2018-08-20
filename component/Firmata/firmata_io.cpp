#include "firmata_io.h"

UartStream Serial(&uart1);

Pwm pwm1(&PB8);
Adc adc1(ADC1);

void firmata_io_init()
{
    pwm1.begin(1000,100);
    adc1.add_ch(&PA1);
    adc1.add_ch(&PA2);
    adc1.add_temp_senser();
    adc1.begin();
  
}

void pinMode(uint8_t pin, uint8_t mode)
{
    

}
void digitalWrite(uint8_t, uint8_t)
{

}
int digitalRead(uint8_t)
{

}
float analogRead(uint8_t pin)
{
    int temp;
    switch(pin)
    {
        case 1: temp = adc1.read(&PA1);break;
        case 2: temp = adc1.read(&PA2);break;
        case 3: temp = adc1.read_temp_senser();break;
        default:temp = adc1.read_temp_senser();break;
    }
    return temp;
}
void analogReference(uint8_t mode)
{

}
void analogWrite(uint8_t pin, int value)
{
    switch(pin)
    {
        case 1: pwm1.set_duty(value);break;
        default:
            pwm1.set_duty(value);break;
    }
}
