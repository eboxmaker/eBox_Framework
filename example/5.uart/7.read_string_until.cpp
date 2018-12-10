#include "ebox.h"
#include "bsp_ebox.h"
#include "ebox_mem.h"

/**
	*	1	此例程演示了UartStream操作，
	*	2	UartStream是串口集成了stream的类。支持了stream的所有功能
	*		包括读取一个String，查找一个关键字，关键词等
	*   	readStringUntil是连续读串口直到遇到定义的关键字才停止读取，
    *       如果开始读取，超过了超时时间还没有遇到，则返回读到的内容。
    *       如果用户连续发送数据间隔不超过设定时间，则会将多次发送
    *       的数据拼接到一起。
	*/


/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"UartStream example"
#define EXAMPLE_DATE	"2018-08-13"


void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);


    UART.setTimeout(1000);//定义超时时间
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