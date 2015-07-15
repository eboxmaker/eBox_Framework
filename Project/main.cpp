
#include "ebox.h"



//USART uart1(USART1,PA9,PA10);
//USART uart2(USART3,PA2,PA3);
USART uart3(USART3,PB10,PB11);

void setup()
{
	eBoxInit();
	uart3.begin(9600);
	
}


int main(void)
{
	setup();
	while(1)
	{
		uart3.printf("hello World !\r\n");
		delay_ms(1000);
	}


}




