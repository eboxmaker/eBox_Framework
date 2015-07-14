
#include "ebox.h"

USART uart3(USART3,PB10,PB11);

uint32_t xx;

EXTIx ex(PA7,EXTI_Trigger_Falling);

void exit()
{
	xx++;
	uart3.printf("\r\nxx = %d",xx);
}
void setup()
{
	eBoxInit();
	uart3.begin(9600);
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




