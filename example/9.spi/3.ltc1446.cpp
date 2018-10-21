/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox


#include "ebox.h"
#include "bsp_ebox.h"

#include "ltc1446.h"



/**
	*	1	此例程需要调用eDrive目录下的LTCX驱动
	*	2	此例程演示了外部DAC（LTC系列）的基本功能
	*/



/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"LTCX example"
#define EXAMPLE_DATE	"2018-08-11"

#define ADCA	PA0
#define ADCB	PA1

mcuSpi sp(SPI1,&PB3,&PA6,&PA7);

LTCX ltc1(&PA6, &sp);
//LTCX ltc1(&PA6, &spi1);
//LTCX ltc1(&PA6, &sspi1);
Adc adc(ADC1);

void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME,EXAMPLE_DATE);
    ltc1.begin();
    

    adc.add_ch(&ADCA);
    adc.add_ch(&ADCB);
    adc.begin();
}
int main(void)
{
    uint8_t tmp[3];
    uint32_t _DAC_volue; 
    uint32_t i = 2048, t = 0;
    setup();

    while(1)
    {
        if(i != t)
        {
            i--;
//            _DAC_volue = (4096 - i) << 12 | (i & 0x0fff);
//            tmp[0] = ((uint8_t *)(&(_DAC_volue)))[2];
//            tmp[1] = ((uint8_t *)(&(_DAC_volue)))[1];
//            tmp[2] = ((uint8_t *)(&(_DAC_volue)))[0];
//            PA6.reset();
//            sspi1.writeBuf(tmp,3);
//            PA6.set();
            ltc1.write(4096 - i, i);
            UART.printf("通道A电压 = %.01f mv; 通道B电压 = %.01f mv \r\n", adc.read_voltage(&ADCA), adc.read_voltage(&ADCB));
            delay_ms(1000);
        }
    }
}

