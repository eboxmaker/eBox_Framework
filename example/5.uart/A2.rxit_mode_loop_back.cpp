#include "ebox.h"
#include "bsp_ebox.h"


/**
	*	1	此例程演示了UartStream操作
	*	2	UartStream是串口集成了stream的类。支持了stream的所有功能
	*		包括读取一个String，查找一个关键字，关键词等
	*   	但是这个类中都是阻塞性的读取，
    *       如果开始读取，就会等到读取结束后还会延时设定的超时时间。用户需注意使用
	*/
	

/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"Uart1,2,3 RxIt mode loop back example"
#define EXAMPLE_DATE	"2018-9-23"


size_t len;

void rx()
{
     LED1.toggle();
}
void rx2()
{
     LED2.toggle();
}

void setup()
{
	ebox_init();
    
    LED1.mode(OUTPUT_PP);
    LED2.mode(OUTPUT_PP);
    
    uart1.begin(115200,RxIt);
    uart1.attach(rx,RxIrq);

    uart2.begin(115200,RxIt);
    uart2.attach(rx2,RxIrq);

    uart3.begin(115200,RxIt);
    uart3.attach(rx2,RxIrq);

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
            uart1.write(uart1.read());
        }
        len = uart2.available();
        for(int i = 0; i < len; i++ )
        {
            uart2.write(uart2.read());
        }      
        len = uart3.available();
        for(int i = 0; i < len; i++ )
        {
            uart3.write(uart3.read());
        }


	}
}