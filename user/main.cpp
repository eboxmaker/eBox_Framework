
#include "ebox.h"

USART uart1(USART1,PA9,PA10);
USART uart3(USART3,PB10,PB11);

void rtcsecit()
{
//	uart3.printf("%02d:%02d:%02d:\r\n",rtc.hour,rtc.min,rtc.sec);
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
		uart1.putString("123\r\n",5);
		uart1.putString("123\r\n",5);
		uart1.printf("====\r\n");
		uart1.printf("====\r\n");
		uart1.printfln("----\r\n",6);
		uart1.printfln("----\r\n",6);
		
		delay_ms(1000);
	}
}




