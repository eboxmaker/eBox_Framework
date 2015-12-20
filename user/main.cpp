/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox
#include "ebox.h"
void setup()
{
	ebox_init();
    uart1.begin(9600);
    uart1.printf("\r\n");
	PB8.mode(OUTPUT_PP);
}
int main(void)
{
	setup();
	while(1)
	{
    uart1.printf("id:%x-%x-%x\r\n",sys.chip_id[0],sys.chip_id[1],sys.chip_id[2]);
    uart1.printf("flash size:%dKb\r\n",sys.flash_size);
        
		PB8 = !PB8;
        delay_ms(500);
		PB8 = 1;
        delay_ms(500);
		PB8 = 0;
        delay_ms(500);
		
	}

}


