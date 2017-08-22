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
Ds18b20 ds(&PA0);
void setup()
{
    int ret;
    ebox_init();
    uart1.begin(115200);
    PB8.mode(OUTPUT_PP);
    ret = ds.begin();
    uart1.printf("%d\n",ret);
    
}
int main(void)
{
    float temper;
    setup();
    while(1)
    {
        temper = ds.get_temp();
        uart1.printf("%f\n",temper);
        delay_ms(1000);

    }

}


