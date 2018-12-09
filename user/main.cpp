#include "ebox.h"
#include "bsp_ebox.h"
#include "ebox_mem.h"

/**
	*	1	此例程演示了uart的收发操作
*/
	

/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"UartStream example"
#define EXAMPLE_DATE	"2018-08-13"
#include "stdio.h"
int len;
void setup()
{
	ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME,EXAMPLE_DATE);

}
int main(void)
{
	setup();
    uint32_t j = 0;
	while (1)
	{
        len = UART.available();
        for(int i = 0; i < len; i++ )
        {
            char c = UART.read();
            uart1.printf("%c couner %d\r\n",c,j++);
            ebox_malloc(100);
        }

	}
}

void uart_write(int c)
{

    uart1.write(c);
}
//extern "C" 
//{    

//int fputc(int ch, FILE *f) {

//    uart1.write(ch);

//        return (ch);
//}
//}
