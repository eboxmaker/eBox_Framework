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

Lora lora(&PA4,&PA2,&spi1);

Exti ex1(&PA3,EXTI_Trigger_Rising);
Exti ex2(&PB0,EXTI_Trigger_Rising);

uint8_t sf;
LoraPack package;
uint8_t buf[8]={'1','8','3','4','5','6','7','\0'};

void test()
{
//    PB8.toggle();
//    lora.tc_evnet();
//    uart1.printf("tx over\n");

    PB9.toggle();
    lora.rx_packet(&package);
    uart1.printf("%s\n",package.data);
}
void timeout()
{
  uart1.printf("timeout\n");
  lora.clearIRQFlags();
}
void setup()
{
    int ret;
    ebox_init();
    PB8.mode(OUTPUT_PP);
    PB9.mode(OUTPUT_PP);
    uart1.begin(115200);
    lora.begin(1,SX1278_BW_6,SX1278_CR_4_5,SX1278_SF_9);
    
    ex1.begin();
    ex1.attach(test);
    ex2.begin();
    ex2.attach(timeout);
    
    package.data = buf;
    for(int i = 0; i < 8; i ++)
        package.source[i] =0; 
    for(int i = 0; i < 8; i ++)
        package.destination[i] =0; 
//    lora.enttry_tx();
    lora.enttry_rx();
}
uint32_t last;
int main(void)
{
    int temp;
    setup();
    while(1)
    {
//        if(lora.state == TXREADY)
//        {
//            uart1.println((uint32_t)(millis() - last));
//            last = millis();
//            lora.tx_packet(&package);
//            temp++;
//            temp%=10;
//            buf[0] = 0x30+temp;
//        }
    }

}
