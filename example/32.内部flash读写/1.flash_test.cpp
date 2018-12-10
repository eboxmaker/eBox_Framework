#include "ebox.h"
#include "bsp_ebox.h"

/**
    *	1	��������ʾ�������ڲ�flash�������
	*	2	����flΪ����ʼ��ַ��ʼ1��page��С�����򣬱��濪������
	*	3	fhΪ����ʼ��ַ��ƫ��3��page��ʼ��3��page��С�����򣬱���������
	*/


/* ���������������̷������� */
#define EXAMPLE_NAME	"independent flash example"
#define EXAMPLE_DATE	"2018-10-18"

//���û�����ʼ��ַ����һ�����棬Ĭ��Ϊ1 page,��С����1*pagesize
Flash fl(0);
//// ���û�����ʼ��ַ+3page��������һ�����棬3 page,��С����3*pagesize
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
    // �ӵ�ַ0����ȡ1���ֽڣ����������rec��
    fh.read(0, rec, 1);
    UART.printf("flash test; �� %d �ο����� \r\n", rec[0]);
    rec[0]++;
    // ����ȡ����rec[0]�ԼӺ�����д��iflash�У���Ϊ��������
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
