
#include "ebox.h"
#include "dataflash.h"


USART uart1(USART1,PA9,PA10);

FLASHCLASS flash;

void setup()
{
	eBoxInit();
	uart1.begin(9600);

   
}

static u8 rbuf[10];
static u8 wbuf[10];
int i,j;

int main(void)
{
	setup();
	PB8->mode(OUTPUT_PP);
	for(i=0;i<10;i++)
		wbuf[i]=i;
	
	while(1)
	{
		
		
		wbuf[0]=(i++)%10;
		flash.Flash_Write(0x08004000,wbuf,10);
		flash.Flash_Read(0x08004000,rbuf,10);

		PB8->write(!PB8->read());
		uart1.printf("running£¡\r\n");
	for(int i=0;i<10;i++)
		uart1.printf("%d",rbuf[i]);
		uart1.printf("\r\n");
		delay_ms(500);
	}


}




