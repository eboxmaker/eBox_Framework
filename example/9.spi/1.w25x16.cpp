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
#include "w25x16.h"



W25x16 flash(&PA15, &spi1);


void setup()
{
    ebox_init();
    uart1.begin(115200);
    flash.begin(1);
}


int16_t tmp[7];
uint16_t id;
uint8_t rbuf[100];
uint8_t wbuf[100];
int main(void)
{
    setup();
    while(1)
    {
        for(int i = 0; i < 100; i++)
            wbuf[i] = random()%256;
        
        flash.read_id(&id);
        uart1.printf("\r\n==readid=======\r\n");
        uart1.printf("id = %x", id);

        uart1.printf("\r\n==write========\r\n");
        flash.write(wbuf, 0, 100);
        for(int i = 0; i < 100; i++)
            uart1.printf(" %x", wbuf[i]);

        uart1.printf("\r\n==read========\r\n");
        flash.read(rbuf, 0, 100);
        for(int i = 0; i < 100; i++)
            uart1.printf(" %x", rbuf[i]);

        uart1.printf("\r\n\r\n");

        delay_ms(1000);
    }
}




