
#include "ebox.h"
#include "w25x16.h"

USART uart3(USART3,&PB10,&PB11);

void setup()
{
	eBoxInit();
	PA0.mode(_OPP);
	
}
int main(void)
{
	setup();
	while(1)
	{
		PA0.write(1);
		delay_ms(10);
		PA0.write(0);
		delay_ms(10);	
	}
}




