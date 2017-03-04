
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

////// Draw a character
////void Adafruit_GFX::drawChar(int16_t x, int16_t y, unsigned char c,
//// uint16_t color, uint16_t bg, uint8_t size) {

////  if(!gfxFont) { // 'Classic' built-in font

////    if((x >= _width)            || // Clip right
////       (y >= _height)           || // Clip bottom
////       ((x + 6 * size - 1) < 0) || // Clip left
////       ((y + 8 * size - 1) < 0))   // Clip top
////      return;

////    if(!_cp437 && (c >= 176)) c++; // Handle 'classic' charset behavior

////    for(int8_t i=0; i<6; i++ ) {
////      uint8_t line;
////      if(i < 5) line = pgm_read_byte(font+(c*5)+i);
////      else      line = 0x0;
////      for(int8_t j=0; j<8; j++, line >>= 1) {
////        if(line & 0x1) {
////          if(size == 1) drawPixel(x+i, y+j, color);
////          else          fillRect(x+(i*size), y+(j*size), size, size, color);
////        } else if(bg != color) {
////          if(size == 1) drawPixel(x+i, y+j, bg);
////          else          fillRect(x+i*size, y+j*size, size, size, bg);
////        }
////      }
////    }

////  } else { // Custom font

////    // Character is assumed previously filtered by write() to eliminate
////    // newlines, returns, non-printable characters, etc.  Calling drawChar()
////    // directly with 'bad' characters of font may cause mayhem!

////    c -= pgm_read_byte(&gfxFont->first);
////    GFXglyph *glyph  = &(((GFXglyph *)pgm_read_pointer(&gfxFont->glyph))[c]);
////    uint8_t  *bitmap = (uint8_t *)pgm_read_pointer(&gfxFont->bitmap);

////    uint16_t bo = pgm_read_word(&glyph->bitmapOffset);
////    uint8_t  w  = pgm_read_byte(&glyph->width),
////             h  = pgm_read_byte(&glyph->height),
////             xa = pgm_read_byte(&glyph->xAdvance);
////    int8_t   xo = pgm_read_byte(&glyph->xOffset),
////             yo = pgm_read_byte(&glyph->yOffset);
////    uint8_t  xx, yy, bits, bit = 0;
////    int16_t  xo16, yo16;

////    if(size > 1) {
////      xo16 = xo;
////      yo16 = yo;
////    }

////    // Todo: Add character clipping here

////    // NOTE: THERE IS NO 'BACKGROUND' COLOR OPTION ON CUSTOM FONTS.
////    // THIS IS ON PURPOSE AND BY DESIGN.  The background color feature
////    // has typically been used with the 'classic' font to overwrite old
////    // screen contents with new data.  This ONLY works because the
////    // characters are a uniform size; it's not a sensible thing to do with
////    // proportionally-spaced fonts with glyphs of varying sizes (and that
////    // may overlap).  To replace previously-drawn text when using a custom
////    // font, use the getTextBounds() function to determine the smallest
////    // rectangle encompassing a string, erase the area with fillRect(),
////    // then draw new text.  This WILL infortunately 'blink' the text, but
////    // is unavoidable.  Drawing 'background' pixels will NOT fix this,
////    // only creates a new set of problems.  Have an idea to work around
////    // this (a canvas object type for MCUs that can afford the RAM and
////    // displays supporting setAddrWindow() and pushColors()), but haven't
////    // implemented this yet.

////    for(yy=0; yy<h; yy++) {
////      for(xx=0; xx<w; xx++) {
////        if(!(bit++ & 7)) {
////          bits = pgm_read_byte(&bitmap[bo++]);
////        }
////        if(bits & 0x80) {
////          if(size == 1) {
////            drawPixel(x+xo+xx, y+yo+yy, color);
////          } else {
////            fillRect(x+(xo16+xx)*size, y+(yo16+yy)*size, size, size, color);
////          }
////        }
////        bits <<= 1;
////      }
////    }

////  } // End classic vs custom font
////}






//// Return the size of the display (per current rotation)

//int16_t Adafruit_GFX::width(void) const {
//  return _width;
//}

//int16_t Adafruit_GFX::height(void) const {
//  return _height;
//}

//void Adafruit_GFX::invertDisplay(bool i) {
//  // Do nothing, must be subclassed if supported by hardware
//}
////void Gui_Circle(u16 X,u16 Y,u16 R,u16 fc) 
////{//Bresenham算法 
////    unsigned short  a,b; 
////    int c; 
////    a=0; 
////    b=R; 
////    c=3-2*R; 
////    while (a<b) 
////    { 
////        _SetPixelIndex(X+a,Y+b,fc);     //        7 
////        _SetPixelIndex(X-a,Y+b,fc);     //        6 
////        _SetPixelIndex(X+a,Y-b,fc);     //        2 
////        _SetPixelIndex(X-a,Y-b,fc);     //        3 
////        _SetPixelIndex(X+b,Y+a,fc);     //        8 
////        _SetPixelIndex(X-b,Y+a,fc);     //        5 
////        _SetPixelIndex(X+b,Y-a,fc);     //        1 
////        _SetPixelIndex(X-b,Y-a,fc);     //        4 

