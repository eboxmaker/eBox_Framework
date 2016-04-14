#include "ultrasonic_wave.h"

ULTRA ultra(&PA1);

IN_CAPTURE ic_csb(&PA0);

void mesure_high()
{
    if(ic_csb.polarity == TIM_ICPOLARITY_FALLING)//检测到下降沿，测量高电平时间完成)
    {
        ultra.value = ic_csb.get_capture();
        ultra.flag = 1;
        ic_csb.set_polarity_rising();
    }
    else
    {
        ic_csb.get_capture();
        ic_csb.set_polarity_falling();
    }
}
ULTRA::ULTRA(GPIO *triger)
{
    this->triger = triger;
}
void ULTRA::begin()
{
    this->triger->mode(OUTPUT_PP);
    triger->reset();
    
    ic_csb.begin(1);
    ic_csb.attch_ic_interrupt(mesure_high);
    
    flag = 0;
}
float ULTRA::detect()
{
    triger->set();
    delay_us(10);   
    triger->reset();
    while(flag == 0);
    flag = 0;
    value = (value/72.0)/58.0;
    return value;
}

