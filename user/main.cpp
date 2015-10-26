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



#define  HEAD '$' 
#define  END '!' 

#define  NEEDHEAD 0 
#define  NEEDDATA 1 
#define  DATAEND 2 

uint8_t state = NEEDHEAD;

char rcv[100];
int i;
void test()
{
	uint8_t c;
	c = uart1.receive();
	switch(state)
	{
		case NEEDHEAD:
			if(c == HEAD)
			{
				i = 0;
				rcv[i++] = c;
				state = NEEDDATA;
			}
			break;
		case NEEDDATA:
			if(c == END)
			{
				rcv[i] = c;
				state = DATAEND;			
			}
			else
			{
				rcv[i++] = c;
			}		
			break;
	}


}
	
void setup()
{
	ebox_init();
	uart1.begin(9600);
	uart1.interrupt(ENABLE);
	uart1.attach_interrupt(test);
}

float x,y;
int main(void)
{

	setup();
	uart1.printf("uart is ok !\r\n");

	while(1)
	{		 	
		if(state == DATAEND)
		{
		
			uart1.printf(rcv);
			for(int i = 0; i < 100; i ++)
				rcv[i] = 0;
			state = NEEDHEAD;
		}

	}


}




