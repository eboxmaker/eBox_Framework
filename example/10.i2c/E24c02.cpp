/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox
#include "ebox.h"
#include "at24c02.h"

EEPROM ee(&i2c1);
uint8_t data;
void setup()
{
	ebox_init();
	uart1.begin(9600);
	ee.begin(100000);
	

	PA7.mode(AIN);
}

int16_t x,i;
uint8_t buf[512];
uint8_t rbuf[512];
int main(void)
{
	setup();
	
	uart1.printf("===wbuf===\r\n");
		for(uint16_t i =0;i<512;i++)
		{
			buf[i] = i%256;
		}	
		for(uint16_t i =0;i<16;i++)
		{		
			for(uint16_t j =0;j<16;j++)
			{
				uart1.printf(" %02x ",buf[i*16+j]);	
				//ee.byteWrite(i*16+j,buf[i*16+j]);
			}	
			uart1.printf("\r\n ");	
		}
			ee.write_byte(0,buf,512);
		
		
		uart1.printf("\r\n===rbuf===\r\n");

			data = ee.read_byte(0,rbuf,512);
		for(uint16_t i =0;i<16;i++)
		{		
			for(uint16_t j =0;j<16;j++)
			{
				uart1.printf(" %02x ",rbuf[i*16+j]);	
			}	
			uart1.printf("\r\n ");	
		}
	uart1.printf("\r\n======\r\n");
	
//ee.byteWrite(0,0xaa);
//delay_ms(10);
//data = ee.byteRead(0);
//uart1.printf("\r\n data = %02x\r\n",data);
	while(1)
	{

		delay_ms(10000);
	}


}




