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
    uint16_t val = 0;
    cs->reset();
    delay_us(1);


    clk->reset();
    delay_us(1);//�˴��ɲ���ɾ���������ٶ�
    clk->set();
    delay_us(1);//�˴��ɲ���ɾ���������ٶ�
    clk->reset();
    delay_us(1);//�˴��ɲ���ɾ���������ٶ�
    clk->set();
    delay_us(1);//�˴��ɲ���ɾ���������ٶ�

    for(int i = 0; i < 12; i++)//�½��ض�ȡ����
    {
        clk->reset();
        delay_us(1);//�˴��ɲ���ɾ���������ٶ�
        val |= din->read() << (11 - i);
        clk->set();
        delay_us(1);//�˴��ɲ���ɾ���������ٶ�
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
