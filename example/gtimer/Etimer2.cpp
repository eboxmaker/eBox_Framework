
#include "ebox.h"
#include "uartx.h"
#include "gtimer.h"
#include "interrupts.h"

//如果中断引脚没有外部上拉或者下拉。
//请设置exti.cpp中的初始化函数将
//引脚设置内部上拉或者下拉
uint32_t xx;
uint8_t flag;

TIM timer2(TIM2);

void t2it()
{
	xx++;
	if(xx == 1000)
	{
		flag = 1;
		xx = 0;
	}
}
void setup()
{
	eBoxInit();
	uart3.begin(115200);
	
	timer2.begin();
	timer2.Interrupt(ENABLE);
	timer2.attachInterrupt(t2it);
	timer2.start();
	
}


int main(void)
{
	setup();
	while(1)
	{
		if(flag == 1)
		{
			uart3.printf("\r\ntimer2 is triggered 1000 times !",xx);
			flag = 0;
		}
	}


}




