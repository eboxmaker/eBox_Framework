#include "ebox.h"
#include "bsp_ebox.h"

Flash flash;//创建一个内部flash读写对象

// 从用户区起始地址创建一个闪存，默认为1 page,大小等于1*pagesize
//Flash flash(0);
//// 从用户区起始地址+2page处，创建一个闪存，3 page,大小等于3*pagesize
//Flash flash(0,63);

uint8_t wbuf[10];
uint8_t rbuf[10];

void setup()
{
    ebox_init();
    UART.begin(115200);

}
int main(void)
{
    setup();

    random_seed(10);
    while(1)
    {
        for(int i = 0; i <10; i++)//使用random函数给写缓冲区赋值
        {
            wbuf[i] = random(100);
        }
        
        flash.write(0,wbuf,10);//将写缓冲区的内容写入内部flash
        UART.printf("write data\r\n");
        for(int i = 0; i <10; i++)
        {
            UART.printf("%02d ",wbuf[i]);
        }
        UART.printf("\r\n");
        UART.printf("read data\r\n");
        flash.read(0,rbuf,10);//将内部flash中的内容读取到读缓冲区
        for(int i = 0; i <10; i++)
        {
            UART.printf("%02d ",rbuf[i]);
        }
        UART.printf("\r\n============================\r\n");
        delay_ms(1000);
    }
}

//uint8_t buf[] = "flash test -  ";
//char rec[20];

//int main(void)
//{
//	int i = 20,j=0;
//	setup();
//	// 从地址0处读取1个字节，结果保存在rec中
//	Fh.read(0,(uint8_t*)rec,1);
//	UART.printf("flash test; 第 %d 次开机！ \r\n",rec[0]);
//	rec[0]++;
//	// 将读取到的rec[0]自加后重新写入iflash中，作为开机次数
//	Fh.write(0,(uint8_t*)rec,1);
//	while (1)
//	{	
//			while(j<2){
//				j++;
//				buf[13] = j+0x30;
//				i = fl.write(0,buf,sizeof(buf)/sizeof(buf[0]));
//				delay_ms(10);
//				if(i>0) {
//				fl.read(0,(uint8_t*)rec,i);
//				UART.write(rec,i-1);
//				UART.printf("\r\n");
//				}
//			 delay_ms(2000);
//			}
//	}
//}
