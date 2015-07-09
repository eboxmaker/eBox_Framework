
#include "ebox.h"

USART uart3(USART3,&PB10,&PB11);

PWM pwm1(&PA0);

void setup()
{
	eBoxInit();

	pwm1.setDuty(500);

}

int main(void)
{
	setup();
	
	while(1)
	{
		uart3.printf("\r\nruning !");
		delay_ms(1000);
	}


}




