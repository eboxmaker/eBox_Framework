
#include "ebox.h"

USART uart3(USART3,PB10,PB11);

void setup()
{
	eBoxInit();
	uart3.begin(9600);
	
	PA7->mode(AIN);
}

int16_t x;
int main(void)
{
	setup();
	while(1)
	{
		
		x = analogRead(PA7);
		uart3.printf("hex = %05d\r\n",x);
		x = analogReadToVoltage(PA7);
		uart3.printf("val = %04dmv\r\n",x);
		uart3.printf("==============\r\n",x);
		delay_ms(1000);
	}


}




