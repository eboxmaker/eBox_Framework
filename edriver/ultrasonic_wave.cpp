#include "ultrasonic_wave.h"

ULTRA ultra(&PA0,&PA1);



void mesure_high()
{
    if(ultra.polarity == TIM_ICPOLARITY_FALLING)//检测到下降沿，测量高电平时间完成)
    {
        ultra.wave_time = ultra.get_zone_time_us();
        ultra.flag = 1;
        ultra.set_polarity_rising();
    }
    else
    {
        ultra.get_zone_time_us();
        ultra.set_polarity_falling();
    }
}
void ULTRA::begin()
{
    this->triger->mode(OUTPUT_PP);
    triger->reset();
    
    IN_CAPTURE::begin(72);
    ultra.attch_ic_interrupt(mesure_high);
    
    flag = 0;
}
float ULTRA::detect()
{
    float distance;
    triger->set();
    delay_us(10);   
    triger->reset();
    while(flag == 0);
    flag = 0;
    distance = (wave_time)/58.0;
    return distance;
}

