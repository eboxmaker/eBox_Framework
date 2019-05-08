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
    *   3   DACA,输出逐渐增加，DACB输出逐渐减小
    *   4   PAO采样DACA并打印输出，PA1采样DACB并打印输出
	*/

/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"LTCX example"
#define EXAMPLE_DATE	"2018-08-11"

#define ADCA	PA0
#define ADCB	PA1

//LTCX ltc1(&PA6, &spi1);
LTCX ltc1(&PA6, &sspi1);
Adc adc(ADC1);

void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);
    ltc1.begin();


    adc.add_ch(&ADCA);
    adc.add_ch(&ADCB);
    adc.begin();
}
int main(void)
{
    uint32_t i = 0, t = 4000;
    setup();

    while(1)
    {
        if(i != t)
        {
            i+=100;
            ltc1.write(i, i);
            UART.printf("通道A电压 = %.01f mv; 通道B电压 = %.01f mv \r\n", (float)(4096 - i),(float) i);
//					            UART.printf("通道A电压 = %.01f mv; 通道B电压 = %.01f mv \r\n", adc.read_voltage(&ADCA), adc.read_voltage(&ADCB));
            delay_ms(10000);
        }
        else
        {
            i = 2048;
        }
    }
}

