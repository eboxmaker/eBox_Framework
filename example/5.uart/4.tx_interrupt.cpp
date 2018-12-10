#include "ebox.h"
#include "bsp_ebox.h"
#include "ebox_mem.h"


/**
    *	1	��������ʾ�˴��ڷ����жϲ���
*/

/* ���������������̷������� */
#define EXAMPLE_NAME	"Uart Tx interrupt example"
#define EXAMPLE_DATE	"2018-08-13"


size_t len;


void tx()
{

    PB8.toggle();
}
void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);
    UART.attach(tx, TxIrq);

    PB8.mode(OUTPUT);


}
int main(void)
{
    setup();

    while (1)
    {

        UART.write('a');
        delay_ms(1000);
    }
}