#include "ebox.h"
#include "bsp_ebox.h"
#include "ebox_mem.h"

/**
	*	1	此例程演示了UartStream操作，
	*	2	UartStream是串口集成了stream的类。支持了stream的所有功能
	*		包括读取一个String，查找一个关键字，关键词等
	*   	parseFloat是在规定的超时时间内读取读取数据流并解析出所有的
    *       表示float类型的字符串。用户可向串口发送123.4567.89.26进行测试
	*/


/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"UartStream example"
#define EXAMPLE_DATE	"2018-08-13"

char buffer[100];
float value;
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
        if(UART.available())
        {
            value = UART.parseFloat();
            UART.println(value);
        }


    }
}