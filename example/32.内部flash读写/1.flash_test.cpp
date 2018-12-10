#include "ebox.h"
#include "bsp_ebox.h"

/**
    *	1	此例程演示了利用内部flash保存参数
	*	2	其中fl为从起始地址开始1个page大小的区域，保存开机次数
	*	3	fh为从起始地址处偏移3个page起始，3个page大小的区域，保存计算变量
	*/


/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"independent flash example"
#define EXAMPLE_DATE	"2018-10-18"

//从用户区起始地址创建一个闪存，默认为1 page,大小等于1*pagesize
Flash fl(0);
//// 从用户区起始地址+3page处，创建一个闪存，3 page,大小等于3*pagesize
Flash fh(3, 3);

uint8_t wbuf[10];
uint8_t rbuf[10];

void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);
    UART.printf("fl size %dkb, fh size %dkb\r\n", fl.getSize(), fh.getSize());
}


uint8_t buf[] = "flash test -  ";
uint8_t rec[20];

int main(void)
{
    int i = 20, j = 0;
    setup();
    // 从地址0处读取1个字节，结果保存在rec中
    fh.read(0, rec, 1);
    UART.printf("flash test; 第 %d 次开机！ \r\n", rec[0]);
    rec[0]++;
    // 将读取到的rec[0]自加后重新写入iflash中，作为开机次数
    fh.write(0, rec, 1);
    while (1)
    {
        while(j < 2)
        {
            j++;
            buf[13] = j + 0x30;
            i = fl.write(0, buf, sizeof(buf) / sizeof(buf[0]));
            delay_ms(10);
            if(i > 0)
            {
                fl.read(0, (uint8_t *)rec, i);
                UART.write(rec, i - 1);
                UART.printf("\r\n");
            }
            delay_ms(2000);
        }
    }
}
