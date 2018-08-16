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
        uint8_t         ch_num;//���ͨ����������������
        uint8_t         ch_table[2][16];//[0]:����IO������[1]������IO��Ӧadcͨ������
        uint16_t        adc_buf[18];//DMA���ϵİ������˳��adc����ͨ��ֵ��������������

};
#endif
