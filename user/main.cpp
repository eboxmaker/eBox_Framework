/**
  ******************************************************************************
  * @file   : *.cpp
  * @author : shentq
  * @version: V1.2
  * @date   : 2016/08/14

  * @brief   ebox application example .
  *
  * Copyright 2016 shentq. All Rights Reserved.
  ******************************************************************************
 */


#include "ebox.h"
#include "ADS8866.h"
#include "bsp_ebox.h"

Ads8866 ad(&PA5,&PA6,&PA4);

void setup()
{
    int ret;
    ebox_init();
    uart1.begin(115200);
    print_log();
    PB8.mode(OUTPUT_PP);

    ad.begin();

}
int main(void)
{
    float temper;
    setup();
    uint32_t last = millis();
    uint16_t hex;
    float vol;
    while(1)
    {
        hex = ad.read();
        vol = hex * 2.5 / 65535.0 ;
        uart1.printf("adc:0X%04x,%0.6fV\r\n",hex,vol);
        delay_ms(100);
    }

}


