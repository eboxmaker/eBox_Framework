/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox


#include "ebox.h"
#include "ir/ir_decoder.h"

IrDecoder ir_decoder(PB0);
void setup()
{
    ebox_init();
    uart1.begin(115200);
    ir_decoder.begin();
    uart1.printf("ebox ir_decoder test\r\n");
}
int main(void)
{
    setup();
    while(1)
    {
        if(ir_decoder.available())
        {
            uint32_t r = ir_decoder.read();
            uart1.printf("addr:0x%x,key_code=0x%x repeat=%d\r\n", (r &0xff0000)>>16, (r & 0xff), ((r & 0xff00) >> 8));
        }
        delay_ms(3000);
    }
}

