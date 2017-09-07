#include "ebox.h"

Flash flash;//创建一个内部flash读写对象

uint8_t wbuf[10];
uint8_t rbuf[10];

void setup()
{
    ebox_init();
    uart1.begin(115200);

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
        
        flash.write(0x8010000,wbuf,10);//将写缓冲区的内容写入内部flash
        uart1.printf("write data\r\n");
        for(int i = 0; i <10; i++)
        {
            uart1.printf("%02d ",wbuf[i]);
        }
        uart1.printf("\r\n");
        uart1.printf("read data\r\n");
        flash.read(0x8010000,rbuf,10);//将内部flash中的内容读取到读缓冲区
        for(int i = 0; i <10; i++)
        {
            uart1.printf("%02d ",rbuf[i]);
        }
        uart1.printf("\r\n============================\r\n");
        delay_ms(1000);
    }
}
