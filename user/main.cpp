
#include "ebox.h"
#include "button.h"

USART uart1(USART1,PA9,PA10);

Button btn(PA8,1);

void setup()
{
	eBoxInit();
	uart1.begin(9600);
   btn.begin();
}



int main(void)
{
	setup();
	PB8->mode(OUTPUT_PP);
	PB9->mode(OUTPUT_PP);
	PB10->mode(OUTPUT_PP);

	
	while(1)
	{
		btn.loop();
		if(btn.click())
		{
			PB8->write(!PB8->read());
			uart1.printf("\r\nclick event!");
		}
		if(btn.release())
		{
			PB9->write(!PB9->read());
			uart1.printf("\r\nrelease event!");
		}
		if(btn.pressedFor(2000,2))
		{
			PB10->write(!PB10->read());
			uart1.printf("\r\nlong press event!");
		}
	}

}




