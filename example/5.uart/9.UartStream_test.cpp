#include "ebox.h"
#include "bsp_ebox.h"


/**
	*	1	��������ʾ��UartStream����������ebox_uart_stream��coreĿ¼��stream
	*	2	UartStream�Ǵ��ڼ�����stream���ࡣ֧����stream�����й���
	*		������ȡһ��String������һ���ؼ��֣��ؼ��ʵ�
	*   	����������ж��������ԵĶ�ȡ��
    *       �����ʼ��ȡ���ͻ�ȵ���ȡ�����󻹻���ʱ�趨�ĳ�ʱʱ�䡣�û���ע��ʹ��
	*/
	

/* ���������������̷������� */
#define EXAMPLE_NAME	"UartStream example"
#define EXAMPLE_DATE	"2018-08-13"

UartStream uart1s(&UART);


void setup()
{
	ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME,EXAMPLE_DATE);
    

    uart1s.begin(115200);
    uart1s.setTimeout(1000);//���峬ʱʱ��
}
int main(void)
{
	setup();
	while (1)
	{
        String x = uart1s.readString();//
        
        if(x != NULL)
        {
            uart1s.print(x);
            uart1s.print('\t');
            uart1s.print(ebox_get_free()/1024.0);
            uart1s.println("KB");
        }
	}
}