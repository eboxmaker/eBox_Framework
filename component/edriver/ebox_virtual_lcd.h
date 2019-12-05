#ifndef __ebox_virtual_lcd_h
#define __ebox_virtual_lcd_h

#include "ebox_core.h"
#include "interface/hmi.h"
#include "print.h"
#include "font.h"

class VLcd : public HMI, public Print
{

private:
    //ws2812��ʽ��GRB 24λģʽ���ȷ���λ
    typedef struct
    {
        uint8_t g;
        uint8_t r;
        uint8_t b;
    } LcdColor_t;

    typedef union
    {
        LcdColor_t color;//���ص���ɫ
        uint8_t    byte[3];//���ص�byteѰַ
    } LcdPixel_t;

    typedef struct
    {
        LcdPixel_t *pixel;//�Դ�����ָ��
        uint8_t     width;//��ʾ�����
        uint8_t     height;//��ʾ���߶�
        uint32_t    mem_size;//�Դ��С
    } Lcd_t;

public:
    VLcd();
    ~VLcd();
    void    begin(uint16_t lcd_w, uint16_t lcd_h);
    void    set_xy(int16_t x, int16_t y);
    void    clear(uint32_t color);
    void    disp_char6x8(int16_t x, int16_t y, uint8_t ch);
    void    disp_char8x16(int16_t x, int16_t y, uint8_t ch);


    //����Print�Ľӿڡ�����guiҲ�����򵥵��ַ�����ӡ����
    virtual size_t write(uint8_t ch);
    using Print::write;

    //����Vhmi�Ľӿ�
    virtual void    draw_pixel(int16_t x, int16_t y, uint32_t color);
    virtual void    fill_screen(uint32_t color);

    //���������ʾ����
    void    print_1bit(Print &p);
    void    print_color(Print &p);

    //�Դ�����ӿڣ����ײ���ʾ�豸���á�����WS2812��DMA
    uint8_t *lcd_mem_ptr()
    {
        return (uint8_t *)lcd.pixel;
    }
    uint32_t lcd_mem_size()
    {
        return lcd.mem_size;
    }
    uint8_t lcd_width()
    {
        return lcd.width;
    }
    uint8_t lcd_height()
    {
        return lcd.height;
    }


public:
    Lcd_t   lcd;
    int16_t current_x, current_y; //���ڵ���print��ع���ʱʹ��

private:
    int32_t position_to_index(int16_t x, int16_t y); //x,yλ��ת��Ϊ�ڴ�����ֵ
    bool    is_pixel_vail(int16_t x, int16_t y); //���x,yλ���Ƿ�����ɫ��ʵ����ת��1bit�Ĺ���





};

#endif

