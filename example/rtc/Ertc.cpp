
#include "ebox.h"

USART uart1(USART1,PA9,PA10);

void rtcsecit()
{
	uart1.printf("%02d:%02d:%02d:\r\n",rtc.hour,rtc.min,rtc.sec);
}


void setup()
{
	eBoxInit();
	uart1.begin(9600);
	rtc.begin();
	rtc.attachInterrupt(RTC_EVENT_SEC,rtcsecit);
	rtc.interrupt(RTC_EVENT_SEC,ENABLE);
}


int main(void)
{
	setup();

	while(1)
	{
		
		delay_ms(1000);
	}
}




