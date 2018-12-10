#include "ebox.h"
#include "bsp_ebox.h"
#include "ebox_mem.h"

/**
	*	1	��������ʾ��UartStream������
	*	2	UartStream�Ǵ��ڼ�����stream���ࡣ֧����stream�����й���
	*		������ȡһ��String������һ���ؼ��֣��ؼ��ʵ�
	*   	readBytes���ڹ涨�ĳ�ʱʱ���ڶ�ȡN���ֽڣ���������ﵽ����
    *       �򷵻أ����������������ʱ���ء�
	*/


/* ���������������̷������� */
#define EXAMPLE_NAME	"UartStream example"
#define EXAMPLE_DATE	"2018-08-13"

char buffer[100];
size_t len;
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
        len = UART.readBytes(buffer, 10);
        if(len > 0)
        {
            UART.write(buffer, len);
        }

    }
}