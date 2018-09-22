#include "ebox.h"
#include "bsp_ebox.h"


/**
	*	1	��������ʾ��UartStream����
	*	2	UartStream�Ǵ��ڼ�����stream���ࡣ֧����stream�����й���
	*		������ȡһ��String������һ���ؼ��֣��ؼ��ʵ�
	*   	����������ж��������ԵĶ�ȡ��
    *       �����ʼ��ȡ���ͻ�ȵ���ȡ�����󻹻���ʱ�趨�ĳ�ʱʱ�䡣�û���ע��ʹ��
	*/
	

/* ���������������̷������� */
#define EXAMPLE_NAME	"UartStream example"
#define EXAMPLE_DATE	"2018-08-13"

int x,y;

Timer timer1(TIM3);
const char *buf = "1234567890\r\n";
size_t len;
void t2it()
{
        x = uart1.availableForWrite();
        uart1.write("12345678\r\n");
        y = uart1.availableForWrite();;
        uart1.println(x);
        uart1.println(y);
//        uart1.flush();
}
void rx()
{
     PB9.toggle();
}
void tx()
{
//    len = sizeof("1234567890\r\n");
//    uart1.write(buf,12);
//    uart1.write(buf,12);
 PB8.toggle();
}
void setup()
{
	ebox_init();
    uart1.begin(115200);
//    no_interrupts();
//    print_log(EXAMPLE_NAME,EXAMPLE_DATE);
//    LED1.mode(OUTPUT_PP);
    uart1.attach(rx,RxIrq);
    uart1.attach(tx,TxIrq);
    uart1.interrupt(RxIrq,ENABLE);
    uart1.nvic(ENABLE,1,0);

    PB8.mode(OUTPUT_PP);
    PB9.mode(OUTPUT_PP);
    timer1.begin(2);
    timer1.nvic(ENABLE,0,0);
    timer1.attach(t2it);
    timer1.interrupt(ENABLE);
    timer1.start();
//    UART.printf("\r\ntimer clock       = %dMhz", timer1.get_timer_source_clock()/1000000);
//    UART.printf("\r\nmax interrupt frq = %dKhz", timer1.get_max_frq()/1000);
   

}
int main(void)
{
	setup();
//        uart1.write("12345678901234567890123456789012345678901234567890\r\n");
//        delay_ms(100);
//    interrupts();
	while (1)
	{
        len = uart1.available();
        for(int i = 0; i < len; i++ )
        {
            uart1.write(uart1.read());
        }
//        uart1.write('3');
//        uart1.write('4');
//        uart1.write('5');
//        uart1.write('6');
//        uart1.write('7');
//        uart1.write('8');
//        uart1.write('9');
//        uart1.write('a');
//        uart1.write('b');
//        uart1.write('c');
//        uart1.write('d');
//        uart1.write('\r');
//        uart1.write('\n');  
//        delay_ms(1000);


	}
}