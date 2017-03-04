#ifndef __GUI_H
#define __GUI_H
#include "ebox.h"
#include "lcd_port.h"
#define RED  	0xf800
#define GREEN	0x07e0
#define BLUE 	0x001f
#define WHITE	0xffff
#define BLACK	0x0000
#define YELLOW  0xFFE0
#define GRAY0   0xEF7D   	//»ÒÉ«0 3165 00110 001011 00101
#define GRAY1   0x8410      	//»ÒÉ«1      00000 000000 00000
#define GRAY2   0x4208      	//»ÒÉ«2  1111111111011111
//u16 LCD_BGR2RGB(u16 c);
//void Gui_Circle(u16 X,u16 Y,u16 R,u16 fc); 
//void Gui_DrawLine(u16 x0, u16 y0,u16 x1, u16 y1,u16 Color);  
//void Gui_box(u16 x, u16 y, u16 w, u16 h,u16 bc);
//void Gui_box2(u16 x,u16 y,u16 w,u16 h, u8 mode);
//void DisplayButtonDown(u16 x1,u16 y1,u16 x2,u16 y2);
//void DisplayButtonUp(u16 x1,u16 y1,u16 x2,u16 y2);
//void Gui_DrawFont_GBK16(u16 x, u16 y, u16 fc, u16 bc, u8 *s);
//void Gui_DrawFont_GBK24(u16 x, u16 y, u16 fc, u16 bc, u8 *s);
//void Gui_DrawFont_Num32(u16 x, u16 y, u16 fc, u16 bc, u16 num) ;

#define GUI_TS_NORMAL           (0)
#define GUI_TS_UNDERLINE        (1 << 0)
#define GUI_TS_STRIKETHRU       (1 << 1)
#define GUI_TS_OVERLINE         (1 << 2)


#define LCD_DRAWMODE_NORMAL (0)
#define LCD_DRAWMODE_XOR    (1<<0)
#define LCD_DRAWMODE_TRANS  (1<<1)
#define LCD_DRAWMODE_REV    (1<<2)

class GUI : public  LcdPort
{
    private:
        uint16_t _width, _height; // Display w/h as modified by current rotation 
        uint16_t cursor_x, cursor_y;    
        uint16_t color, back_color;
        uint8_t  draw_mode;
        uint8_t  text_style;

    public:
        GUI(Lcd *_lcd,int16_t w, int16_t h):LcdPort(_lcd){}; // Constructor
        void draw_pixel(int16_t x, int16_t y);
        void draw_h_line(int16_t x0, int16_t y0, int16_t x1);
        void draw_v_line(int16_t x0, int16_t y0, int16_t y1);
        void draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1);
        void draw_rect(int16_t x, int16_t y, int16_t x1, int16_t y1);    
        void fill_rect(int16_t x0, int16_t y0, int16_t x1, int16_t y1);
        void draw_circle(int16_t x0, int16_t y0, int16_t r);
        void fill_circle(int16_t x0, int16_t y0, int16_t r);
        void draw_circle_helper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername);
        void fill_circle_helper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,int16_t delta);
        void draw_round_rect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius);
        void fill_round_rect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius);
        void draw_triangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2);
        void fill_triangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2);
            
        void disp_char(char c);
        void disp_chars(char c,uint16_t cun);
        void disp_char_at(char c,uint16_t x,uint16_t y);
        void disp_string(const char *str);
        void disp_string_at(const char *str,uint16_t x,uint16_t y);
     
            
        void set_color(uint32_t color);
        void set_back_color(uint32_t back_color);    
        void set_cursor(uint16_t x,uint16_t y);
        void fill_screen(uint16_t color) ;
        void set_draw_mode(uint8_t mode);    
     
};
//    virtual void    invertDisplay(bool i) ;
  void

    drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap,
      int16_t w, int16_t h, uint16_t color),
    drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap,
      int16_t w, int16_t h, uint16_t color, uint16_t bg),
    drawBitmap(int16_t x, int16_t y, uint8_t *bitmap,
      int16_t w, int16_t h, uint16_t color),
    drawBitmap(int16_t x, int16_t y, uint8_t *bitmap,
      int16_t w, int16_t h, uint16_t color, uint16_t bg),
    drawXBitmap(int16_t x, int16_t y, const uint8_t *bitmap,
      int16_t w, int16_t h, uint16_t color),
    drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color,
      uint16_t bg, uint8_t size),
    setCursor(int16_t x, int16_t y),
    setTextColor(uint16_t c),
    setTextColor(uint16_t c, uint16_t bg),
    setTextSize(uint8_t s),
    setTextWrap(bool w),
    setRotation(uint8_t r),
    cp437(bool x=true),
    //setFont(const GFXfont *f = NULL),
    getTextBounds(char *string, int16_t x, int16_t y,
      int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h),
    getTextBounds(const __FlashStringHelper *s, int16_t x, int16_t y,
      int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h);

//  int16_t height(void) const;
//  int16_t width(void) const;

//  uint8_t getRotation(void) const;

//  // get current cursor position (get rotation safe maximum values, using: width() for x, height() for y)
//  int16_t getCursorX(void) const;
//  int16_t getCursorY(void) const;
//  protected:
//  const int16_t
//    WIDTH, HEIGHT;   // This is the 'raw' display w/h - never changes
//  int16_t
//    _width, _height, // Display w/h as modified by current rotation
//    cursor_x, cursor_y;
//  uint16_t
//    textcolor, textbgcolor;
//  uint8_t
//    textsize,
//    rotation;
//  bool
//    wrap,   // If set, 'wrap' text at right edge of display
//    _cp437; // If set, use correct CP437 charset (default is off)
//  GFXfont
//    *gfxFont;

//};
#endif
