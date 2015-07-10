
#include "ebox.h"

USART uart3(USART3,&PB10,&PB11);

#include "lcd1602.h"
GPIO* led = &PB12;
GPIO* en = &PB13;
GPIO* rw = &PB14;
GPIO* rs = &PB15;

GPIO* d0 = &PB0;
GPIO* d1 = &PB1;
GPIO* d2 = &PB2;
GPIO* d3 = &PB3;
GPIO* d4 = &PB4;
GPIO* d5 = &PB5;
GPIO* d6 = &PB6;
GPIO* d7 = &PB7;
LCD1602 lcd(led,en,rw,rs,d0,d1,d2,d3,d4,d5,d6,d7);

//#define rs PB15
//#define rw PB14
//#define en PB13
//#define led PB12

//#define d0 PB0
//#define d1 PB1
//#define d2 PB2
//#define d3 PB3
//#define d4 PB4
//#define d5 PB5
//#define d6 PB6
//#define d7 PB7

//LCD1602 lcd(&led,&en,&rw,&rs,&d0,&d1,&d2,&d3,&d4,&d5,&d6,&d7);




void setup()
{
	eBoxInit();
	uart3.begin(9600);
	lcd.begin();

}

int16_t tmp[7];
int16_t x,y,z;
uint8_t id;
int main(void)
{
	setup();
	lcd.test();
	lcd.setPos(2,0);
	lcd.printf("test = %05d",x);
	while(1)
	{
		delay_ms(10);
		uart3.printf("\r\nid = %d",id);
		uart3.printf("\r\naccx = %d",tmp[0]);
		uart3.printf("\r\naccy = %d",tmp[1]);
		uart3.printf("\r\naccz = %d",tmp[2]);
		uart3.printf("\r\ntemp = %d",tmp[3]);
		uart3.printf("\r\ngyrox = %d",tmp[4]);
		uart3.printf("\r\ngyroy = %d",tmp[5]);
		uart3.printf("\r\ngyroz = %d",tmp[6]);
		uart3.printf("\r\n==========");
		uart3.printf("\r\nX = %d",x);
		uart3.printf("\r\nY = %d",y);
		uart3.printf("\r\nZ = %d",z);
		uart3.printf("\r\n==========");
		delay_ms(1000);
	}


}




