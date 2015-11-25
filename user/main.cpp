/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/


#include "string.h"
#include "ebox.h"

#include "nrf24l01.h"

NRF24L01 nrf(&PA2,&PA1,&PA0,&spi1);


	
uint8_t buf[32] = "adfafdadfaf";
int status;
void setup()
{
	ebox_init();
	uart1.begin(9600);
    uart1.printf("uart is ok !\r\n");
    nrf.begin(1);
    nrf.set_tx_mode();

}
uint32_t tmp_time;
int main(void)
{
	setup();
	while(1)
	{	
        status = nrf.self_check();
        uart1.printf("status : 0x%x",status);
        delay_ms(1000);
        
	}
}
