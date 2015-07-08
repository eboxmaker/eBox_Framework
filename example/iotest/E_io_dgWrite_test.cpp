
#include "ebox.h"
#include "io.h"

GPIO *PF6 = new GPIO(GPIOF,GPIO_Pin_6);
GPIO *PF7 = new GPIO(GPIOF,GPIO_Pin_7);
GPIO *PF8 = new GPIO(GPIOF,GPIO_Pin_8);

void setup()
{
	eBoxInit();
	pinMode(PF6,_OPP);
	pinMode(PF8,_OPP);
}

//测试多层指针指向后对性能的影响
//结果：不影响速度
//原因：指针在初始化的时候已经被指定为一个地址而已
GPIO * pp;
void test(GPIO* p)
{
	pp = p;
}

int main(void)
{
	setup();
	test(PF6);
	while(1)
	{
		//寄存器 5.15Mhz 
		//汇编指令数：1
//		GPIOF->BRR = GPIO_Pin_6;
//		GPIOF->BSRR = GPIO_Pin_6;
		//固件库的速度  ：1.84Mhz；
		//汇编指令数：3
//		GPIO_SetBits(GPIOF,GPIO_Pin_6);
//		GPIO_ResetBits(GPIOF,GPIO_Pin_6);
//一下三种方式性能是一样的。生成的汇编代码一样，只比固件库慢一点点，原因是内部有一个if判断
		//指针的速度：1.29Mhz；
		//汇编指令数：4
//		PF6->write(0);
//		PF6->write(1);
		//宏的速度：1.29Mhz；
		//汇编指令数：4
			dgWrite(pp,1);
			dgWrite(pp,0);
		//多层指针的速度:1.26Mhz
		//汇编指令数：4
//		pp->write(0);
//		pp->write(1);
		//普通函数的速度 :562khz
		//汇编指令数：
//		digitalWrite(0x56,1);
//		digitalWrite(0x56,0);

	}
}




