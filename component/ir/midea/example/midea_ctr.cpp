/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox


#include "ebox.h"
#include "ir/midea/midea.h"

MideaNormal md(PA0);

void setup()
{
    ebox_init();
    uart1.begin(115200);
    md.begin();
    delay_ms(1000);
    md.cool_26();
    md.wind(MideaNormal::WindHigh);
    md.print(uart1);
    delay(1000);
}
int main(void)
{
    setup();
    while(1)
    {
        md.temp(random()%13 + 17);
        md.print(uart1);
        delay_ms(3000);

        md.mode(MideaNormal::ModeCool);
        md.print(uart1);
        delay_ms(3000);
        
        md.mode(MideaNormal::ModeHeat);
        md.print(uart1);
        delay_ms(3000);
        
        md.powerOff();
        delay(3000);
    }
}




