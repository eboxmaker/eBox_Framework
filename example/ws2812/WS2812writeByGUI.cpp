#include "ebox.h"
#include "bsp_ebox.h"

#include "ws2812.h"
#include "ebox_virtual_lcd.h"
#include "gui.h"
#include "color_convert.h"
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


WS2812 led(&PB0);

VLcd lcdx;//创建一个虚拟显示器
GUI gui;//创建一个GUI对象

COLOR_HSV hsv;
COLOR_RGB rgb;

void setup()
{
    ebox_init();
    //    UART.begin(115200);

    lcdx.begin(8, 8); //初始化显示器宽度32，高度32
    gui.begin(&lcdx, 8, 8); //绑定显示器，并按照显示器尺寸初始化GUI
    gui.fill_screen(0);//清空屏幕
    gui.set_font(&GUI_Font8_ASCII);//设置字体

    led.begin();

    gui.set_rotation(0);
    hsv.s = 1;
    hsv.v = 0.1;
}

int main(void)
{
    int count = 8;
    uint32_t color;
    int i = 0;

    setup();
    while (1)
    {
        count--;

        for(int j = 0; j < 10; j++)
        {
            //色彩控制
            i++;
            if(i > 360)i = 0;
            hsv.h = i % 360;

            HSV_to_RGB(hsv, rgb);
            color = (rgb.r << 16) + (rgb.g << 8) + rgb.b;
            gui.set_color(color);

            //平移控制
            gui.set_cursor(count, 0); //平移光标，使得字移动

            //显示内容
            gui.printf("12:30");

            //输出到显示器
            //led.rainbow_Loop1();
            led.display(lcdx.lcd_mem_ptr(), lcdx.lcd.height, lcdx.lcd.width);
            //清空虚拟显示器
            lcdx.clear(0);
        }

        if(count == -32)
        {
            delay_ms(500);
            count = 8;
        }

        delay_ms(40);

    }
}