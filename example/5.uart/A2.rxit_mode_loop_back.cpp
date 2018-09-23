#include "ebox.h"
#include "bsp_ebox.h"


/**
	*	1	��������ʾ��UartStream����
	*	2	UartStream�Ǵ��ڼ�����stream���ࡣ֧����stream�����й���
	*		������ȡһ��String������һ���ؼ��֣��ؼ��ʵ�
	*   	����������ж��������ԵĶ�ȡ��
    *       �����ʼ��ȡ���ͻ�ȵ���ȡ�����󻹻���ʱ�趨�ĳ�ʱʱ�䡣�û���ע��ʹ��
	*/
	

/* ���������������̷������� */
#define EXAMPLE_NAME	"Uart1,2,3 RxIt mode loop back example"
#define EXAMPLE_DATE	"2018-9-23"


size_t len;

void rx()
{
     LED1.toggle();
}
void rx2()
{
     LED2.toggle();
}

void setup()
{
	ebox_init();
    
    LED1.mode(OUTPUT_PP);
    LED2.mode(OUTPUT_PP);
    
    uart1.begin(115200,RxIt);
    uart1.attach(rx,RxIrq);

    uart2.begin(115200,RxIt);
    uart2.attach(rx2,RxIrq);

    uart3.begin(115200,RxIt);
    uart3.attach(rx2,RxIrq);

    print_log(EXAMPLE_NAME,EXAMPLE_DATE);


}
int main(void)
{
	setup();

	while (1)
	{

        len = uart1.available();
        for(int i = 0; i < len; i++ )
        {
            uart1.write(uart1.read());
        }
        len = uart2.available();
        for(int i = 0; i < len; i++ )
        {
            uart2.write(uart2.read());
        }      
        len = uart3.available();
        for(int i = 0; i < len; i++ )
        {
            uart3.write(uart3.read());
        }


	}
}