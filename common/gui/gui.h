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
#define GRAY0   0xEF7D   	//灰色0 3165 00110 001011 00101
#define GRAY1   0x8410      	//灰色1      00000 000000 00000
#define GRAY2   0x4208      	//灰色2  1111111111011111
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


#define LCD_DRAWMODE_NORMAL (0)//正常
#define LCD_DRAWMODE_XOR    (1<<0)//异或
#define LCD_DRAWMODE_TRANS  (1<<1)//透明
#define LCD_DRAWMODE_REV    (1<<2)//翻转

#define LCD_TEXTMODE_NORMAL LCD_DRAWMODE_NORMAL//正常
#define LCD_TEXTMODE_XOR    LCD_DRAWMODE_XOR//异或
#define LCD_TEXTMODE_TRANS  LCD_DRAWMODE_TRANS//透明
#define LCD_TEXTMODE_REV    LCD_DRAWMODE_REV//翻转

typedef struct {
  uint8_t XSize;
  uint8_t XDist;
  uint8_t BytesPerLine;
  const unsigned char  * pData;
} GUI_CHARINFO;
typedef struct GUI_FONT_PROP {
  uint16_t First;                                /* first character               */
  uint16_t Last;                                 /* last character                */
  const GUI_CHARINFO  * paCharInfo;            /* address of first character    */
  const struct GUI_FONT_PROP  * pNext;        /* pointer to next */
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
extern const GUI_FONT GUI_Font32_ASCII;
extern const GUI_FONT GUI_FontHZ16X16;

/*************************************
 * GUI API
*************************************/
class GUI : public  LcdPort
{
    private:
        uint16_t cursor_x, cursor_y;    
        uint16_t color, back_color;
        uint8_t  draw_mode;
        uint8_t  text_style;
        uint8_t rotation;
        GUI_FONT *current_font;

    public:
        GUI(Lcd *_lcd,int16_t w, int16_t h):LcdPort(_lcd,w,h){
            draw_mode = LCD_DRAWMODE_NORMAL;
            rotation = 0;
        }; 
        void begin(){LcdPort::begin();};
        //graphic
        void draw_pixel(int16_t x, int16_t y);
        void draw_h_line(int16_t x0, int16_t y0, int16_t x1);
        void draw_v_line(int16_t x0, int16_t y0, int16_t y1);
        void draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1);
        void draw_rect(int16_t x, int16_t y, int16_t x1, int16_t y1);    
        void fill_rect(int16_t x0, int16_t y0, int16_t x1, int16_t y1);
        void fill_screen(uint16_t color) ;
        
        void draw_circle(int16_t x0, int16_t y0, int16_t r);
        void fill_circle(int16_t x0, int16_t y0, int16_t r);
        void draw_circle_helper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername);
        void fill_circle_helper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,int16_t delta);
        void draw_round_rect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius);
        void fill_round_rect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius);
        void draw_triangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2);
        void fill_triangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2);
        
        //text
        //设置字体
        void set_font(const GUI_FONT *font);
        void set_text_style(uint8_t style);    
        void set_text_mode(uint8_t mode);    
        
        //解码转换,打印字符等函数
        void char_index_of_font(uint16_t code,const GUI_FONT_PROP **font_list,uint16_t *index);
        void disp_index(const GUI_FONT_PROP *font_list,uint16_t index);
        void disp_char(uint16_t ch);
        void disp_chars(uint16_t ch,uint16_t cun);
        void disp_char_at(uint16_t ch,uint16_t x,uint16_t y);
        void disp_string(const char *str);
        void disp_string_at(const char *str,uint16_t x,uint16_t y);
     
        //settings     
        void set_color(uint32_t color);
        void set_back_color(uint32_t back_color);    
        void set_cursor(uint16_t x,uint16_t y);
        void set_draw_mode(uint8_t mode); 
        
        //get info
        int16_t height(void);
        int16_t width(void);

};

