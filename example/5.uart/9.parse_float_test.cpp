#include "ebox.h"
#include "bsp_ebox.h"
#include "ebox_mem.h"

/**
	*	1	��������ʾ��UartStream������
	*	2	UartStream�Ǵ��ڼ�����stream���ࡣ֧����stream�����й���
	*		������ȡһ��String������һ���ؼ��֣��ؼ��ʵ�
	*   	parseFloat���ڹ涨�ĳ�ʱʱ���ڶ�ȡ��ȡ�����������������е�
    *       ��ʾfloat���͵��ַ������û����򴮿ڷ���123.4567.89.26���в���
	*/


/* ���������������̷������� */
#define EXAMPLE_NAME	"UartStream example"
#define EXAMPLE_DATE	"2018-08-13"

char buffer[100];
float value;
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
        if(UART.available())
        {
            value = UART.parseFloat();
            UART.println(value);
        }


    }
}