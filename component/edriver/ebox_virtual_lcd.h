#ifndef __ebox_virtual_lcd_h
#define __ebox_virtual_lcd_h

#include "ebox_core.h"
#include "interface/hmi.h"
#include "print.h"
#include "font.h"

class VLcd : public HMI, public Print
{

private:
    //ws2812格式：GRB 24位模式。先发高位
    typedef struct
    {
        uint8_t g;
        uint8_t r;
        uint8_t b;
    } LcdColor_t;

    typedef union
    {
        LcdColor_t color;//像素的颜色
        uint8_t    byte[3];//像素的byte寻址
    } LcdPixel_t;

    typedef struct
    {
        LcdPixel_t *pixel;//显存的入口指针
        uint8_t     width;//显示器宽度
        uint8_t     height;//显示器高度
        uint32_t    mem_size;//显存大小
    } Lcd_t;

public:
    VLcd();
    ~VLcd();
    void    begin(uint16_t lcd_w, uint16_t lcd_h);
    void    set_xy(int16_t x, int16_t y);
    void    clear(uint32_t color);
    void    disp_char6x8(int16_t x, int16_t y, uint8_t ch);
    void    disp_char8x16(int16_t x, int16_t y, uint8_t ch);


    //重载Print的接口。脱离gui也可做简单的字符串打印功能
    virtual size_t write(uint8_t ch);
    using Print::write;

    //重载Vhmi的接口
    virtual void    draw_pixel(int16_t x, int16_t y, uint32_t color);
    virtual void    fill_screen(uint32_t color);

    //串口输出显示功能
    void    print_1bit(Print &p);
    void    print_color(Print &p);

    //显存输出接口，被底层显示设备调用。比如WS2812的DMA
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
    int16_t current_x, current_y; //仅在调用print相关功能时使用

private:
    int32_t position_to_index(int16_t x, int16_t y); //x,y位置转换为内存索引值
    bool    is_pixel_vail(int16_t x, int16_t y); //检查x,y位置是否有颜色，实现了转换1bit的功能





};

#endif

