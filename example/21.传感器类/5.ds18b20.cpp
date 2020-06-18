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
#include "ds18b20.h"
#include "bsp_ebox.h"
Ds18b20 ds(&PA1);
bool is_exist;
void setup()
{
    int ret;
    ebox_init();
    uart1.begin(115200);
    print_log();
    PB8.mode(OUTPUT_PP);
    if(ds.begin()== false)
    {
        is_exist =  false;
        uart1.printf("Ã»ÓÐÕÒµ½DS18B20");
    }
    else
    {
        is_exist = true;
    }
    

}
int main(void)
{
    float temper;
    setup();
    uint32_t last = millis();
    while(1)
    {
        if(is_exist)
        {
            ds.loop();
        }
        if(millis() - last > 1000)
        {
            last = millis();
            uart1.printf("%f\n", ds.get_temp());   
        }

    }

}


