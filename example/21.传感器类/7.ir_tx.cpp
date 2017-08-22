/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox


#include "ebox.h"
#include "ir_encoder.h"

PWM nec_tx(&PA0);
IR_ENCODER nec;
void setup()
{
    ebox_init();
    nec_tx.begin(38000, 0);
    nec.begin(&nec_tx);

}
int main(void)
{
    setup();
    while(1)
    {
        nec.send(0x00ff55aa);
        delay_ms(1000);
    }
}




