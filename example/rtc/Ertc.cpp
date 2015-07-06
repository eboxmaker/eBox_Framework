
#include "ebox.h"
#include "uartx.h"

#include "rtc.h"

uint8_t h,m,s;
uint32_t counter;
void rtc_it_sec()
{
	rtc.getTimeHMS(&h,&m,&s);
	counter = rtc.getCounter();
	if(counter == 0x1517f)
		rtc.setCounter(0);
	uart3.printf("timeNow = %02d:%02d:%02d !",h,m,s);
	uart3.printf("  counter = %x \r\n",counter);
}
void rtc_it_ow()
{
//	uart3.printf("\r\n over flow\r\n");
}
void rtc_it_alr()
{
	
	/*
	
	*/

//	uart3.printf("\r\n alr occured\r\n");
}
void setup()
{
	eBoxInit();
	uart3.begin(115200);
	
	rtc.begin();
	rtc.interrupt(RTC_EVENT_OW | RTC_EVENT_ALR| RTC_EVENT_SEC,ENABLE);
	rtc.attachInterrupt(RTC_EVENT_OW,rtc_it_ow);
	rtc.attachInterrupt(RTC_EVENT_ALR,rtc_it_alr);
	rtc.attachInterrupt(RTC_EVENT_SEC,rtc_it_sec);
	rtc.setAlarm(23,59,55);
	rtc.setTimeHMS(23,59,50);
	
	pinMode(7,OUTPUT);
}


int main(void)
{
	setup();
	while(1)
	{

	}


}




