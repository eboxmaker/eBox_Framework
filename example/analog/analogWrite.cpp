#include "boardcfg.h"
#include "math.h"

#include "ebox.h"

#include "pwm.h"

#include "lcd1602.h"
#include "uartx.h"
//PWM p(7);
/*
analog pin table
0,1,2,3,6,7,16,17,22,23,24,25,26,27
*/
int rcvok;

char rcv[100];
int i;
void test1()
{


;
	//digitalWrite(7,!digitalRead(7));
}
void test2()
{


;
	//digitalWrite(7,!digitalRead(7));
}
void test3()
{
	rcv[i++] = USART_ReceiveData(USART3);
	if(rcv[i-1] == '!')
	{
			rcvok =1;
			i = 0;	
	}
	//digitalWrite(7,!digitalRead(7));
}
	
void setup()
{
	eBoxInit();
	uart1.begin(115200);
	uart2.begin(115200);
	uart3.begin(115200);

	
	//uart3.interrupt(ENABLE);
//	uart1.attachInterrupt(test1,1);
//	uart1.attachInterrupt(test2,1);
//	uart3.attachInterrupt(test3,1);
	pinMode(7,OUTPUT);
}

float x,y;
int main(void)
{

	setup();
	//uart3.printf("shentqlfsdfafadfadf");

	while(1)
	{		 	
		delay_ms(1000);
		uart1.printf("uart1 ok\r\n");
		uart2.printf("uart2 ok\r\n");
		uart3.printf("uart3 ok\r\n");
		if(rcvok == 1)
		{
		
			uart3.putstr(rcv);
			rcvok = 0;
		}		
//		x = x + PI*0.01;
//		if(x >= PI)x=0;
//		y = 2000 - (sin(x)+1)*1000;
		
	//	analogWrite(7,y);
	//	delay_ms(50);
	}


}




