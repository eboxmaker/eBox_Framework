
#include "gui.h"
//从ILI93xx读出的数据为GBR格式，而我们写入的时候为RGB格式。
//通过该函数转换
//c:GBR格式的颜色值
//返回值：RGB格式的颜色值
//u16 LCD_BGR2RGB(u16 c)
//{
//  u16  r,g,b,rgb;   
//  b=(c>>0)&0x1f;
//  g=(c>>5)&0x3f;
//  r=(c>>11)&0x1f;	 
//  rgb=(b<<11)+(g<<5)+(r<<0);		 
//  return(rgb);

//}

//#if !defined(__INT_MAX__) || (__INT_MAX__ > 0xFFFF)
// #define pgm_read_pointer(addr) ((void *)pgm_read_dword(addr))
//#else
// #define pgm_read_pointer(addr) ((void *)pgm_read_word(addr))
//#endif

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef _swap_int16_t
#define _swap_int16_t(a, b) { int16_t t = a; a = b; b = t; }
#endif

/*********************************************************************
*
*       GUI graphic
*
**********************************************************************
*/

void GUI::draw_pixel(int16_t x, int16_t y)
{
    dev_draw_pixel(x,y,this->color);
}
void GUI::draw_h_line(int16_t x0, int16_t y0, int16_t x1)
{
    dev_draw_h_line(x0,y0,x1,this->color);
}
void GUI::draw_v_line(int16_t x0, int16_t y0, int16_t y1)
{
    dev_draw_v_line(x0,y0,y1,this->color);
}
void GUI::draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1)
{
    dev_draw_line(x0,y0,x1,y1,this->color);
}
void GUI::draw_rect(int16_t x0, int16_t y0, int16_t x1, int16_t y1)
{
    dev_draw_rect(x0,y0,x1,y1,this->color);
}    
void GUI::fill_rect(int16_t x0, int16_t y0, int16_t x1, int16_t y1)
{
    dev_fill_fect(x0,y0,x1,y1,this->color);
}
void GUI::fill_screen(uint16_t color) 
{
    dev_fill_screen(color);
}

void GUI::draw_circle(int16_t x0, int16_t y0, int16_t r)
{

    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    draw_pixel(x0  , y0+r);
    draw_pixel(x0  , y0-r);
    draw_pixel(x0+r, y0  );
    draw_pixel(x0-r, y0  );

    while (x<y) 
    {
        if (f >= 0) 
        {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        draw_pixel(x0 + x, y0 + y);
        draw_pixel(x0 - x, y0 + y);
        draw_pixel(x0 + x, y0 - y);
        draw_pixel(x0 - x, y0 - y);
        draw_pixel(x0 + y, y0 + x);
        draw_pixel(x0 - y, y0 + x);
        draw_pixel(x0 + y, y0 - x);
        draw_pixel(x0 - y, y0 - x);
    }
}
void GUI::draw_circle_helper( int16_t x0, int16_t y0,
 int16_t r, uint8_t cornername) {
  int16_t f     = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x     = 0;
  int16_t y     = r;

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f     += ddF_y;
    }
    x++;
    ddF_x += 2;
    f     += ddF_x;
    if (cornername & 0x4) {
      draw_pixel(x0 + x, y0 + y);
      draw_pixel(x0 + y, y0 + x);
    }
    if (cornername & 0x2) {
      draw_pixel(x0 + x, y0 - y);
      draw_pixel(x0 + y, y0 - x);
    }
    if (cornername & 0x8) {
      draw_pixel(x0 - y, y0 + x);
      draw_pixel(x0 - x, y0 + y);
    }
    if (cornername & 0x1) {
      draw_pixel(x0 - y, y0 - x);
      draw_pixel(x0 - x, y0 - y);
    }
  }
}
void GUI::fill_circle(int16_t x0, int16_t y0, int16_t r)
{
    draw_v_line(x0, y0-r, y0 + r+1);
    fill_circle_helper(x0, y0, r, 3, 0);
}
// Used to do circles and roundrects
void GUI::fill_circle_helper(int16_t x0, int16_t y0, int16_t r,
 uint8_t cornername, int16_t delta) {

  int16_t f     = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x     = 0;
  int16_t y     = r;

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f     += ddF_y;
    }
    x++;
    ddF_x += 2;
    f     += ddF_x;

    if (cornername & 0x1) {
      draw_v_line(x0+x, y0-y, y0 + y+1+delta);
      draw_v_line(x0+y, y0-x, y0 + x+1+delta);
    }
    if (cornername & 0x2) {
      draw_v_line(x0-x, y0-y, y0 + y+1+delta);
      draw_v_line(x0-y, y0-x, y0 + x+1+delta);
    }
  }
}





