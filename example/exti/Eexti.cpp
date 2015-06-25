
#include "ebox.h"
#include "uartx.h"
#include "exti.h"
#include "interrupts.h"

//如果中断引脚没有外部上拉或者下拉。
//请设置exti.cpp中的初始化函数将
//引脚设置内部上拉或者下拉
uint32_t xx;
uint8_t flag;
EXTIx ex(7,EXTI_Trigger_Falling);

void exit()
{
	xx++;
	flag = 1;
}
void setup()
{
	eBoxInit();
	uart3.begin(115200);
	ex.attachInterrupt(exit);
}


int main(void)
{
	setup();
	while(1)
	{
		if(flag == 1)
		{
			uart3.printf("\r\nxx = %d",xx);
			flag = 0;
		}
	}


}




