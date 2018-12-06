#include "ebox.h"
#include "bsp_ebox.h"


/**
	*	1	��������ʾ��uart���շ�����
*/
	

/* ���������������̷������� */
#define EXAMPLE_NAME	"UartStream example"
#define EXAMPLE_DATE	"2018-08-13"

int len;

void setup()
{
	ebox_init();
    uart1.begin(115200);
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
            char c = uart1.read();
            uart1.write(c);
        }

	}
}