// Draw a rounded rectangle
void GUI::draw_round_rect(int16_t x, int16_t y, int16_t w,
 int16_t h, int16_t r) {
  // smarter version
  draw_h_line(x+r  , y    , x + w-r + 1); // Top
  draw_h_line(x+r  , y+h-1, x + w-r + 1); // Bottom
  draw_v_line(x    , y+r  , y + h-r + 1); // Left
  draw_v_line(x+w-1, y+r  , y + h-r + 1); // Right
  // draw four corners
  draw_circle_helper(x+r    , y+r    , r, 1);
  draw_circle_helper(x+w-r-1, y+r    , r, 2);
  draw_circle_helper(x+w-r-1, y+h-r-1, r, 4);
  draw_circle_helper(x+r    , y+h-r-1, r, 8);
}

// Fill a rounded rectangle
void GUI::fill_round_rect(int16_t x, int16_t y, int16_t w,
 int16_t h, int16_t r) {
  // smarter version
  fill_rect(x+r, y, x + w-r + 1, y + h);

  // draw four corners
  fill_circle_helper(x+w-r-1, y+r, r, 1, h-2*r-1);
  fill_circle_helper(x+r    , y+r, r, 2, h-2*r-1);
}

// Draw a triangle
void GUI::draw_triangle(int16_t x0, int16_t y0,
 int16_t x1, int16_t y1, int16_t x2, int16_t y2) {
  draw_line(x0, y0, x1, y1);
  draw_line(x1, y1, x2, y2);
  draw_line(x2, y2, x0, y0);
}

//// Fill a triangle
void GUI::fill_triangle(int16_t x0, int16_t y0,
 int16_t x1, int16_t y1, int16_t x2, int16_t y2) {

  int16_t a, b, y, last;

  // Sort coordinates by Y order (y2 >= y1 >= y0)
  if (y0 > y1) {
    _swap_int16_t(y0, y1); _swap_int16_t(x0, x1);
  }
  if (y1 > y2) {
    _swap_int16_t(y2, y1); _swap_int16_t(x2, x1);
  }
  if (y0 > y1) {
    _swap_int16_t(y0, y1); _swap_int16_t(x0, x1);
  }

  if(y0 == y2) { // Handle awkward all-on-same-line case as its own thing
    a = b = x0;
    if(x1 < a)      a = x1;
    else if(x1 > b) b = x1;
    if(x2 < a)      a = x2;
    else if(x2 > b) b = x2;
    draw_h_line(a, y0, b+1);
    return;
  }

  int16_t
    dx01 = x1 - x0,
    dy01 = y1 - y0,
    dx02 = x2 - x0,
    dy02 = y2 - y0,
    dx12 = x2 - x1,
    dy12 = y2 - y1;
  int32_t
    sa   = 0,
    sb   = 0;

  // For upper part of triangle, find scanline crossings for segments
  // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
  // is included here (and second loop will be skipped, avoiding a /0
  // error there), otherwise scanline y1 is skipped here and handled
  // in the second loop...which also avoids a /0 error here if y0=y1
  // (flat-topped triangle).
  if(y1 == y2) last = y1;   // Include y1 scanline
  else         last = y1-1; // Skip it

  for(y=y0; y<=last; y++) {
    a   = x0 + sa / dy01;
    b   = x0 + sb / dy02;
    sa += dx01;
    sb += dx02;
    /* longhand:
    a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
    b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
    */
    if(a > b) _swap_int16_t(a,b);
    draw_h_line(a, y, b+1);
  }

  // For lower part of triangle, find scanline crossings for segments
  // 0-2 and 1-2.  This loop is skipped if y1=y2.
  sa = dx12 * (y - y1);
  sb = dx02 * (y - y0);
  for(; y<=y2; y++) {
    a   = x1 + sa / dy12;
    b   = x0 + sb / dy02;
    sa += dx12;
    sb += dx02;
    /* longhand:
    a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
    b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
    */
    if(a > b) _swap_int16_t(a,b);
    draw_h_line(a, y, b+1);
  }
}

