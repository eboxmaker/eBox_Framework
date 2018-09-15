#include "ebox.h"
#include "bsp_ebox.h"


/**
	*	1	此例程演示了UartStream操作，依赖ebox_uart_stream和core目录下stream
	*	2	UartStream是串口集成了stream的类。支持了stream的所有功能
	*		包括读取一个String，查找一个关键字，关键词等
	*   	但是这个类中都是阻塞性的读取，
    *       如果开始读取，就会等到读取结束后还会延时设定的超时时间。用户需注意使用
	*/
	

/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"UartStream example"
#define EXAMPLE_DATE	"2018-08-13"

UartStream uart1s(&UART);


void setup()
{
	ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME,EXAMPLE_DATE);
    

    uart1s.begin(115200);
    uart1s.setTimeout(1000);//定义超时时间
}
int main(void)
{
	setup();
	while (1)
	{
        String x = uart1s.readString();//
        
        if(x != NULL)
        {
            uart1s.print(x);
            uart1s.print('\t');
            uart1s.print(ebox_get_free()/1024.0);
            uart1s.println("KB");
        }
	}
}