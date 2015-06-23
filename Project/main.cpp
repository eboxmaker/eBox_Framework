
#include "ebox.h"

#include "button.h"
#include "lcd1602.h"
#include "led.h"
//#include "uart1.h"
//#include "uart2.h"
//#include "uart3.h"
#include "ds3231.h"
#include "encoder.h"

#include "gtimer.h"
#include "pwm.h"

#include "exti.h"
#include "spi.h"
#include "att7022.h"

#include "interrupts.h"
	u32 ms,us;
uint32_t xx;
EXTIx ex(11,EXTI_Trigger_Falling);

DateTime t;
void pit()
{
xx++;
}
void setup()
{
	eBoxInit();
	t.year = 2015;
	t.month = 6;
	t.date = 14;
	t.hour = 23;
	t.min = 39;
	t.sec = 55;
	Init_ADC1();
	//ATT.begin();
	ex.attachInterrupt(pit);
}


int main(void)
{

	setup();
	while(1)
	{


		


	
	}


}




