#include "ebox.h"
#include "bsp_ebox.h"

#include "servo.h"
/**
	*	1	此例程演示了Servo操作
	*	2	Servo是舵机的控制类，按照-100%（左满舵）到100%（右满舵）方式设定
	*/


/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"Servo example"
#define EXAMPLE_DATE	"2018-08-13"

Servo servo(&PB8);


void setup()
{
    ebox_init();
    uart1.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);

    servo.begin();

}
int main(void)
{
    setup();
    servo.setPct(0);
    float pct = 0, increase = 1;
    while (1)
    {
        pct += increase;
        if (pct >= 100 || pct <= -100)
        {
            increase = -increase;
        }
        servo.setPct(pct);
        uart1.printf("%f\n", servo.getPct());
        delay_ms(20);
    }
}