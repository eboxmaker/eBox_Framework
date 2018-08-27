#include "ebox.h"
#include "bsp_ebox.h"

#include "ws2812.h"
#include "ebox_virtual_lcd.h"
#include "gui.h"
#include "color_convert.h"
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


WS2812 led(&PB0);

VLcd lcdx;//����һ��������ʾ��
GUI gui;//����һ��GUI����

COLOR_HSV hsv;
COLOR_RGB rgb;
    
void setup()
{
	ebox_init();
//    UART.begin(115200);
    
    lcdx.begin(8,8);//��ʼ����ʾ�����32���߶�32
    gui.begin(&lcdx,8,8);//����ʾ������������ʾ���ߴ��ʼ��GUI
    gui.fill_screen(0);//�����Ļ
    gui.set_font(&GUI_Font8_ASCII);//��������

    led.begin();

    gui.set_rotation(0);
            hsv.s = 1;
            hsv.v = 0.1;}

int main(void)
{
    int count =8;
    uint32_t color;
    int i = 0;
    
	setup();
	while (1)
	{
        count--;
        
        for(int j = 0; j < 10;j++)
        {
            //ɫ�ʿ���
            i++;
            if(i>360)i=0;
            hsv.h = i % 360;

            HSV_to_RGB(hsv, rgb);
            color = (rgb.r<<16) + (rgb.g << 8) + rgb.b;
            gui.set_color(color);
            
            //ƽ�ƿ���
            gui.set_cursor(count,0);//ƽ�ƹ�꣬ʹ�����ƶ�
            
            //��ʾ����
            gui.printf("12:30");

            //�������ʾ��
            //led.rainbow_Loop1();
            led.display(lcdx.lcd_mem_ptr(),lcdx.lcd.height,lcdx.lcd.width);
            //���������ʾ��
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