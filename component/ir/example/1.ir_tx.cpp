/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox


#include "ebox.h"
#include "ir/ir_encoder.h"

IrEncoder nec(PA0);
void setup()
{
    ebox_init();
    nec.begin();

}
int main(void)
{
    setup();
    while(1)
    {
        nec.send(0x12,0x45);
        delay_ms(1000);
    }
}




