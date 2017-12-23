/**
  ******************************************************************************
  * @file   : *.cpp
  * @author : shentq
  * @version: V1.2
  * @date   : 2016/08/14

  * @brief   ebox application example .
  *
  * Copyright 2016 shentq. All Rights Reserved.         
  ******************************************************************************
 */
 
 
#include "ebox.h"
#include "lcd_1.8.h"
#include "color_convert.h"
#include "mcu_mem.h"
#include "mem_test.h"
#define BYTE_ALIGNMENT          (8)                 //�ֽڶ���
	#define BYTE_ALIGNMENT_MASK ( 0x00000007 )
                                            
extern int Image$$RW_IRAM1$$ZI$$Limit;

//����������eBox
//              GND   ��Դ��
//              VCC   ��5V��3.3v��Դ
//              SCL   ��PA5��SCL��
//              SDA   ��PA7��SDA��
//              RES   ��PB3 (RST)
//              DC    ��PB4 (RS)
//              CS    ��PB5 
//              BL    ��PB6

COLOR_HSV hsv;
COLOR_RGB rgb;

Lcd lcd(&PB5, &PB6, &PB4, &PB3, &spi1);
#define ALIGN(size, align)      ((align + size - 1) & (~ (size - 1)))
#define MEM_ALIGN(size)         ((8 + size - 1) & (~ (size - 1)))


#define RT_ALIGN(size, align)           (((size) + (align) - 1) & ~((align) - 1))


#define RT_ALIGN_DOWN(size, align)      ((size) & ~((align) - 1))
u8 index = 0x20;
u8 r;
u16 _color[3600] __attribute__((section("Region$$Table$$Base")));

uint8_t *ptr;
void setup()
{
    ebox_init();
    uart1.begin(115200);
    ptr = (uint8_t *)ebox_malloc(1);
    uart1.printf("%ld\r\n",123);
    uart1.printf("%ld\r\n",MEM_ALIGN(31));
    uart1.printf("%ld\r\n",MEM_ALIGN(1022));
    while(1);
    PB8.mode(OUTPUT_PP);
    lcd.begin(1);
    lcd.clear(RED);
    uart1.begin(9600);

    lcd.column_order(1);
    lcd.row_order(1);

    lcd.front_color = RED;
    lcd.back_color = BLACK;
    hsv.s = 1;
    hsv.v = 0.5;
    hsv.h = 0;

    lcd.front_color = RED;
    if(index >= 0x50)index = 0x20;
    for(int i = 0; i < 160; i++)
    {
        hsv.h = i * 36 / 16;
        hsv.h %= 360;
        HSV_to_RGB(hsv, rgb);
        rgb_to_565(rgb, _color[i]);
        lcd.front_color = _color[i];
        lcd.draw_h_line(0, i, 128,BLUE);
    }
    lcd.disp_char8x16(0, 0, index++);

    lcd.printf(2, 2, "1231asddfgdsfgthkfhddddj2nhd");


    lcd.front_color = GREEN;
    lcd.draw_circle(50, 50, 50,YELLOW);
    lcd.draw_line(64, 50, r, 100,GREEN);


}
int main(void)
{
    setup();
    while(1)
    {
        delay_ms(1000);
        PB8.toggle();
    }

}


