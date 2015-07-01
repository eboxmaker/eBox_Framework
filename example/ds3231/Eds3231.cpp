
#include "ebox.h"
#include "boardcfg.h"
#include "uartx.h"
#include "interrupts.h"

#include "ds3231.h"

DS3231 ds(DS3231_SDA_PIN,DS3231_SCL_PIN);
DateTime t;
void setup()
{
	eBoxInit();
	uart3.begin(115200);
	ds.begin();
	ds.setSpeed(400000);
	t.year = 15;
	t.month = 7;
	t.date = 1;
	t.hour = 11;
	t.min = 59;
	t.sec = 55;
}

int main(void)
{
	setup();
	ds.setTime(&t);
	while(1)
	{	
		ds.getTime(&t);

		uart3.printf("\r\n%d-%d-%d %02d:%02d:%02d",t.year,t.month,t.date,t.hour,t.min,t.sec);

		delay_ms(1000);
	}


}




