/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox


/*
一个简单的命令帧接收示例
*/
#include "ebox.h"



u8 count;

void test()
{
	uint8_t c;
	c = uart1.receive();
    uart1.put_char(c);
}
	
void test1()
{
    count++;
    PB8.toggle();
}
void setup()
{
	ebox_init();
	uart1.begin(9600);
	uart1.attach_rx_interrupt(test);
	uart1.attach_tx_interrupt(test1);
    PB8.mode(OUTPUT_PP);
    PB8.reset();
}

int main(void)
{

	setup();

	while(1)
	{		 	
        uart1.printf("uart is ok ! count = %d\r\n",count);
        delay_ms(1000);
	}


}