////        if(c<0) c=c+4*a+6; 
////        else 
////        { 
////            c=c+4*(a-b)+10; 
////            b-=1; 
////        } 
////       a+=1; 
////    } 
////    if (a==b) 
////    { 
////        _SetPixelIndex(X+a,Y+b,fc); 
////        _SetPixelIndex(X+a,Y+b,fc); 
////        _SetPixelIndex(X+a,Y-b,fc); 
////        _SetPixelIndex(X-a,Y-b,fc); 
////        _SetPixelIndex(X+b,Y+a,fc); 
////        _SetPixelIndex(X-b,Y+a,fc); 
////        _SetPixelIndex(X+b,Y-a,fc); 
////        _SetPixelIndex(X-b,Y-a,fc); 
////    } 
////	
////} 
//////画线函数，使用Bresenham 画线算法
////void Gui_DrawLine(u16 x0, u16 y0,u16 x1, u16 y1,u16 Color)   
////{
////int dx,             // difference in x's
////    dy,             // difference in y's
////    dx2,            // dx,dy * 2
////    dy2, 
////    x_inc,          // amount in pixel space to move during drawing
////    y_inc,          // amount in pixel space to move during drawing
////    error,          // the discriminant i.e. error i.e. decision variable
////    index;          // used for looping	


////	Lcd_SetXY(x0,y0);
////	dx = x1-x0;//计算x距离
////	dy = y1-y0;//计算y距离

////	if (dx>=0)
////	{
////		x_inc = 1;
////	}
////	else
////	{
////		x_inc = -1;
////		dx    = -dx;  
////	} 
////	
////	if (dy>=0)
////	{
////		y_inc = 1;
////	} 
////	else
////	{
////		y_inc = -1;
////		dy    = -dy; 
////	} 

////	dx2 = dx << 1;
////	dy2 = dy << 1;

////	if (dx > dy)//x距离大于y距离，那么每个x轴上只有一个点，每个y轴上有若干个点
////	{//且线的点数等于x距离，以x轴递增画点
////		// initialize error term
////		error = dy2 - dx; 

////		// draw the line
////		for (index=0; index <= dx; index++)//要画的点数不会超过x距离
////		{
////			//画点
////			_SetPixelIndex(x0,y0,Color);
////			
////			// test if error has overflowed
////			if (error >= 0) //是否需要增加y坐标值
////			{
////				error-=dx2;

////				// move to next line
////				y0+=y_inc;//增加y坐标值
////			} // end if error overflowed

////			// adjust the error term
////			error+=dy2;

////			// move to the next pixel
////			x0+=x_inc;//x坐标值每次画点后都递增1
////		} // end for
////	} // end if |slope| <= 1
////	else//y轴大于x轴，则每个y轴上只有一个点，x轴若干个点
////	{//以y轴为递增画点
////		// initialize error term
////		error = dx2 - dy; 

////		// draw the line
////		for (index=0; index <= dy; index++)
////		{
////			// set the pixel
////			_SetPixelIndex(x0,y0,Color);

////			// test if error overflowed
////			if (error >= 0)
////			{
////				error-=dy2;

////				// move to next line
////				x0+=x_inc;
////			} // end if error overflowed

////			// adjust the error term
////			error+=dx2;

////			// move to the next pixel
////			y0+=y_inc;
////		} // end for
////	} // end else |slope| > 1
////}



////void Gui_box(u16 x, u16 y, u16 w, u16 h,u16 bc)
////{
////	Gui_DrawLine(x,y,x+w,y,0xEF7D);
////	Gui_DrawLine(x+w-1,y+1,x+w-1,y+1+h,0x2965);
////	Gui_DrawLine(x,y+h,x+w,y+h,0x2965);
////	Gui_DrawLine(x,y,x,y+h,0xEF7D);
////    Gui_DrawLine(x+1,y+1,x+1+w-2,y+1+h-2,bc);
////}
////void Gui_box2(u16 x,u16 y,u16 w,u16 h, u8 mode)
////{
////	if (mode==0)	{
////		Gui_DrawLine(x,y,x+w,y,0xEF7D);
////		Gui_DrawLine(x+w-1,y+1,x+w-1,y+1+h,0x2965);
////		Gui_DrawLine(x,y+h,x+w,y+h,0x2965);
////		Gui_DrawLine(x,y,x,y+h,0xEF7D);
////		}
////	if (mode==1)	{
////		Gui_DrawLine(x,y,x+w,y,0x2965);
////		Gui_DrawLine(x+w-1,y+1,x+w-1,y+1+h,0xEF7D);
////		Gui_DrawLine(x,y+h,x+w,y+h,0xEF7D);
////		Gui_DrawLine(x,y,x,y+h,0x2965);
////	}
////	if (mode==2)	{
////		Gui_DrawLine(x,y,x+w,y,0xffff);
////		Gui_DrawLine(x+w-1,y+1,x+w-1,y+1+h,0xffff);
////		Gui_DrawLine(x,y+h,x+w,y+h,0xffff);
////		Gui_DrawLine(x,y,x,y+h,0xffff);
////	}
////}


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