//// Draw a 1-bit image (bitmap) at the specified (x,y) position from the
//// provided bitmap buffer (must be PROGMEM memory) using the specified
//// foreground color (unset bits are transparent).
//void Adafruit_GFX::drawBitmap(int16_t x, int16_t y,
// const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color) {

//  int16_t i, j, byteWidth = (w + 7) / 8;
//  uint8_t byte;

////  for(j=0; j<h; j++) {
////    for(i=0; i<w; i++) {
////      if(i & 7) byte <<= 1;
////      else      byte   = pgm_read_byte(bitmap + j * byteWidth + i / 8);
////      if(byte & 0x80) drawPixel(x+i, y+j, color);
////    }
////  }
//}

//// Draw a 1-bit image (bitmap) at the specified (x,y) position from the
//// provided bitmap buffer (must be PROGMEM memory) using the specified
//// foreground (for set bits) and background (for clear bits) colors.
//void Adafruit_GFX::drawBitmap(int16_t x, int16_t y,
// const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color, uint16_t bg) {

//  int16_t i, j, byteWidth = (w + 7) / 8;
//  uint8_t byte;

////  for(j=0; j<h; j++) {
////    for(i=0; i<w; i++ ) {
////      if(i & 7) byte <<= 1;
////      else      byte   = pgm_read_byte(bitmap + j * byteWidth + i / 8);
////      if(byte & 0x80) drawPixel(x+i, y+j, color);
////      else            drawPixel(x+i, y+j, bg);
////    }
////  }
//}

//// drawBitmap() variant for RAM-resident (not PROGMEM) bitmaps.
//void Adafruit_GFX::drawBitmap(int16_t x, int16_t y,
// uint8_t *bitmap, int16_t w, int16_t h, uint16_t color) {

//  int16_t i, j, byteWidth = (w + 7) / 8;
//  uint8_t byte;

//  for(j=0; j<h; j++) {
//    for(i=0; i<w; i++ ) {
//      if(i & 7) byte <<= 1;
//      else      byte   = bitmap[j * byteWidth + i / 8];
//      if(byte & 0x80) drawPixel(x+i, y+j, color);
//    }
//  }
//}

//// drawBitmap() variant w/background for RAM-resident (not PROGMEM) bitmaps.
//void Adafruit_GFX::drawBitmap(int16_t x, int16_t y,
// uint8_t *bitmap, int16_t w, int16_t h, uint16_t color, uint16_t bg) {

//  int16_t i, j, byteWidth = (w + 7) / 8;
//  uint8_t byte;

//  for(j=0; j<h; j++) {
//    for(i=0; i<w; i++ ) {
//      if(i & 7) byte <<= 1;
//      else      byte   = bitmap[j * byteWidth + i / 8];
//      if(byte & 0x80) drawPixel(x+i, y+j, color);
//      else            drawPixel(x+i, y+j, bg);
//    }
//  }
//}

////Draw XBitMap Files (*.xbm), exported from GIMP,
////Usage: Export from GIMP to *.xbm, rename *.xbm to *.c and open in editor.
////C Array can be directly used with this function
//void Adafruit_GFX::drawXBitmap(int16_t x, int16_t y,
// const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color) {

//  int16_t i, j, byteWidth = (w + 7) / 8;
//  uint8_t byte;

////  for(j=0; j<h; j++) {
////    for(i=0; i<w; i++ ) {
////      if(i & 7) byte >>= 1;
////      else      byte   = pgm_read_byte(bitmap + j * byteWidth + i / 8);
////      if(byte & 0x01) drawPixel(x+i, y+j, color);
////    }
////  }
//}


