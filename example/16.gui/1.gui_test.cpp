#include "ebox.h"
#include "bsp_ebox.h"
#include "ebox_virtual_lcd.h"

#include "gui.h"
/**
	*	1	此例程演示了VLcd（虚拟显示器）的操作，依赖ebox_virtual_lcd和font.h
	*	2	VLcd实现了LCD的大部分功能，字符串打印。字体设置
    *       lcd是比较大的一个显示器，窗口在lcd上移动实现了窗口显示内容动态
    *       变化效果
    *   3   本例程通过串口打印了缓存
	*/


/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"Virtual LCD example"
#define EXAMPLE_DATE	"2018-08-14"


VLcd lcdx(32, 8);
GUI gui;
void setup()
{
    ebox_init();
    UART.begin(115200);
    lcdx.begin();
    gui.begin(&lcdx, 32, 8);
    lcdx.disp_char6x8(-3, 0, 'a');
    lcdx.disp_char6x8(1 * 6 - 3, 0, 'b');
    lcdx.disp_char6x8(2 * 6 - 3, 0, '1');
    lcdx.disp_char6x8(3 * 6 - 3, 0, '2');
    lcdx.disp_char6x8(4 * 6 - 3, 0, '3');
    lcdx.print();
    lcdx.print_color();


}
int main(void)
{
    int count = 32;
    setup();
    while (1)
    {
        count--;
        //    lcdx.disp_char6x8(-count,0,'a');
        //    lcdx.disp_char6x8(1*6-count,0,'b');
        //    lcdx.disp_char6x8(2*6-count,0,'1');
        //    lcdx.disp_char6x8(3*6-count,0,'2');
        //    lcdx.disp_char6x8(4*6-count,0,'3');
        lcdx.set_pixel(count, 0);
        lcdx.printf("12:30");
        lcdx.print();
        lcdx.clear(0);
        if(count == 0)count = 32;
        delay_ms(100);

    }
}