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
#include "gui.h"
#include "ds18b20.h"
#include "menu.h"
#include "bsp.h"
#include "sx1278.h"

Lora lora(&PA4,&PA2,&PA3,&spi1);
uint8_t sf;
LoraPack package;
const char buf[8]={1,2,3,4,5,6,7,8};
void setup()
{
    int ret;
    ebox_init();
    uart1.begin(115200);
    lora.begin(1,SX1278_BW_9,SX1278_CR_4_5,SX1278_SF_6);
    sf = lora.readRegister(SX1278_REG_VERSION);
    uart1.printf("SF = 0x%x\n",sf);

    package.data = buf;
    for(int i = 0; i < 8; i ++)
        package.source[i] =0; 
    for(int i = 0; i < 8; i ++)
        package.destination[i] =0; 
}
int main(void)
{
    float temper;
    setup();
    while(1)
    {
//        lora.tx(&package);
//        delay_ms(500);
//        uart1.printf("asdf\n");
        package = *lora.rx(SX1278_RXSINGLE,8);
        uart1.printf("asdf\n");
    }

}
