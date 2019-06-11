#include "ebox.h"
#include "bsp_ebox.h"
#include "ebox_virtual_lcd.h"

#include "gui.h"

/**
	*	1	VLcd（虚拟显示器），依赖ebox_virtual_lcd,font.h
    *       和GUI组件
	*	2	VLcd实现了基本gui的接口。可以使用gui来实现顶层效果
    *   3   VLcd还像底层设备提供了虚拟lcd显存入口，大小等信息，
    *       可以直接写到真实的显示器上。
    *   4   此例程使用了gui创建了几个元素，并通过串口打印出来
	*/


/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"Virtual LCD example"
#define EXAMPLE_DATE	"2018-08-14"


VLcd lcdx;//创建一个虚拟显示器
GUI gui;//创建一个GUI对象

void setup()
{
    ebox_init();
    UART.begin(115200);

    lcdx.begin(32, 32); //初始化显示器宽度32，高度32
    gui.begin(&lcdx, 32, 32); //绑定显示器，并按照显示器尺寸初始化GUI
    gui.fill_screen(0);//清空屏幕
    gui.set_font(&GUI_FontHZ16X16);//设置字体


    gui.set_rotation(0);

}
int main(void)
{
    int count = 32;
    setup();
    while (1)
    {
        count--;

        gui.set_cursor(count, 0); //平移光标，使得字移动

        gui.set_color(GUI_GREEN);
        gui.printf("强");
        gui.draw_v_line(10, 10, 10);
        gui.draw_h_line(10, 10, 10);
        gui.set_color(GUI_RED);
        gui.fill_rect(0, 0, 5, 5);
        gui.draw_circle(16, 16, 16);
        gui.set_color(GUI_BLUE);
        gui.draw_line(10, 10, 30, 31);

        lcdx.print_1bit(UART);
        lcdx.print_color(UART);
        lcdx.clear(0);
        if(count == 0)count = 32;
        delay_ms(100);

    }
}