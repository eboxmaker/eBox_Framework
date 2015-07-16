/*
һ���򵥵�����֡����ʾ��
*/
#include "ebox.h"

USART uart3(USART3,PB10,PB11);



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
	c = uart3.receiveData();
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
	eBoxInit();
	uart3.begin(9600);
	uart3.interrupt(ENABLE);
	uart3.attachInterrupt(test);
}

float x,y;
int main(void)
{

	setup();
	uart3.printf("uart is ok !\r\n");

	while(1)
	{		 	
		if(state == DATAEND)
		{
		
			uart3.printf(rcv);
			for(int i = 0; i < 100; i ++)
				rcv[i] = 0;
			state = NEEDHEAD;
		}

	}


}




