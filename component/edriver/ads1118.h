#ifndef __ADS1118_H
#define __ADS1118_H
#include "ebox_core.h"


#define CONFIG_DEFAULT = 0x008B;
#define CONFIG_TEMPERATURE = 0x049B;

// Bit masks
#define PIN_BITMASK = 0x7000;
#define PGA_BITMASK = 0x0E00;
typedef union
{
    struct
    {
        uint16_t mode: 1;
        uint16_t pga: 3;
        uint16_t mux: 3;
        uint16_t ss: 1;

        uint16_t reserv: 1;
        uint16_t nop: 2;
        uint16_t pu_en: 1;
        uint16_t ts_mode: 1;
        uint16_t dr: 3;
    } bit;
    uint8_t byte[2];
    uint16_t value;
} AdsConfig_t;


#define ADC_AIN0 4
#define ADC_AIN1 5
#define ADC_AIN2 6
#define ADC_AIN3 7

#define PGA6144 0
#define PGA4096 1
#define PGA2048 2
#define PGA1024 3
#define PGA512  4
#define PGA256_1  5
#define PGA256_2  6
#define PGA256_3  7

#define MODE_CONTINU    0
#define MODE_SINGAL     1

#define DR_8    0
#define DR_16   1
#define DR_32   2
#define DR_64   3
#define DR_128  4
#define DR_250  5
#define DR_475  6
#define DR_860  7

#define TS_MODE_ADC 0
#define TS_MODE_TEMP 1

#define PU_DISABLE  0
#define PU_EN       1

#define NOP_A   1
#define NOP_D   0


class Ads1118
{
public:
    AdsConfig_t cfg;
public:
    Ads1118(Gpio *cs, Spi *spi);
    void        begin(uint8_t dev_num);
    bool        self_test();
    AdsConfig_t update_cfg(AdsConfig_t *cfg);
    void        read_temperature();

    uint16_t    read(uint8_t ch);
    double      read_voltage(uint8_t ch);

    float      read_average(uint8_t ch);
    double      read_vol_average(uint8_t ch);

    double      adc_to_voltage(double hex);

    void        set_gain();

private:
    Gpio    *cs;
    Gpio    *miso;
    Spi     *spi;
    SpiConfig_t config;


};

#endif
