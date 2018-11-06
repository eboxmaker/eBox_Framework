#include "servo.h"

Servo::Servo(Gpio *pin, uint32_t frequency /*= 50*/, float limLowMs /*= 0.6*/, float limHighMs /*= 2.4*/) :
    pwm(pin), pct(50)
{
    //为保证一个周期足以容纳2.4ms的高电平时间，周期认为至少2.5ms，即频率最多400Hz
    limit(frequency, (uint32_t)50, (uint32_t)400);
    frq = frequency;
    float T = 1.0 / frequency * 1000;//周期，以ms为单位
    limLow = limLowMs / T * 1000;
    limHigh = limHighMs / T * 1000;
}

void Servo::begin()
{
    pwm.begin(frq, 0);
    setPct(0);
}

void Servo::setPct(float percent)
{
    limit(percent, -100.f, 100.f);
    pct = percent;
    pwm.set_duty((percent + 100) / 2 * (limHigh - limLow) / 100 + limLow);
}

float Servo::getPct()
{
    return pct;
}

