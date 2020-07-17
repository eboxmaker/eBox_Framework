#include "ADS8866.h"

Ads8866::Ads8866(Gpio *clk, Gpio *data, Gpio *convst)
{
    this->clk = clk;
    this->data = data;
    this->convst = convst;
}
void Ads8866::begin()
{
    clk->mode(OUTPUT_PP);
    data->mode(INPUT);
    convst->mode(OUTPUT_PP);

}
uint16_t Ads8866::read()
{
    uint16_t value = 0;
    convst->set();
         delay_us(1);
   for(int i = 0; i < 128; i++)
    {
        clk->set();
        delay_us(1);
        clk->reset();
        delay_us(1);
    }
    convst->reset();
    delay_us(1);
    for(int i = 0; i < 16; i++)
    {
        clk->set();
        delay_us(1);
        clk->reset();
        delay_us(1);
        value = value << 1;
        value |= data->read();
    }
    return value;
}
