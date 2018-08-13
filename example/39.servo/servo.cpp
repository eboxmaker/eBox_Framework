#include "ebox.h"
#include "bsp_ebox.h"

#include "servo.h"
/**
	*	1	��������ʾ��Servo����
	*	2	Servo�Ƕ���Ŀ����࣬����-100%�������棩��100%�������棩��ʽ�趨
	*/
	

/* ���������������̷������� */
#define EXAMPLE_NAME	"Servo example"
#define EXAMPLE_DATE	"2018-08-13"

Servo servo(&PB8);


void setup()
{
	ebox_init();
    uart1.begin(115200);
    print_log(EXAMPLE_NAME,EXAMPLE_DATE);
    
	servo.begin();

}
int main(void)
{
	setup();
	servo.setPct(0);
	float pct = 0, increase = 1;
	while (1)
	{
		pct += increase;
		if (pct >= 100 || pct <= -100)
		{
			increase = -increase;
		}
		servo.setPct(pct);
		uart1.printf("%f\n", servo.getPct());
		delay_ms(20);
	}
}