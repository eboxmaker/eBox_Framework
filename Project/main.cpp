
#include "ebox.h"
#include "uartx.h"
#include "interrupts.h"

#include "w25x16.h"

W25X flash(0x45);

void setup()
{
	eBoxInit();
	uart3.begin(9600);
	
	flash.begin();

}

int16_t tmp[7];
uint16_t id;
int main(void)
{
	setup();
	while(1)
	{
		flash.readId(&id);

		uart3.printf("\r\nid = %x",id);
		uart3.printf("\r\n==========");
		delay_ms(1000);
	}
}




