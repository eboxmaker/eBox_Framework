#ifndef __EBOX_ADC_H
#define __EBOX_ADC_H
#include "ebox_core.h"
#include "mcu.h"
#include "dma.h"
class Adc
{
public:
    Adc(ADC_TypeDef *ADCx)
    {
        this->ADCx = ADCx;
    }

    void add_temp_senser();
    void add_ch(Gpio *io);
    void begin();


    uint16_t read(Gpio *io);
    uint16_t read(int index);

    float read_voltage(Gpio *io);
    float read_voltage(int index);

    float read_temp_senser();

private:
    ADC_TypeDef     *ADCx;
    Dma             *dma;
    uint32_t        dma_channel;
    void            dma_cfg();
    void            adc_cfg();

    uint8_t         ch_num;//添加通道的数量自增变量
    uint8_t         ch_table[2][16];//[0]:保存IO索引，[1]：保存IO对应adc通道索引
    uint16_t        adc_buf[18];//DMA不断的按照添加顺序将adc各个通道值。传输至此数组

};
#endif
