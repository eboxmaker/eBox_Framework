#include "ebox.h"
#include "template.h"
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
class Adafruit_GFX //: public Print 
{
 public:

  Adafruit_GFX(int16_t w, int16_t h); // Constructor
 
    // This MUST be defined by the subclass:
    virtual void drawPixel(int16_t x, int16_t y, uint16_t color) = 0;

    // These MAY be overridden by the subclass to provide device-specific
    // optimized code.  Otherwise 'generic' versions are used.
    virtual void    drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) ;
    virtual void    drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
    virtual void    drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) ;
    virtual void    drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) ;
    virtual void    fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) ;
    virtual void    fillScreen(uint16_t color) ;
    virtual void    invertDisplay(bool i) ;
  void
    drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color),
    drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,
      uint16_t color),
    fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color),
    fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,
      int16_t delta, uint16_t color),
    drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
      int16_t x2, int16_t y2, uint16_t color),
    fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
      int16_t x2, int16_t y2, uint16_t color),
    drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h,
      int16_t radius, uint16_t color),
    fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h,
      int16_t radius, uint16_t color),
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

  int16_t height(void) const;
  int16_t width(void) const;

  uint8_t getRotation(void) const;

  // get current cursor position (get rotation safe maximum values, using: width() for x, height() for y)
  int16_t getCursorX(void) const;
  int16_t getCursorY(void) const;
  protected:
  const int16_t
    WIDTH, HEIGHT;   // This is the 'raw' display w/h - never changes
  int16_t
    _width, _height, // Display w/h as modified by current rotation
    cursor_x, cursor_y;
  uint16_t
    textcolor, textbgcolor;
  uint8_t
    textsize,
    rotation;
  bool
    wrap,   // If set, 'wrap' text at right edge of display
    _cp437; // If set, use correct CP437 charset (default is off)
//  GFXfont
//    *gfxFont;

};
