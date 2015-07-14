
#include "ebox.h"

USART uart3(USART3,PB10,PB11);

void rtcsecit()
{
	uart3.printf("%02d:%02d:%02d:\r\n",rtc.hour,rtc.min,rtc.sec);
}


void setup()
{
	eBoxInit();
	uart3.begin(9600);
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




