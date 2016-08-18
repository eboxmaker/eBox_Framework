#include "ultrasonic_wave.h"

void ULTRA::begin()
{
    this->triger->mode(OUTPUT_PP);
    triger->reset();
    InCapture::begin(72);
    flag = 0;
		status = 0;
}

void ULTRA::attch_mesuer_event(void(*callback)(void))
{
   InCapture::attach(callback);
}

void ULTRA::mesure_event()
{
    if(polarity == TIM_ICPOLARITY_FALLING)//检测到下降沿，测量高电平时间完成)
    {
			if(status == 1)
			{
        wave_time_us = get_zone_time_us();
        flag = 1;
				status = 0;
			}
			else
			{
				get_zone_time_us();
			}
      set_polarity_rising();
    }
    else
    {
        get_zone_time_us();
        set_polarity_falling();
				status = 1;
    }
}

void ULTRA::start()
{
    triger->set();
    delay_us(10);   
    triger->reset();
}
int ULTRA::avaliable()
{
	return flag;
}
float ULTRA::read_cm()
{
	flag = 0;
	return wave_time_us/58.0;
}
