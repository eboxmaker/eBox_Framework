#include "firmata_io.h"



#define PWM_NUM  4

Pwm pwm[PWM_NUM] =
{
    Pwm(&PB6),
    Pwm(&PB7),
    Pwm(&PB8),
    Pwm(&PB9),
};


Adc adc1(ADC1);


void firmata_io_init()
{
    for(int i = 0; i < PWM_NUM; i++)
    {
        pwm[i].begin(1000, 100);
        pwm[i].disable_pin();
    }

    adc1.add_ch(&PA0);
    adc1.add_ch(&PA1);
    adc1.add_ch(&PA2);
    adc1.add_ch(&PA3);
    adc1.add_ch(&PA4);
    adc1.add_ch(&PA5);
    adc1.add_ch(&PA6);
    adc1.add_ch(&PA7);
    adc1.begin();
    PA8.mode(INPUT);

}
Gpio *objectGpioTable[32] =
{
    &PA0, &PA1, &PA2, &PA3, &PA4, &PA5, &PA6, &PA7, &PA8, &PA9, &PA10, &PA11, &PA12, &PA13, &PA14, &PA15,
    &PB0, &PB1, &PB2, &PB3, &PB4, &PB5, &PB6, &PB7, &PB8, &PB9, &PB10, &PB11, &PB12, &PB13, &PB14, &PB15,
};

Gpio *digitalPinToObjectGpio(uint8_t pin)
{
    if(pin > 31) return 0;
    return objectGpioTable[pin];
}

void pinMode(uint8_t pin, uint8_t mode)
{
    Gpio *io;
    io = digitalPinToObjectGpio(pin);
    io->mode((PIN_MODE)mode);
}
void digitalWrite(uint8_t pin, uint8_t val)
{
    Gpio *io;
    io = digitalPinToObjectGpio(pin);
    io->mode((PIN_MODE)OUTPUT);
    io->write(val);

}

int digitalRead(uint8_t pin)
{
    Gpio *io;
    io = digitalPinToObjectGpio(pin);
    io->mode((PIN_MODE)INPUT);
    return io->read();
}

int analogRead(uint8_t pin)
{
    Gpio *io;
    io = digitalPinToObjectGpio(pin);
    io->mode((PIN_MODE)AIN);
    delay(1);
    return adc1.read(io);
}
void analogReference(uint8_t mode)
{


}
void analogWrite(uint8_t pin, int value)
{
    Gpio *io;
    io = digitalPinToObjectGpio(pin);
    io->mode((PIN_MODE)AF_PP);

    for(int i = 0; i < PWM_NUM; i++)
    {
        if(pwm[i].pin->id == io->id)
            pwm[i].set_duty(value);
    }

}
