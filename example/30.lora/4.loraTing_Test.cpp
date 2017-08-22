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
#include "bsp.h"
#include "sx1278.h"


#define TXMODE 1

Lora lora(&PA4,&PA2,&spi1);

Exti ex1(&PA3,EXTI_Trigger_Rising);
Exti ex2(&PB0,EXTI_Trigger_Rising);


uint8_t buf[8]={0xff,0xff,0xff,0xff,'5','6','7','8'};


void test()
{
#if TXMODE
    PB8.toggle();
    lora.evnet_tc();
    uart1.printf("tx over\n");
#else
    uint8_t len;
    PB9.toggle();
    lora.read(buf,&len);
    uart1.write("====\n");
    uart1.write(buf,len);
		for(int i = 0;i < 8; i++)
		{
			buf[i] = 0;
		}
    uart1.write("\n====\n");
#endif
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
    PA1.mode(OUTPUT_PP);
    PA1.reset();
    PB8.mode(OUTPUT_PP);
    PB9.mode(OUTPUT_PP);
    uart1.begin(115200);
    lora.begin(1,SX1278_BW_6,SX1278_CR_4_5,SX1278_SF_10);
    
    ex1.begin();
    ex1.attach(test);
    ex2.begin();
    ex2.attach(timeout);
    
    #if TXMODE
        lora.enttry_tx();
    #else
        lora.enttry_rx();
    #endif
}
uint32_t last;
uint8_t temp;
int main(void)
{
    int temp;
    setup();
    while(1)
    {
        #if TXMODE
        if(lora.state == TXREADY)
        {
            uart1.printf("tx time:%dms\n",(uint32_t)(millis() - last));
            last = millis();
            lora.write(buf,8);
            //temp++;
            //temp%=10;
            //buf[0] = 0x30+temp;
            delay_ms(1000);
            ebox_reset();
        }
        if(millis() - last > 5000)
        {            
            last = millis();
            temp = lora.readRegister(SX1278_REG_IRQ_FLAGS);
            uart1.printf("temp:0x%x\n",temp);
            lora.clearIRQFlags();
            lora.state = TXREADY;
        }
        //delay_ms(500);
        #else
        if(millis() - last > 5000)
        {            
            last = millis();
            lora.enttry_rx();
        }
				

        #endif
    }

}
