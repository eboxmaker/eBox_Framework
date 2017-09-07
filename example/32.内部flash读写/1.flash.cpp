#include "ebox.h"

Flash flash;//����һ���ڲ�flash��д����

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
        for(int i = 0; i <10; i++)//ʹ��random������д��������ֵ
        {
            wbuf[i] = random(100);
        }
        
        flash.write(0x8010000,wbuf,10);//��д������������д���ڲ�flash
        uart1.printf("write data\r\n");
        for(int i = 0; i <10; i++)
        {
            uart1.printf("%02d ",wbuf[i]);
        }
        uart1.printf("\r\n");
        uart1.printf("read data\r\n");
        flash.read(0x8010000,rbuf,10);//���ڲ�flash�е����ݶ�ȡ����������
        for(int i = 0; i <10; i++)
        {
            uart1.printf("%02d ",rbuf[i]);
        }
        uart1.printf("\r\n============================\r\n");
        delay_ms(1000);
    }
}
