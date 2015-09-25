
#include "ebox.h"
#include "math.h"
USART uart1(USART1,PA9,PA10);

PWM pwm1(PB8);
//STM32 RUN IN eBox
void setup()
{
	eBoxInit();
	uart1.begin(9600);
	pwm1.setDuty(500);
}

float x;
uint16_t y;

int main(void)
{
	setup();
	
	while(1)
	{		 	

		pwm1.setFrq(1500);
		delay_ms(10);
	}

}




