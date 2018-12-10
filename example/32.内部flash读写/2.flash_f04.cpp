#include "ebox.h"
#include "bsp_ebox.h"

/**
    *	1	��������ʾ���ڲ�flash����
	*	2	����ͨ����ͬ����ʼ��ַ����ͬ��ҳ�������岻ͬ��flash��
	*	3	flash����ʼ��ַ��mcu_config�ж���
	*   4	���������������������ÿ��page�д��һ��������ͬʱ��ebox_config�е�FLASH_OVERRIDE����Ϊ1
    *       ����д����ֱ�Ӹ��ǵ�ǰpage�����ݣ�������ö��뻺�������޸Ļ�������д�ص����̣�������Դ
	*/


/* ���������������̷������� */
#define EXAMPLE_NAME	"independent flash example"
#define EXAMPLE_DATE	"2018-08-08"


//Flash flash;//�����еĿ���flasj����һ���ڲ�flash��д����

//���û�����ʼ��ַ����һ�����棬Ĭ��Ϊ1 page,��С����1*pagesize
Flash flash(0);
//// ���û�����ʼ��ַ+2page��������һ�����棬3 page,��С����3*pagesize
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
        for(int i = 0; i < 10; i++) //ʹ��random������д��������ֵ
        {
            wbuf[i] = random(100);
        }
        flash.write(0, wbuf, 10); //��д������������д���ڲ�flash
        UART.printf("write data\r\n");
        for(int i = 0; i < 10; i++)
        {
            UART.printf("%02d ", wbuf[i]);
        }
        UART.printf("\r\n");
        UART.printf("read data\r\n");
        flash.read(0, rbuf, 10); //���ڲ�flash�е����ݶ�ȡ����������
        for(int i = 0; i < 10; i++)
        {
            UART.printf("%02d ", rbuf[i]);
        }
        UART.printf("\r\n============================\r\n");
        delay_ms(1000);
    }
}
