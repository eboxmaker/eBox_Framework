
#include "ebox.h"

USART uart1(USART1,PA9,PA10);

uint32_t xx;

EXTIx ex(PA8,EXTI_Trigger_Falling);

void exit()
{
	xx++;
	uart1.printf("\r\nxx = %d",xx);
}
void setup()
{
	eBoxInit();
	uart1.begin(9600);
	ex.attachInterrupt(exit);
}


int main(void)
{
	setup();
	while(1)
	{
		;
	}


}