//void Gui_box(u16 x, u16 y, u16 w, u16 h,u16 bc)
//{
//	Gui_DrawLine(x,y,x+w,y,0xEF7D);
//	Gui_DrawLine(x+w-1,y+1,x+w-1,y+1+h,0x2965);
//	Gui_DrawLine(x,y+h,x+w,y+h,0x2965);
//	Gui_DrawLine(x,y,x,y+h,0xEF7D);
//    Gui_DrawLine(x+1,y+1,x+1+w-2,y+1+h-2,bc);
//}
//void Gui_box2(u16 x,u16 y,u16 w,u16 h, u8 mode)
//{
//	if (mode==0)	{
//		Gui_DrawLine(x,y,x+w,y,0xEF7D);
//		Gui_DrawLine(x+w-1,y+1,x+w-1,y+1+h,0x2965);
//		Gui_DrawLine(x,y+h,x+w,y+h,0x2965);
//		Gui_DrawLine(x,y,x,y+h,0xEF7D);
//		}
//	if (mode==1)	{
//		Gui_DrawLine(x,y,x+w,y,0x2965);
//		Gui_DrawLine(x+w-1,y+1,x+w-1,y+1+h,0xEF7D);
//		Gui_DrawLine(x,y+h,x+w,y+h,0xEF7D);
//		Gui_DrawLine(x,y,x,y+h,0x2965);
//	}
//	if (mode==2)	{
//		Gui_DrawLine(x,y,x+w,y,0xffff);
//		Gui_DrawLine(x+w-1,y+1,x+w-1,y+1+h,0xffff);
//		Gui_DrawLine(x,y+h,x+w,y+h,0xffff);
//		Gui_DrawLine(x,y,x,y+h,0xffff);
//	}
//}


/////**************************************************************************************
////功能描述: 在屏幕显示一凸起的按钮框
////输    入: u16 x1,y1,x2,y2 按钮框左上角和右下角坐标
////输    出: 无
////**************************************************************************************/
////void DisplayButtonDown(u16 x1,u16 y1,u16 x2,u16 y2)
////{
////	Gui_DrawLine(x1,  y1,  x2,y1, GRAY2);  //H
////	Gui_DrawLine(x1+1,y1+1,x2,y1+1, GRAY1);  //H
////	Gui_DrawLine(x1,  y1,  x1,y2, GRAY2);  //V
////	Gui_DrawLine(x1+1,y1+1,x1+1,y2, GRAY1);  //V
////	Gui_DrawLine(x1,  y2,  x2,y2, WHITE);  //H
////	Gui_DrawLine(x2,  y1,  x2,y2, WHITE);  //V
////}

/////**************************************************************************************
////功能描述: 在屏幕显示一凹下的按钮框
////输    入: u16 x1,y1,x2,y2 按钮框左上角和右下角坐标
////输    出: 无
////**************************************************************************************/
////void DisplayButtonUp(u16 x1,u16 y1,u16 x2,u16 y2)
////{
////	Gui_DrawLine(x1,  y1,  x2,y1, WHITE); //H
////	Gui_DrawLine(x1,  y1,  x1,y2, WHITE); //V
////	
////	Gui_DrawLine(x1+1,y2-1,x2,y2-1, GRAY1);  //H
////	Gui_DrawLine(x1,  y2,  x2,y2, GRAY2);  //H
////	Gui_DrawLine(x2-1,y1+1,x2-1,y2, GRAY1);  //V
////    Gui_DrawLine(x2  ,y1  ,x2,y2, GRAY2); //V
////}
/*********************************************************************
*
*       GUI text
*
**********************************************************************
*/
void GUI::set_font(const GUI_FONT *font)
{
    this->current_font = (GUI_FONT *)font;

}
void GUI::set_text_style(uint8_t style)
{
    this->text_style = style;
}
void GUI::set_text_mode(uint8_t mode)
{   
    draw_mode = mode;
}
////解码//////////////////////////////
void GUI::char_index_of_font(uint16_t code,const GUI_FONT_PROP **font_list,uint16_t *index)
{
    uint16_t tmp;
    uint16_t count;
    const GUI_FONT_PROP *pList;
    pList = current_font->list;
    
    while(pList != NULL)
    {
        if(code >= pList->First && code <= pList->Last)
        {
            *font_list = pList;
            *index = code - pList->First;
            break;
        }
        else
        {
            *font_list = current_font->list;
            *index = 0; 
            pList=pList->pNext;
        }
    }


}

