
#include "ebox.h"
#include "uartx.h"
#include "interrupts.h"

#include "rtc.h"

uint8_t h,m,s;
uint32_t counter;
void rtcit()
{
			rtc.getTimeHMS(&h,&m,&s);
			counter = rtc.getCounter();

	uart3.printf("timeNow = %02d:%02d:%02d !",h,m,s);
	uart3.printf("  counter = %d \r\n",counter);
}
void setup()
{
	eBoxInit();
	uart3.begin(115200);
	
	rtc.begin();
	rtc.interrupt(ENABLE);
	rtc.attachInterrupt(rtcit);
	rtc.setCounter(50);
	rtc.setTimeHMS(1,59,50);
	pinMode(7,OUTPUT);
}


int main(void)
{
	setup();
	while(1)
	{

	}


}




