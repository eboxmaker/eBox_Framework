#include "ebox.h"
#include "bsp_ebox.h"
#include "soft_uart.h"

/**
	*	1	此例程演示了uart的收发操作
*/


/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"UartStream example"
#define EXAMPLE_DATE	"2018-08-13"

SoftUart suart(&PA9,&PA10);
SoftUart suart1(&PB9,&PA10);
int len;
Timer timer2(TIM2);
void t2it()
{
suart.loop_230400();
//suart1.loop_230400();
    PB8.toggle();
}
void setup()
{
    ebox_init();
   // UART.begin(115200);
   // print_log(EXAMPLE_NAME, EXAMPLE_DATE);

    suart.begin(36000);
    suart1.begin(36000);
    
    timer2.begin(360000);
    timer2.attach(t2it);
    //    timer2.attach(&test,&Test::event);
    timer2.interrupt(ENABLE);
    timer2.start();
    PB8.mode(OUTPUT);


}
int main(void)
{
    uint32_t j = 0;
    setup();

    while (1)
    {

        suart.printf("123");;
        //suart1.printf("123\r\n");;
        delay_ms(1000);
        
    }
}



