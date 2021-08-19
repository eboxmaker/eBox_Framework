#ifndef __GRAPHIC_H
#define __GRAPHIC_H

#include "ebox_core.h"
#include "gui_font.h"
#include "hmi.h"
//#define GUI_TS_NORMAL           (0)
//#define GUI_TS_UNDERLINE        (1 << 0)
//#define GUI_TS_STRIKETHRU       (1 << 1)
//#define GUI_TS_OVERLINE         (1 << 2)


//#define DRAW_MODE_NORMAL (0)//正常
//#define DRAW_MODE_XOR    (1<<0)//异或
//#define DRAW_MODE_TRANS  (1<<1)//透明
//#define DRAW_MODE_REV    (1<<2)//翻转

//#define TEXT_MODE_NORMAL DRAW_MODE_NORMAL//正常
//#define TEXT_MODE_XOR    DRAW_MODE_XOR//异或
//#define TEXT_MODE_TRANS  DRAW_MODE_TRANS//透明
//#define TEXT_MODE_REV    DRAW_MODE_REV//翻转


DECLARE_ENUM(GuiDrawMode)
Normal = 0,     //正常
Xor,            //异或
Trans,          //透明
Reverse         //翻转
END_ENUM();

#ifndef _swap_int16_t
#define _swap_int16_t(a, b) { int16_t t = a; a = b; b = t; }
#endif

class Graphic
{
    
    public:
        enum ColorMode{RGB888,RGB565,GRAY};
        uint32_t color, back_color;

        GUI_FONT *current_font;
        uint16_t _width, _height; // Display w/h as modified by current rotation

private:
    HMI     *lcd;
    uint8_t  rotation;
    uint16_t lcd_width, lcd_height; //  LCD属性。初始化后不可更改
    int16_t  cursor_x, cursor_y;
    uint8_t  draw_mode;


    //字体相关设置
    FontSelect_t font_select;
    GuiDrawMode  text_mode;
    uint8_t  text_style;
    uint8_t  text_auto_reline;
    //外部字库的传入接口
    eBoxCharInfo_t char_info;
    uint8_t text_extern_font_ascii_id;
    uint8_t text_extern_font_hz_id;
    
    bool text_font_ascii_extern_enable;
    bool text_font_hz_extern_enable;
public:
    Graphic(HMI *_lcd, uint16_t w, uint16_t h);
    void begin();

    //fun
    void clear();
    void flush();
    //settings
    void        set_rotation(uint8_t value);
    int16_t     ro_x(int16_t x, int16_t y);
    int16_t     ro_y(int16_t x, int16_t y);
    void        set_color(uint32_t color);
    void        set_back_color(uint32_t back_color);
    void        set_cursor(int16_t x, int16_t y);
    void        set_draw_mode(uint8_t mode);
    uint16_t    bgr2rgb(uint16_t c);

    //port
    void draw_pixel();
    void draw_pixel(uint32_t color);
    void draw_pixel(int16_t x, int16_t y);
    void draw_pixel(int16_t x, int16_t y, uint32_t color);

    void draw_h_line(int16_t x0, int16_t y0, int16_t x1);
    void draw_h_line(int16_t x0, int16_t y0, int16_t x1, uint32_t color);
    
    void draw_v_line(int16_t x0, int16_t y0, int16_t y1);
    void draw_v_line(int16_t x0, int16_t y0, int16_t y1, uint32_t color);
    
    void draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1);
    void draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint32_t color);
    
    void draw_rect(int16_t x, int16_t y, int16_t x1, int16_t y1);
    void draw_rect(int16_t x, int16_t y, int16_t x1, int16_t y1, uint32_t color);
    
    void fill_rect(int16_t x0, int16_t y0, int16_t x1, int16_t y1);
    void fill_rect(int16_t x0, int16_t y0, int16_t x1, int16_t y1,uint32_t color);
    void fill_screen(uint32_t color) ;

    //graphic
    void draw_circle(int16_t x0, int16_t y0, int16_t r);
    void fill_circle(int16_t x0, int16_t y0, int16_t r);
    void draw_circle_helper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername);
    void fill_circle_helper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta);
    void draw_round_rect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius);
    void fill_round_rect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius);
    void draw_triangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2);
    void fill_triangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2);



    void attach( bool (*api)(uint16_t inner_code,uint8_t font_id,eBoxCharInfo_t *info));


//text
    //设置字体
    void set_font_select(FontSelect_t select);//设置字库选择
    void set_font(const GUI_FONT *font);//设置内部字库
    void set_font_ascii_extern(uint8_t font_id);//设置外部ASCII字库
    void set_font_hz_extern(uint8_t font_id);//设置外部汉子字库
    eBoxCharInfo_t get_extern_char_info(){return char_info;};
    void set_text_style(uint8_t style);//无效
    void set_text_mode(GuiDrawMode mode);//设置填充模式
    void set_text_auto_reline(uint8_t enable);
    
    //解码转换,打印字符等函数
    void char_index_of_font(uint16_t code, const GUI_FONT_PROP **font_list, uint16_t *index);
    void disp_index(const GUI_FONT_PROP *font_list, uint16_t index);
    void disp_char(uint16_t ch);
    void disp_chars(uint16_t ch, uint16_t cun);
    void disp_char_at(uint16_t ch, int16_t x, int16_t y);
    void disp_string(const char *str);
    void disp_string_at(const char *str, int16_t x, int16_t y);
    void printf(const char *fmt, ...);
    void printf(int16_t x, int16_t y, const char *fmt, ...);
    
    uint8_t get_char_xlength(uint16_t ch,GUI_FONT *font);
    uint16_t get_string_xlength( const char *fmt, ...);
    
//    void box(int16_t x, int16_t y, int16_t w, int16_t h, int16_t bc);
//    void box2(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t mode);

    //bitmap
    void drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint32_t color);
    void drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint32_t color, uint16_t bg);
    void drawBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint32_t color) ;
    void drawBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint32_t color, uint16_t bg) ;
    void drawXBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint32_t color) ;
};
extern Graphic *_gpu;
#endif
