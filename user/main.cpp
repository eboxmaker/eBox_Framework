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


PARALLEL_GPIO p;

void setup()
{
	ebox_init();
	uart1.begin(9600);

    p.bit[0] = &PA1;
    p.bit[1] = &PA3;
    p.bit[2] = &PA5;
    p.bit[3] = &PA7;
    p.bit[4] = &PA8;
    p.bit[5] = &PB1;
    p.bit[6] = &PB6;
    p.bit[7] = &PB2;
}

int main(void)
{

	setup();

	while(1)
	{		 	
        p.write(0xaa);
        delay_ms(1000);
	}


}




