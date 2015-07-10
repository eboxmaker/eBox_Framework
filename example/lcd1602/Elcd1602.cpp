
#include "ebox.h"

USART uart3(USART3,&PB10,&PB11);

void setup()
{
	eBoxInit();
	uart3.begin(9600);

	PA7.mode(_AIN);
}

int16_t x;
int main(void)
{
	setup();
	while(1)
	{
		x = analogRead(&PA7);
		uart3.printf(2,0,"test = %05d",x);
		delay_ms(1000);
	}


}




