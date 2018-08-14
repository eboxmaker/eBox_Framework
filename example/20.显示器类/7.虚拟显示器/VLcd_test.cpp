#include "ebox.h"
#include "bsp_ebox.h"
#include "ebox_virtual_lcd.h"

#include "gui.h"

/**
	*	1	VLcd��������ʾ����������ebox_virtual_lcd,font.h
    *       ��GUI���
	*	2	VLcdʵ���˻���gui�Ľӿڡ�����ʹ��gui��ʵ�ֶ���Ч��
    *   3   VLcd����ײ��豸�ṩ������lcd�Դ���ڣ���С����Ϣ��
    *       ����ֱ��д����ʵ����ʾ���ϡ�
    *   4   ������ʹ����gui�����˼���Ԫ�أ���ͨ�����ڴ�ӡ����
	*/
	

/* ���������������̷������� */
#define EXAMPLE_NAME	"Virtual LCD example"
#define EXAMPLE_DATE	"2018-08-14"


VLcd lcdx;//����һ��������ʾ��
GUI gui;//����һ��GUI����

void setup()
{
	ebox_init();
    UART.begin(115200);
    
    lcdx.begin(32,32);//��ʼ����ʾ�����32���߶�32
    gui.begin(&lcdx,32,32);//����ʾ������������ʾ���ߴ��ʼ��GUI
    gui.fill_screen(0);//�����Ļ
    gui.set_font(&GUI_FontHZ16X16);//��������


    gui.set_rotation(0);

}
int main(void)
{
    int count =32;
	setup();
	while (1)
	{
        count--;

        gui.set_cursor(count,0);//ƽ�ƹ�꣬ʹ�����ƶ�

        gui.set_color(GUI_GREEN);
        gui.printf("ǿ");
        gui.draw_v_line(10,10,10);
        gui.draw_h_line(10,10,10);
        gui.set_color(GUI_RED);
        gui.fill_rect(0,0,5,5);
        gui.draw_circle(16,16,16);
        gui.set_color(GUI_BLUE);
        gui.draw_line(10,10,30,31);

        lcdx.print_1bit(UART);
        lcdx.print_color(UART);
        lcdx.clear(0);
        if(count == 0)count = 32;
        delay_ms(100);
        
	}
}