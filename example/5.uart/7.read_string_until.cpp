#include "ebox.h"
#include "bsp_ebox.h"
#include "ebox_mem.h"

/**
	*	1	��������ʾ��UartStream������
	*	2	UartStream�Ǵ��ڼ�����stream���ࡣ֧����stream�����й���
	*		������ȡһ��String������һ���ؼ��֣��ؼ��ʵ�
	*   	readStringUntil������������ֱ����������Ĺؼ��ֲ�ֹͣ��ȡ��
    *       �����ʼ��ȡ�������˳�ʱʱ�仹û���������򷵻ض��������ݡ�
    *       ����û������������ݼ���������趨ʱ�䣬��Ὣ��η���
    *       ������ƴ�ӵ�һ��
	*/


/* ���������������̷������� */
#define EXAMPLE_NAME	"UartStream example"
#define EXAMPLE_DATE	"2018-08-13"


void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);


    UART.setTimeout(1000);//���峬ʱʱ��
}
int main(void)
{
    setup();
    while (1)
    {
        String x = UART.readStringUntil(';');//

        if(x != NULL)
        {
            UART.print(x);
            UART.print('\t');
            UART.print(ebox_get_free() / 1024.0);
            UART.println("KB");
        }
    }
}