////void Gui_DrawFont_GBK16(u16 x, u16 y, u16 fc, u16 bc, u8 *s)
////{
////	unsigned char i,j;
////	unsigned short k,x0;
////	x0=x;

////	while(*s) 
////	{	
////		if((*s) < 128) 
////		{
////			k=*s;
////			if (k==13) 
////			{
////				x=x0;
////				y+=16;
////			}
////			else 
////			{
////				if (k>32) k-=32; else k=0;
////	
////			    for(i=0;i<16;i++)
////				for(j=0;j<8;j++) 
////					{
////				    	if(asc16[k*16+i]&(0x80>>j))	_SetPixelIndex(x+j,y+i,fc);
////						else 
////						{
////							if (fc!=bc) _SetPixelIndex(x+j,y+i,bc);
////						}
////					}
////				x+=8;
////			}
////			s++;
////		}
////			
////		else 
////		{
////		

////			for (k=0;k<hz16_num;k++) 
////			{
////			  if ((hz16[k].Index[0]==*(s))&&(hz16[k].Index[1]==*(s+1)))
////			  { 
////				    for(i=0;i<16;i++)
////				    {
////						for(j=0;j<8;j++) 
////							{
////						    	if(hz16[k].Msk[i*2]&(0x80>>j))	_SetPixelIndex(x+j,y+i,fc);
////								else {
////									if (fc!=bc) _SetPixelIndex(x+j,y+i,bc);
////								}
////							}
////						for(j=0;j<8;j++) 
////							{
////						    	if(hz16[k].Msk[i*2+1]&(0x80>>j))	_SetPixelIndex(x+j+8,y+i,fc);
////								else 
////								{
////									if (fc!=bc) _SetPixelIndex(x+j+8,y+i,bc);
////								}
////							}
////				    }
////				}
////			  }
////			s+=2;x+=16;
////		} 
////		
////	}
////}

////void Gui_DrawFont_GBK24(u16 x, u16 y, u16 fc, u16 bc, u8 *s)
////{
////	unsigned char i,j;
////	unsigned short k;

////	while(*s) 
////	{
////		if( *s < 0x80 ) 
////		{
////			k=*s;
////			if (k>32) k-=32; else k=0;

////		    for(i=0;i<16;i++)
////			for(j=0;j<8;j++) 
////				{
////			    	if(asc16[k*16+i]&(0x80>>j))	
////					_SetPixelIndex(x+j,y+i,fc);
////					else 
////					{
////						if (fc!=bc) _SetPixelIndex(x+j,y+i,bc);
////					}
////				}
////			s++;x+=8;
////		}
////		else 
////		{

////			for (k=0;k<hz24_num;k++) 
////			{
////			  if ((hz24[k].Index[0]==*(s))&&(hz24[k].Index[1]==*(s+1)))
////			  { 
////				    for(i=0;i<24;i++)
////				    {
////						for(j=0;j<8;j++) 
////							{
////						    	if(hz24[k].Msk[i*3]&(0x80>>j))
////								_SetPixelIndex(x+j,y+i,fc);
////								else 
////								{
////									if (fc!=bc) _SetPixelIndex(x+j,y+i,bc);
////								}
////							}
////						for(j=0;j<8;j++) 
////							{
////						    	if(hz24[k].Msk[i*3+1]&(0x80>>j))	_SetPixelIndex(x+j+8,y+i,fc);
////								else {
////									if (fc!=bc) _SetPixelIndex(x+j+8,y+i,bc);
////								}
////							}
////						for(j=0;j<8;j++) 
////							{
////						    	if(hz24[k].Msk[i*3+2]&(0x80>>j))	
////								_SetPixelIndex(x+j+16,y+i,fc);
////								else 
////								{
////									if (fc!=bc) _SetPixelIndex(x+j+16,y+i,bc);
////								}
////							}
////				    }
////			  }
////			}
////			s+=2;x+=24;
////		}
////	}
////}
////void Gui_DrawFont_Num32(u16 x, u16 y, u16 fc, u16 bc, u16 num)
////{
////	unsigned char i,j,k,c;
////	//lcd_text_any(x+94+i*42,y+34,32,32,0x7E8,0x0,sz32,knum[i]);
//////	w=w/8;

////    for(i=0;i<32;i++)
////	{
////		for(j=0;j<4;j++) 
////		{
////			c=*(sz32+num*32*4+i*4+j);
////			for (k=0;k<8;k++)	
////			{
////	
////		    	if(c&(0x80>>k))	_SetPixelIndex(x+j*8+k,y+i,fc);
////				else {
////					if (fc!=bc) _SetPixelIndex(x+j*8+k,y+i,bc);
////				}
////			}
////		}
////	}
////}
void GUI::fill_screen(uint16_t color) 
{
    dev_fill_screen(color);
}

void GUI::set_color(uint32_t color)
{
    this->color = color;
}
void GUI::set_back_color(uint32_t back_color)
{
    this->back_color = back_color;
}


