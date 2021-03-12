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
#include "ir/ir_decoder.h"

MideaNormal md(PA0);
IrDecoder ir_decoder(PB0);

void setup()
{
    ebox_init();
    uart1.begin(115200);
    ir_decoder.begin();

    md.begin();
    delay_ms(1000);
    md.cool_26();
    md.wind(MideaNormal::WindHigh);
    md.print(uart1);
    delay(1000);
}
uint8_t temp = 17;
uint8_t w = 0;
//+:0x15\
//- 0x07
//>>0x40
//<<0x44
//ch+0x47
//ch-0x45
int main(void)
{
    setup();
    while(1)
    {
        
        if(ir_decoder.available())
        {
            IrDecoder::IrCode_t r = ir_decoder.read();
            
            uart1.printf("addr:0x%x,key_code=0x%x repeat=%d\r\n", r.code.addr,r.code.value,r.code.is_repeat);
            
            switch(r.code.value)
            {
                case 0x15:
                    md.tempAdd();break;
                case 0x07:
                    md.tempSub();break;
                
                case 0x40:
                    md.modeNext();break;
                case 0x44:
                    md.modeNext();break;
                
                case 0x47:
                    md.windNext();break;
            }
            md.print(uart1);
        }
        
//        md.temp(random()%13 + 17);
//        md.print(uart1);
//        delay_ms(3000);

//        md.modeNext();
//        md.print(uart1);
//        delay_ms(3000);
//        
//        md.mode(MideaNormal::ModeHeat);
//        md.print(uart1);
//        delay_ms(3000);
//        
//        md.powerOff();
//        delay(3000);
    }
}




