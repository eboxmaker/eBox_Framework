
#include "ebox.h"

USART uart3(USART3,&PB10,&PB11);



int rcvok;

char rcv[100];
int i;
void test()
{
	rcv[i++] = USART_ReceiveData(USART3);
	if(rcv[i-1] == '!')
	{
		rcvok =1;
		i = 0;	
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
	uart3.printf("shentqlf\r\n");

	while(1)
	{		 	
		if(rcvok == 1)
		{
		
			uart3.printf(rcv);
			rcvok = 0;
			for(int i = 0; i < 100; i ++)
				rcv[i] = 0;
		}

	}


}




