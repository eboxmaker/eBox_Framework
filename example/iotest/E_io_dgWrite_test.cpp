
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

//���Զ��ָ��ָ�������ܵ�Ӱ��
//�������Ӱ���ٶ�
//ԭ��ָ���ڳ�ʼ����ʱ���Ѿ���ָ��Ϊһ����ַ����
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
		//�Ĵ��� 5.15Mhz 
		//���ָ������1
//		GPIOF->BRR = GPIO_Pin_6;
//		GPIOF->BSRR = GPIO_Pin_6;
		//�̼�����ٶ�  ��1.84Mhz��
		//���ָ������3
//		GPIO_SetBits(GPIOF,GPIO_Pin_6);
//		GPIO_ResetBits(GPIOF,GPIO_Pin_6);
//һ�����ַ�ʽ������һ���ġ����ɵĻ�����һ����ֻ�ȹ̼�����һ��㣬ԭ�����ڲ���һ��if�ж�
		//ָ����ٶȣ�1.29Mhz��
		//���ָ������4
//		PF6->write(0);
//		PF6->write(1);
		//����ٶȣ�1.29Mhz��
		//���ָ������4
			dgWrite(pp,1);
			dgWrite(pp,0);
		//���ָ����ٶ�:1.26Mhz
		//���ָ������4
//		pp->write(0);
//		pp->write(1);
		//��ͨ�������ٶ� :562khz
		//���ָ������
//		digitalWrite(0x56,1);
//		digitalWrite(0x56,0);

	}
}




