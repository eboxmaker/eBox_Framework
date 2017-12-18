#include "ads7816.h"

void Ads7816::begin(void)
{
    cs->mode(OUTPUT_PP);
    clk->mode(OUTPUT_PP);
    din->mode(INPUT_PU);
    cs->set(); 
    clk->set(); 
}

uint16_t Ads7816::read(void)
{
    uint16_t val=0;
    cs->reset();
    delay_us(1);


    clk->reset();
    delay_us(1);//此处可测试删除来提升速度
    clk->set(); 
    delay_us(1);//此处可测试删除来提升速度
    clk->reset();
    delay_us(1);//此处可测试删除来提升速度
    clk->set();
    delay_us(1);//此处可测试删除来提升速度
    
    for(int i = 0; i < 12; i++)//下降沿读取数据
    {
        clk->reset();
        delay_us(1);//此处可测试删除来提升速度
        val |= din->read() << (11 - i);
        clk->set();
        delay_us(1);//此处可测试删除来提升速度
    }
    cs->set();
    return val;
}
float Ads7816::read_voltage(void)
{
    uint16_t val;
    float voltage;
    val = read();
    voltage = (val * 2500) / 4096.0 ; 
    return voltage;
}
