#include "ebox.h"
#include "bsp_ebox.h"

Flash flash;//����һ���ڲ�flash��д����

// ���û�����ʼ��ַ����һ�����棬Ĭ��Ϊ1 page,��С����1*pagesize
//Flash flash(0);
//// ���û�����ʼ��ַ+2page��������һ�����棬3 page,��С����3*pagesize
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
        for(int i = 0; i <10; i++)//ʹ��random������д��������ֵ
        {
            wbuf[i] = random(100);
        }
        
        flash.write(0,wbuf,10);//��д������������д���ڲ�flash
        UART.printf("write data\r\n");
        for(int i = 0; i <10; i++)
        {
            UART.printf("%02d ",wbuf[i]);
        }
        UART.printf("\r\n");
        UART.printf("read data\r\n");
        flash.read(0,rbuf,10);//���ڲ�flash�е����ݶ�ȡ����������
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
//	// �ӵ�ַ0����ȡ1���ֽڣ����������rec��
//	Fh.read(0,(uint8_t*)rec,1);
//	UART.printf("flash test; �� %d �ο����� \r\n",rec[0]);
//	rec[0]++;
//	// ����ȡ����rec[0]�ԼӺ�����д��iflash�У���Ϊ��������
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
