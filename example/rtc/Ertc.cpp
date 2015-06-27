
#include "ebox.h"
#include "uartx.h"
#include "interrupts.h"

#include "rtc.h"


void rtcit()
{
			uart3.printf("\r\ntimeNow = %d !",rtc.readCounter());
}
void setup()
{
	eBoxInit();
	uart3.begin(115200);
	
	rtc.begin();
	rtc.interrupt(ENABLE);
	rtc.attachInterrupt(rtcit);
	rtc.setCounter(100);

	pinMode(7,OUTPUT);
}


int main(void)
{
	setup();
	while(1)
	{

	}


}




