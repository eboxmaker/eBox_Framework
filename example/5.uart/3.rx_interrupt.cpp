#include "ebox.h"
#include "bsp_ebox.h"


/**
	*	1	此例程演示了串口中断操作
	*/


/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"UartStream example"
#define EXAMPLE_DATE	"2018-08-13"

size_t len;

void rx()
{
    len = UART.available();
    for(int i = 0; i < len; i++ )
    {
        UART.write(UART.read());
    }
    PB9.toggle();
}
void setup()
{
    ebox_init();
    UART.begin(115200, RxIt);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);
    UART.attach(rx, RxIrq);
    UART.interrupt(RxIrq, ENABLE);

    PB8.mode(OUTPUT_PP);
    PB9.mode(OUTPUT_PP);
}
int main(void)
{
    setup();

    while (1)
    {

    }
}