#define	________	0x0
#define	_______X	0x1
#define	______X_	0x2
#define	______XX	0x3
#define	_____X__	0x4
#define	_____X_X	0x5
#define	_____XX_	0x6
#define	_____XXX	0x7
#define	____X___	0x8
#define	____X__X	0x9
#define	____X_X_	0xa
#define	____X_XX	0xb
#define	____XX__	0xc
#define	____XX_X	0xd
#define	____XXX_	0xe
#define	____XXXX	0xf
#define	___X____	0x10
#define	___X___X	0x11
#define	___X__X_	0x12
#define	___X__XX	0x13
#define	___X_X__	0x14
#define	___X_X_X	0x15
#define	___X_XX_	0x16
#define	___X_XXX	0x17
#define	___XX___	0x18
#define	___XX__X	0x19
#define	___XX_X_	0x1a
#define	___XX_XX	0x1b
#define	___XXX__	0x1c
#define	___XXX_X	0x1d
#define	___XXXX_	0x1e
#define	___XXXXX	0x1f
#define	__X_____	0x20
#define	__X____X	0x21
#define	__X___X_	0x22
#define	__X___XX	0x23
#define	__X__X__	0x24
#define	__X__X_X	0x25
#define	__X__XX_	0x26
#define	__X__XXX	0x27
#define	__X_X___	0x28
#define	__X_X__X	0x29
#define	__X_X_X_	0x2a
#define	__X_X_XX	0x2b
#define	__X_XX__	0x2c
#define	__X_XX_X	0x2d
#define	__X_XXX_	0x2e
#define	__X_XXXX	0x2f
#define	__XX____	0x30
#define	__XX___X	0x31
#define	__XX__X_	0x32
#define	__XX__XX	0x33
#define	__XX_X__	0x34
#define	__XX_X_X	0x35
#define	__XX_XX_	0x36
#define	__XX_XXX	0x37
#define	__XXX___	0x38
#define	__XXX__X	0x39
#define	__XXX_X_	0x3a
#define	__XXX_XX	0x3b
#define	__XXXX__	0x3c
#define	__XXXX_X	0x3d
#define	__XXXXX_	0x3e
#define	__XXXXXX	0x3f
#define	_X______	0x40
#define	_X_____X	0x41
#define	_X____X_	0x42
#define	_X____XX	0x43
#define	_X___X__	0x44
#define	_X___X_X	0x45
#define	_X___XX_	0x46
#define	_X___XXX	0x47
#define	_X__X___	0x48
#define	_X__X__X	0x49
#define	_X__X_X_	0x4a
#define	_X__X_XX	0x4b
#define	_X__XX__	0x4c
#define	_X__XX_X	0x4d
#define	_X__XXX_	0x4e
#define	_X__XXXX	0x4f
#define	_X_X____	0x50
#define	_X_X___X	0x51
#define	_X_X__X_	0x52
#define	_X_X__XX	0x53
#define	_X_X_X__	0x54
#define	_X_X_X_X	0x55
#define	_X_X_XX_	0x56
#define	_X_X_XXX	0x57
#define	_X_XX___	0x58
#define	_X_XX__X	0x59
#define	_X_XX_X_	0x5a
#define	_X_XX_XX	0x5b
#define	_X_XXX__	0x5c
#define	_X_XXX_X	0x5d
#define	_X_XXXX_	0x5e
#define	_X_XXXXX	0x5f
#define	_XX_____	0x60
#define	_XX____X	0x61
#define	_XX___X_	0x62
#define	_XX___XX	0x63
#define	_XX__X__	0x64
#define	_XX__X_X	0x65
#define	_XX__XX_	0x66
#define	_XX__XXX	0x67
#define	_XX_X___	0x68
#define	_XX_X__X	0x69
#define	_XX_X_X_	0x6a
#define	_XX_X_XX	0x6b
#define	_XX_XX__	0x6c
#define	_XX_XX_X	0x6d
#define	_XX_XXX_	0x6e
#define	_XX_XXXX	0x6f
#define	_XXX____	0x70
#define	_XXX___X	0x71
#define	_XXX__X_	0x72
#define	_XXX__XX	0x73
#define	_XXX_X__	0x74
#define	_XXX_X_X	0x75
#define	_XXX_XX_	0x76
#define	_XXX_XXX	0x77
#define	_XXXX___	0x78
#define	_XXXX__X	0x79
#define	_XXXX_X_	0x7a
#define	_XXXX_XX	0x7b
#define	_XXXXX__	0x7c
#define	_XXXXX_X	0x7d
#define	_XXXXXX_	0x7e
#define	_XXXXXXX	0x7f
#define	X_______	0x80
#define	X______X	0x81
#define	X_____X_	0x82
#define	X_____XX	0x83
#define	X____X__	0x84
#define	X____X_X	0x85
#define	X____XX_	0x86
#define	X____XXX	0x87
#define	X___X___	0x88
#define	X___X__X	0x89
#define	X___X_X_	0x8a
#define	X___X_XX	0x8b
#define	X___XX__	0x8c
#define	X___XX_X	0x8d
#define	X___XXX_	0x8e
#define	X___XXXX	0x8f
#define	X__X____	0x90
#define	X__X___X	0x91
#define	X__X__X_	0x92
#define	X__X__XX	0x93
#define	X__X_X__	0x94
#define	X__X_X_X	0x95
#define	X__X_XX_	0x96
#define	X__X_XXX	0x97
#define	X__XX___	0x98
#define	X__XX__X	0x99
#define	X__XX_X_	0x9a
#define X__XX_XX	0x9b
#define X__XXX__	0x9c
#define X__XXX_X	0x9d
#define	X__XXXX_	0x9e
#define	X__XXXXX	0x9f
#define	X_X_____	0xa0
#define	X_X____X	0xa1
#define	X_X___X_	0xa2
#define	X_X___XX	0xa3
#define	X_X__X__	0xa4
#define	X_X__X_X	0xa5
#define	X_X__XX_	0xa6
#define	X_X__XXX	0xa7
#define	X_X_X___	0xa8
#define	X_X_X__X	0xa9
#define	X_X_X_X_	0xaa
#define	X_X_X_XX	0xab
#define	X_X_XX__	0xac
#define	X_X_XX_X	0xad
#define	X_X_XXX_	0xae
#define	X_X_XXXX	0xaf
#define	X_XX____	0xb0
#define X_XX___X	0xb1
#define	X_XX__X_	0xb2
#define	X_XX__XX	0xb3
#define	X_XX_X__	0xb4
#define	X_XX_X_X	0xb5
#define	X_XX_XX_	0xb6
#define	X_XX_XXX	0xb7
#define	X_XXX___	0xb8
#define	X_XXX__X	0xb9
#define	X_XXX_X_	0xba
#define	X_XXX_XX	0xbb
#define	X_XXXX__	0xbc
#define	X_XXXX_X	0xbd
#define	X_XXXXX_	0xbe
#define	X_XXXXXX	0xbf
#define	XX______	0xc0
#define	XX_____X	0xc1
#define	XX____X_	0xc2
#define	XX____XX	0xc3
#define	XX___X__	0xc4
#define	XX___X_X	0xc5
#define	XX___XX_	0xc6
#define	XX___XXX	0xc7
#define	XX__X___	0xc8
#define	XX__X__X	0xc9
#define	XX__X_X_	0xca
#define	XX__X_XX	0xcb
#define	XX__XX__	0xcc
#define	XX__XX_X	0xcd
#define	XX__XXX_	0xce
#define XX__XXXX	0xcf
#define	XX_X____	0xd0
#define	XX_X___X	0xd1
#define	XX_X__X_	0xd2
#define	XX_X__XX	0xd3
#define	XX_X_X__	0xd4
#define	XX_X_X_X	0xd5
#define	XX_X_XX_	0xd6
#define	XX_X_XXX	0xd7
#define	XX_XX___	0xd8
#define	XX_XX__X	0xd9
#define	XX_XX_X_	0xda
#define	XX_XX_XX	0xdb
#define	XX_XXX__	0xdc
#define	XX_XXX_X	0xdd
#define	XX_XXXX_	0xde
#define	XX_XXXXX	0xdf
#define	XXX_____	0xe0
#define	XXX____X	0xe1
#define	XXX___X_	0xe2
#define	XXX___XX	0xe3
#define	XXX__X__	0xe4
#define	XXX__X_X	0xe5
#define	XXX__XX_	0xe6
#define	XXX__XXX	0xe7
#define	XXX_X___	0xe8
#define	XXX_X__X	0xe9
#define	XXX_X_X_	0xea
#define	XXX_X_XX	0xeb
#define	XXX_XX__	0xec
#define	XXX_XX_X	0xed
#define	XXX_XXX_	0xee
#define	XXX_XXXX	0xef
#define	XXXX____	0xf0
#define	XXXX___X	0xf1
#define	XXXX__X_	0xf2
#define	XXXX__XX	0xf3
#define	XXXX_X__	0xf4
#define	XXXX_X_X	0xf5
#define	XXXX_XX_	0xf6
#define	XXXX_XXX	0xf7
#define	XXXXX___	0xf8
#define	XXXXX__X	0xf9
#define	XXXXX_X_	0xfa
#define	XXXXX_XX	0xfb
#define	XXXXXX__	0xfc
#define	XXXXXX_X	0xfd
#define	XXXXXXX_	0xfe
#define	XXXXXXXX	0xff
#endif
