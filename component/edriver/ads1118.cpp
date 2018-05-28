#include "ads1118.h"

Ads1118::Ads1118(Gpio *cs,Spi *spi)
{
    this->cs = cs;
    this->spi = spi;
    if(spi == &spi1)
    {
        miso = &PA6;
    }
    cfg.bit.ss = 0;
    cfg.bit.mux = AIN0;
    cfg.bit.pga = PGA6144;
    cfg.bit.mode = MODE_CONTINU;
    
    cfg.bit.dr = DR_128;
    cfg.bit.ts_mode = TS_MODE_ADC;
    cfg.bit.pu_en = PU_EN;
    
    cfg.bit.nop = NOP_A;
    cfg.bit.reserv = 0;
}
void Ads1118::begin(uint8_t dev_num)
{
    AdsConfig_t temp_cfg;
    config.dev_num = dev_num;
    config.mode = SPI_MODE0;
    config.prescaler = SPI_CLOCK_DIV4;
    config.bit_order = MSB_FIRST;

    spi->begin(&config);
    cs->mode(OUTPUT_PP);
    cs->set();
    
    temp_cfg = update_cfg(&cfg);
    if(temp_cfg.value != cfg.value)
    {
        uart1.printf("config failed\r\n");
    }
}
bool Ads1118::self_test()
{
    AdsConfig_t temp_cfg = cfg;
    if(update_cfg(&temp_cfg).value == temp_cfg.value)
    {
        return true;
    }
    else
        return false;
}
uint16_t Ads1118::read(uint8_t ch)
{
    uint16_t value;
    //更新配置
    if(cfg.bit.mux != ch)
    {
        cfg.bit.mux = ch;
        update_cfg(&cfg);
    }
    
    //读取相应通道值
    spi->take_spi_right(&config);
    cs->reset();
    while(miso->read() == 1);
    //delay_us(1);
    value |= spi->read() << 8;
    value |= spi->read();
    cs->set();
    spi->release_spi_right();
    return value;
}
AdsConfig_t Ads1118::update_cfg(AdsConfig_t *cfg)
{
    
    AdsConfig_t temp;
    spi->take_spi_right(&config);
    cs->reset();
    while(miso->read() == 1);
    //delay_us(1);
    spi->write(cfg->byte[0]);
    spi->write(cfg->byte[1]);
    temp.byte[0] = spi->read();
    temp.byte[1] = spi->read();
    
    cs->set();
    spi->release_spi_right();
    return temp;
}
float Ads1118::read_voltage(uint8_t ch)
{
    uint16_t temp;
    float gain;
    float voltage;
    temp = read(ch);
    float gain_table[8] = {6.144, 4.096, 2.048, 1.024, 0.512, 0.256, 0.256, 0.256};
    gain = gain_table[cfg.bit.pga];
    //temp -=32768;
    voltage = (float)temp*gain/(float)32768;
    return voltage;
}

void Ads1118::read_temperature()
{

}
void Ads1118::set_gain()
{

}