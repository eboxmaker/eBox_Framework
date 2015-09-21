
#include "ebox.h"


USART uart1(USART1,PA9,PA10);


uint32_t xx;
uint8_t flag;
TIM timer2(TIM2);

void t2it()
{
	xx++;
	if(xx == 1000)
	{
		flag = 1;
		xx = 0;
	}
}
void setup()
{
	eBoxInit();
	uart1.begin(9600);
	
	timer2.begin(1000,72);
	timer2.interrupt(ENABLE);
	timer2.attachInterrupt(t2it);
	timer2.start();
}


int main(void)
{
	setup();
	while(1)
	{
		if(flag == 1)
		{
			uart1.printf("\r\ntimer2 is triggered 1000 times !",xx);
			flag = 0;
		}
	}


}








