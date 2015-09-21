
#include "ebox.h"

USART uart1(USART1,PA9,PA10);

PWM pwm1(PA7);

void setup()
{
	eBoxInit();
	uart1.begin(9600);
	pwm1.setDuty(500);
}

int main(void)
{
	setup();
	
	while(1)
	{
		uart1.printf("\r\nruning !");
		delay_ms(1000);
	}
}




