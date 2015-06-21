
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

//TIM timer4(TIM4,100,72);
//TIM timer2(TIM2,100,72);
DateTime t;
void setup()
{
	t.year = 2015;
	t.month = 6;
	t.date = 14;
	t.hour = 23;
	t.min = 39;
	t.sec = 55;
	systemtickcfg(9000);//1ms产生一次中断
	Init_ADC1();
	//ATT.begin();
}

void t2it(void)
{

	;

}
void t4it(void)
{
	digitalWrite(7,1);
	digitalWrite(7,0);
	digitalWrite(7,1);
	digitalWrite(7,0);

}
int i=10;
int main(void)
{
//	NVIC_SETPRIMASK();
	pinMode(7,OUTPUT);

	setup();
//	USART1Init(115200);
//	USART3Init(115200);
//	uint32_t ms;
	
// timer2.attachInterrupt(t2it);
// timer4.attachInterrupt(t4it);
	while(1)
	{

		delay_ms(1);
		digitalWrite(7,0);

		delay_ms(1);
//	for(int i = 0 ;i < 10; i++)
		digitalWrite(7,1);

//	for(int i = 0 ;i < 10; i++)



	
	}


}




