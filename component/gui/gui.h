#ifndef __GUI_H
#define __GUI_H
#include "ebox.h"

#define GUI_COLOR_BIT 24 //定义gui颜色位数

#if GUI_COLOR_BIT == 16
#define GUI_RED  	0xf800
#define GUI_GREEN	0x07e0
#define GUI_BLUE 	0x001f
#define GUI_WHITE	0xffff
#define GUI_BLACK	0x0000
#define GUI_YELLOW  0xFFE0
#define GUI_GRAY0   0xEF7D   	//灰色0 3165 00110 001011 00101
#define GUI_GRAY1   0x8410      	//灰色1      00000 000000 00000
#define GUI_GRAY2   0x4208      	//灰色2  1111111111011111
#elif GUI_COLOR_BIT == 24
#define GUI_BLUE            0xFF0000
#define GUI_GREEN           0x00FF00
#define GUI_RED             0x0000FF
#define GUI_CYAN            0xFFFF00
#define GUI_MAGENTA         0xFF00FF
#define GUI_YELLOW          0x00FFFF
#define GUI_LIGHTBLUE       0xFF8080
#define GUI_LIGHTGREEN      0x80FF80
#define GUI_LIGHTRED        0x8080FF
#define GUI_LIGHTCYAN       0xFFFF80
#define GUI_LIGHTMAGENTA    0xFF80FF
#define GUI_LIGHTYELLOW     0x80FFFF
#define GUI_DARKBLUE        0x800000
#define GUI_DARKGREEN       0x008000
#define GUI_DARKRED         0x000080
#define GUI_DARKCYAN        0x808000
#define GUI_DARKMAGENTA     0x800080
#define GUI_DARKYELLOW      0x008080
#define GUI_WHITE           0xFFFFFF
#define GUI_LIGHTGRAY       0xD3D3D3
#define GUI_GRAY            0x808080
#define GUI_DARKGRAY        0x404040
#define GUI_BLACK           0x000000

#define GUI_BROWN           0x2A2AA5

#define GUI_GRAY0           0xEF7D   	//灰色0 3165 00110 001011 00101
#define GUI_GRAY1           0x8410      	//灰色1      00000 000000 00000
#define GUI_GRAY2           0x4208      	//灰色2  1111111111011111

#define GUI_INVALID_COLOR 0xFFFFFFF      /* Invalid color - more than 24 bits */
#endif


#define GUI_TS_NORMAL           (0)
#define GUI_TS_UNDERLINE        (1 << 0)
#define GUI_TS_STRIKETHRU       (1 << 1)
#define GUI_TS_OVERLINE         (1 << 2)


#define DRAW_MODE_NORMAL (0)//正常
#define DRAW_MODE_XOR    (1<<0)//异或
#define DRAW_MODE_TRANS  (1<<1)//透明
#define DRAW_MODE_REV    (1<<2)//翻转

#define TEXT_MODE_NORMAL DRAW_MODE_NORMAL//正常
#define TEXT_MODE_XOR    DRAW_MODE_XOR//异或
#define TEXT_MODE_TRANS  DRAW_MODE_TRANS//透明
#define TEXT_MODE_REV    DRAW_MODE_REV//翻转

typedef struct
{
    uint8_t XSize;
    uint8_t XDist;
    uint8_t BytesPerLine;
    const unsigned char   *pData;
} GUI_CHARINFO;
typedef struct GUI_FONT_PROP
{
    uint16_t First;                                /* first character               */
    uint16_t Last;                                 /* last character                */
    const GUI_CHARINFO   *paCharInfo;            /* address of first character    */
    const struct GUI_FONT_PROP   *pNext;        /* pointer to next */
} GUI_FONT_PROP;

typedef struct _tFont
{
    const GUI_FONT_PROP *list;
    uint16_t    YSize;
    uint8_t     YDist;
} GUI_FONT;

/*************************************
 * GUI FONTS
*************************************/
extern const GUI_FONT GUI_Font8_ASCII;
extern const GUI_FONT GUI_Font16_ASCII;
extern const GUI_FONT GUI_Font24_ASCII;
extern const GUI_FONT GUI_Font32_ASCII;
extern const GUI_FONT GUI_FontHZ16X16;



#ifndef _swap_int16_t
#define _swap_int16_t(a, b) { int16_t t = a; a = b; b = t; }
#endif

/*************************************
 * GUI API
*************************************/
class GUI
{
private:
    Vhmi     *lcd;
    uint8_t  rotation;
    uint16_t width, height; //  LCD属性。初始化后不可更改
    uint16_t _width, _height; // Display w/h as modified by current rotation
    int16_t  cursor_x, cursor_y;

    uint32_t color, back_color;
    uint8_t  draw_mode;

    //字体相关设置
    GUI_FONT *current_font;
    uint8_t  text_mode;
    uint8_t  text_style;
    uint8_t  text_auto_reline;


public:

    void begin(Vhmi *_lcd, uint16_t w, uint16_t h);

    //settings
    void        set_rotation(uint8_t value);
    int16_t     ro_x(int16_t x, int16_t y);
    int16_t     ro_y(int16_t x, int16_t y);
    void        set_color(uint32_t color);
    void        set_back_color(uint32_t back_color);
    void        set_cursor(int16_t x, int16_t y);
    void        set_draw_mode(uint8_t mode);
    uint16_t    bgr2rgb(u16 c);

    //port
    void draw_pixel(int16_t x, int16_t y);
    void draw_pixel(int16_t x, int16_t y, uint32_t color);
    void draw_h_line(int16_t x0, int16_t y0, int16_t x1);
    void draw_v_line(int16_t x0, int16_t y0, int16_t y1);
    void draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1);
    void draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint32_t color);
    void draw_rect(int16_t x, int16_t y, int16_t x1, int16_t y1);
    void fill_rect(int16_t x0, int16_t y0, int16_t x1, int16_t y1);
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

    void box(int16_t x, int16_t y, int16_t w, int16_t h, int16_t bc);
    void box2(int16_t x, int16_t y, int16_t w, int16_t h, u8 mode);
    void btn_down(int16_t x1, int16_t y1, int16_t x2, int16_t y2);
    void btn_up(int16_t x1, int16_t y1, int16_t x2, int16_t y2);

    //text
    //设置字体
    void set_font(const GUI_FONT *font);
    void set_text_style(uint8_t style);
    void set_text_mode(uint8_t mode);
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




    //bitmap
    void drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint32_t color);
    void drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint32_t color, uint16_t bg);
    void drawBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint32_t color) ;
    void drawBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint32_t color, uint16_t bg) ;
    void drawXBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint32_t color) ;


};

#endif
