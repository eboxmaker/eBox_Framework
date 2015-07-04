
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
uint8_t buf[10];
uint8_t wbuf[10];
int main(void)
{
	setup();
	for(int i=0;i<10;i++)

	 wbuf[i] = i;

	while(1)
	{
		flash.readId(&id);
		uart3.printf("\r\nid = %x\r\n",id);
		flash.read(buf,1,10);
		for(int i=0;i<10;i++)
		uart3.printf("\r\n%x",buf[i]);
		uart3.printf("\r\n==========");

		flash.write(wbuf,0,10);
		flash.read(buf,0,10);	
		for(int i=0;i<10;i++)
		uart3.printf("\r\n%x",buf[i]);
		uart3.printf("\r\n==========");
		
		flash.eraseSector(0);
		flash.read(buf,1,10);	
		for(int i=0;i<10;i++)
		uart3.printf("\r\n%x",buf[i]);
		
		uart3.printf("\r\n==========");
		delay_ms(1000);
	}
}




