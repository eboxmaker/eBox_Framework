
#include "ebox.h"
#include "w25x16.h"

USART uart3(USART3,PB10,PB11);


W25X flash(PE5,SPI1,PA5,PA6,PA7);


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
		uart3.printf("\r\n==readid=======\r\n");
		uart3.printf("id = %x",id);

		uart3.printf("\r\n==write&read========\r\n");
		flash.write(wbuf,0,10);
		flash.read(buf,0,10);	
		for(int i=0;i<10;i++)
		uart3.printf(" %x",buf[i]);
		
		uart3.printf("\r\n==erase&read========\r\n");
		flash.eraseSector(0);
		flash.read(buf,1,10);	
		for(int i=0;i<10;i++)
		uart3.printf(" %x",buf[i]);
		uart3.printf("\r\n=========================\r\n");
		uart3.printf("\r\n\r\n");
		
		delay_ms(1000);
	}
}




