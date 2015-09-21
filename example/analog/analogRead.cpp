
#include "ebox.h"

USART uart1(USART1,PA9,PA10);

void setup()
{
	eBoxInit();
	uart1.begin(9600);
	PA7->mode(AIN);
}

int16_t x;
int main(void)
{
	setup();
	while(1)
	{
		x = analogRead(PA7);
		uart1.printf("hex = %05d\r\n",x);
		x = analogReadToVoltage(PA7);
		uart1.printf("val = %04dmv\r\n",x);
		uart1.printf("==============\r\n",x);
		delay_ms(1000);
	}


}




