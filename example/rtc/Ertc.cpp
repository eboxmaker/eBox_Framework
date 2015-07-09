
#include "ebox.h"

USART uart3(USART3,&PB10,&PB11);



uint32_t x;
uint32_t xx;
uint8_t flag1;
uint8_t flag;

TIM timer2(TIM2);
TIMERONE t1;

void t2it()
{
	xx++;
	if(xx == 1000)
	{
		flag = 1;
		xx = 0;
	}
}
void t1it()
{
	x++;
	digitalWrite(7,!digitalRead(7));
	if(x == 1000)
	{
		flag1 = 1;
		x = 0;
	}
}
void setup()
{
	eBoxInit();
	uart3.begin(115200);
	
	timer2.begin();
	timer2.Interrupt(ENABLE);
	timer2.attachInterrupt(t2it);
	timer2.start();
	
	t1.begin();
	t1.interrupt(ENABLE);
	t1.attachInterrupt(t1it);
	t1.start();
	pinMode(7,OUTPUT);
}


int main(void)
{
	setup();
	while(1)
	{
		if(flag == 1)
		{
			uart3.printf("\r\ntimer2 is triggered 1000 times !",xx);
			flag = 0;
		}
		if(flag1 == 1)
		{
			uart3.printf("\r\ntimer1 is triggered 1000 times !",xx);
			flag1 = 0;
		}
	}


}




