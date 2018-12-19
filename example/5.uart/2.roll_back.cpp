#include "ebox.h"
#include "bsp_ebox.h"


/**
	*	1	此例程演示了uart的收发操作
*/


/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"UartStream example"
#define EXAMPLE_DATE	"2018-08-13"

int len;

void setup()
{
    ebox_init();
    UART.begin(115200,RxIt);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);
}
int main(void)
{
    uint32_t j = 0;
    setup();

    while (1)
    {
        len = UART.available();
        for(int i = 0; i < len; i++ )
        {
            char c = UART.read();

            UART.printf("%c \r\n", c);

        }
    }
}



