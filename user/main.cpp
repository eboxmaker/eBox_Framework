#include "ebox.h"
#include "bsp_ebox.h"
#include "lcd_1.8.h"
#include "graphic.h"
#include "gui_button.h"
#include "gui_sidebar.h"
#include "gui.h"
/**
	*	1	��������ʾ��VLcd��������ʾ�����Ĳ���������ebox_virtual_lcd��font.h
	*	2	VLcdʵ����LCD�Ĵ󲿷ֹ��ܣ��ַ�����ӡ����������
    *       lcd�ǱȽϴ��һ����ʾ����������lcd���ƶ�ʵ���˴�����ʾ���ݶ�̬
    *       �仯Ч��
    *   3   ������ͨ�����ڴ�ӡ�˻���
	*/


/* ���������������̷������� */
#define EXAMPLE_NAME	"Virtual LCD example"
#define EXAMPLE_DATE	"2018-08-14"

Lcd lcd(&PB5,&PB6,&PB4,&PB3,&spi1);
Graphic gpu(&lcd,128,160);
GUI gui;
GuiButton btn(10,10,30,20);
GuiSideBar bar(1,0,100,3);
void setup()
{
    ebox_init();
    UART.begin(115200);
    lcd.begin();
    lcd.clear(BLUE);
    gpu.begin();
    gpu.clear();
    gpu.set_color(GREEN);


    bar.set_orientation(1);
    bar.set_max(30);
    bar.draw();
    btn.draw();
    gpu.fill_screen(GRAY1);
    gpu.btn_down(60,30,100,60);
    gpu.btn_up(60,100,100,130);
}
uint32_t start ,stop;
int main(void)
{
    int count = 32;
    setup();
    while (1)
    {
        start = millis();
        for(int k = 0; k < 10;k++)
        for(int i = 0; i < 30;i++)
        {
            bar.set_prograss(i);
            //bar.set_orientation(i%2);
            delay_ms(200);
        }
        stop=millis();
        uart1.println(stop - start);
    }
}