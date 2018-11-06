#include "ads1118.h"
#include "ebox.h"
Ads1118::Ads1118(Gpio *cs, Spi *spi)
{
    this->cs = cs;
    this->spi = spi;
    if(spi == &spi1)
    {
        miso = &PA6;
    }
    else if(spi == &spi2)
    {
        miso = &PB14;
    }
    cfg.value = 0x0;

    cfg.bit.ss = 0;
    cfg.bit.mux = ADC_AIN0;
    cfg.bit.pga = PGA6144;
    cfg.bit.mode = MODE_CONTINU;

    cfg.bit.dr = DR_860;
    cfg.bit.ts_mode = TS_MODE_ADC;
    cfg.bit.pu_en = PU_EN;

    cfg.bit.nop = NOP_A;
    cfg.bit.reserv = 1;



}
void Ads1118::begin(uint8_t dev_num)
{
    AdsConfig_t temp_cfg;
    config.dev_num = dev_num;
    config.mode = SPI_MODE1;
    config.prescaler = SPI_CLOCK_DIV2;
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
        return 1;
    }
    else
        return 0;
}
uint16_t Ads1118::read(uint8_t ch)
{
    uint32_t last = millis();
    uint16_t value;
    //更新配置
    if(cfg.bit.mux != ch)
    {
        cfg.bit.mux = ch;
        update_cfg(&cfg);
        delay_ms(10);
    }
    //读取相应通道值
    spi->take_spi_right(&config);
    cs->reset();
    last = millis();
    while(miso->read() == 1)
    {
        if(millis() - last > 10)
            break;
    }
    value |= spi->transfer(0xff) << 8;
    value |= spi->transfer(0xff);
    cs->set();
    spi->release_spi_right();
    return value;
}
AdsConfig_t Ads1118::update_cfg(AdsConfig_t *cfg)
{

    uint32_t last = millis();
    AdsConfig_t temp;
    spi->take_spi_right(&config);
    cs->reset();

    while(miso->read() == 1)
    {
        if(millis() - last > 10)
            break;
    }
    spi->write(cfg->byte[0]);
    spi->write(cfg->byte[1]);
    temp.byte[0] = spi->transfer(0xff);
    temp.byte[1] = spi->transfer(0xff);
    cs->set();
    spi->release_spi_right();
    return temp;
}
double Ads1118::read_voltage(uint8_t ch)
{
    uint16_t temp;
    double gain;
    double voltage;
    temp = read(ch);
    double gain_table[8] = {6144, 4096, 2048, 1024, 512, 256, 256, 256};
    gain = gain_table[cfg.bit.pga];
    //temp -=32768;
    voltage = (double)temp * gain / (double)32768;
    return voltage;
}

float Ads1118::read_average(uint8_t ch)
{
    float sum = 0;
    for(int i = 0; i < 20; i++)
    {
        sum += read(ch);
        sum = sum;
        //delay_ms(10);
        //uart3.printf("\r\n\r\n__%f___ \r\n\r\n",sum);
    }
    sum = sum / 20.0;
    return sum;
}
double Ads1118::read_vol_average(uint8_t ch)
{
    double sum = 0;
    for(int i = 0; i < 10; i++)
        sum += read_voltage(ch);
    sum = sum / 10.0;
    return sum;

}

double Ads1118::adc_to_voltage(double hex)
{
    double temp = hex;
    double gain;
    double voltage;
    double gain_table[8] = {6144, 4096, 2048, 1024, 512, 256, 256, 256};
    gain = gain_table[cfg.bit.pga];
    //temp -=32768;
    voltage = ((double)gain / (double)32768) * temp;
    return voltage;
}
void Ads1118::read_temperature()
{

}
void Ads1118::set_gain()
{

}