void GUI::disp_index(const GUI_FONT_PROP *font_list,uint16_t index)
{
	uint32_t count, row ,col, mask;
	uint8_t tmp;
    const GUI_CHARINFO *pCharInfo;
    uint8_t byte_per_line;
    if((font_list == NULL) || (index > (font_list->Last - font_list->First + 1)))return;
    pCharInfo = &font_list->paCharInfo[index];
    byte_per_line = pCharInfo->BytesPerLine;
    #if TEXT_AUTORELINE 
        if(cursor_x + pCharInfo->XSize > _width)cursor_x = 0,cursor_y+=current_font->YSize;
    #endif
	for(row = 0; row < current_font->YSize; row++){   
        for( count = 0; count < byte_per_line; count++){
            tmp = pCharInfo->pData[byte_per_line * row + count];
            for(mask = 0x80, col = 0; col < 8 ; mask >>= 1, col++){	
                switch(draw_mode)
                {
                    case LCD_DRAWMODE_NORMAL:
                        if(mask & tmp)
                            draw_pixel(cursor_x,cursor_y);
                        else
                            dev_draw_pixel(cursor_x,cursor_y,back_color);
                        break;
                    case LCD_DRAWMODE_XOR:
                        if(mask & tmp)
                            draw_pixel(cursor_x,cursor_y);
                        break;
                    case LCD_DRAWMODE_TRANS:
                        if(mask & tmp)
                            draw_pixel(cursor_x,cursor_y);
                        break;
                    case LCD_DRAWMODE_REV:
                        if(mask & tmp)
                            dev_draw_pixel(cursor_x,cursor_y,back_color);
                        else
                            draw_pixel(cursor_x,cursor_y);
                        break;

                }
                cursor_x++;
            }
        }
            cursor_x-=byte_per_line * 8;
            cursor_y++;
    }
    cursor_y-=current_font->YDist;
    cursor_x+=pCharInfo->XSize;
}

void GUI::disp_char(uint16_t ch)
{
    const GUI_FONT_PROP *font_list;
    uint16_t index;
    if(ch == '\n')
    {
        set_cursor(0,cursor_y+current_font->YSize);
        return;
    }
    if(ch == '\r')
    {
        set_cursor(0,cursor_y);
        return;
    }
    char_index_of_font(ch,&font_list,&index);
    disp_index(font_list,index);
}
void GUI::disp_char_at(uint16_t ch,uint16_t x,uint16_t y)
{
    set_cursor(x,y);
    disp_char(ch);
}
void GUI::disp_chars(uint16_t ch,uint16_t count)
{
    while(count--)
        disp_char(ch);
}
void GUI::disp_string(const char *str)
{
    uint16_t ch = 0;
    while(*str)
    {
        if(*str < 0x7e)//是字母
            disp_char(*str++);
        else//汉字
        {            
            ch = (*str++)<<8;
            ch += *str++;
            disp_char(ch);        
        }
    }
}
void GUI::disp_string_at(const char *str,uint16_t x,uint16_t y)
{
    set_cursor(x,y);
    disp_string(str);
}

void GUI::printf(const char *fmt, ...)
{
    char buf[30];
    uint8_t i = 0;
    va_list va_params;
    va_start(va_params, fmt);
    vsprintf(buf, fmt, va_params);
    va_end(va_params);
    while(buf[i] != '\0')
    {
        if(buf[i] < 0x7e)//是字母
            disp_char(buf[i++]);
        else//汉字
        {        
            uint16_t tmp;
            tmp = buf[i++];
            tmp = (tmp<<8) + buf[i++];
            disp_char(tmp);        
        }
    }
 
}
    
void GUI::printf(u16 x, u16 y, const char *fmt, ...)
{
    char buf[30];
    uint8_t i = 0;
    va_list va_params;
    va_start(va_params, fmt);
    vsprintf(buf, fmt, va_params);
    va_end(va_params);
    set_cursor(x,y);
    while(buf[i] != '\0')
    {
        if(buf[i] < 0x7e)//是字母
            disp_char(buf[i++]);
        else//汉字
        {        
            uint16_t tmp;
            tmp = buf[i++];
            tmp = (tmp<<8) + buf[i++];
            disp_char(tmp);        
        }
    }
}

/*********************************************************************
*
*       GUI settings
*
**********************************************************************
*/



void GUI::set_color(uint32_t color)
{
    this->color = color;
}
void GUI::set_back_color(uint32_t back_color)
{
    this->back_color = back_color;
}

void GUI::set_cursor(uint16_t x,uint16_t y)
{
    this->cursor_x = x;
    this->cursor_y = y;
}
void GUI::set_draw_mode(uint8_t mode)
{   
    draw_mode = mode;
}


int16_t GUI::height()
{
    return _height;
}
int16_t GUI::width()
{
    return _width;
}

