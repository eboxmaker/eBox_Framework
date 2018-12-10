#include "ebox.h"
#include "bsp_ebox.h"

/**
    *	1	此例程演示了内部flash操作
	*	2	可以通过不同的起始地址，不同的页面数定义不同的flash区
	*	3	flash的起始地址在mcu_config中定义
	*   4	如果保存少量参数，建议每个page中存放一个参数，同时将ebox_config中的FLASH_OVERRIDE设置为1
    *       这样写入是直接覆盖当前page区内容，无需采用读入缓冲区，修改缓冲区，写回的流程，减少资源
	*/


/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"independent flash example"
#define EXAMPLE_DATE	"2018-08-08"


//Flash flash;//将所有的可用flasj创建一个内部flash读写对象

//从用户区起始地址创建一个闪存，默认为1 page,大小等于1*pagesize
Flash flash(0);
//// 从用户区起始地址+2page处，创建一个闪存，3 page,大小等于3*pagesize
//Flash flash(0,63);

uint8_t wbuf[10];
uint8_t rbuf[10];

void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);
}
int main(void)
{
    setup();
    random_seed(10);
    UART.printf("flash size %d kb \r\n", flash.getSize());
    while(1)
    {
        for(int i = 0; i < 10; i++) //使用random函数给写缓冲区赋值
        {
            wbuf[i] = random(100);
        }
        flash.write(0, wbuf, 10); //将写缓冲区的内容写入内部flash
        UART.printf("write data\r\n");
        for(int i = 0; i < 10; i++)
        {
            UART.printf("%02d ", wbuf[i]);
        }
        UART.printf("\r\n");
        UART.printf("read data\r\n");
        flash.read(0, rbuf, 10); //将内部flash中的内容读取到读缓冲区
        for(int i = 0; i < 10; i++)
        {
            UART.printf("%02d ", rbuf[i]);
        }
        UART.printf("\r\n============================\r\n");
        delay_ms(1000);
    }
}
