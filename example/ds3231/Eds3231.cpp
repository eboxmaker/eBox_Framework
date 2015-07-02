
#include "ebox.h"
#include "boardcfg.h"
#include "uartx.h"
#include "interrupts.h"

#include "ds3231.h"

DS3231 ds(DS3231_SDA_PIN,DS3231_SCL_PIN);

DateTime t;
char time[9];
char date[9];

void setup()
{
	eBoxInit();
	uart3.begin(115200);
	ds.begin();
	ds.setSpeed(400000);
	
	t.year = 15;
	t.month = 7;
	t.date = 3;
	t.hour = 23;
	t.min = 59;
	t.sec = 55;
}
int main(void)
{
	setup();
	ds.setTime(&t);
	while(1)
	{	
		ds.getDateTime(&t);
		ds.getTime(time);
		ds.getDate(date);
		uart3.printf("=======\r\n");
		uart3.printf("%02d-%02d-%02d %02d:%02d:%02d\r\n",t.year,t.month,t.date,t.hour,t.min,t.sec);

		uart3.printf(date);
		uart3.printf(" ");
		uart3.printf(time);
		uart3.printf("\r\n");
		delay_ms(1000);
	}


}




