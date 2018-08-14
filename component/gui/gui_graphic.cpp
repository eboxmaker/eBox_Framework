#include "gui.h"

/*********************************************************************
*
*       GUI graphic
*
**********************************************************************
*/
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
    fill_circle_helper(x0, y0, r, 3, -1);
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


void GUI::box(int16_t x, int16_t y, int16_t w, int16_t h,int16_t bc)
{
	draw_line(x,y,x+w,y,0xEF7D);
	draw_line(x+w-1,y+1,x+w-1,y+1+h,0x2965);
	draw_line(x,y+h,x+w,y+h,0x2965);
	draw_line(x,y,x,y+h,0xEF7D);
    draw_line(x+1,y+1,x+1+w-2,y+1+h-2,bc);
}
void GUI::box2(int16_t x,int16_t y,int16_t w,int16_t h, u8 mode)
{
	if (mode==0)	{
		draw_line(x,y,x+w,y,0xEF7D);
		draw_line(x+w-1,y+1,x+w-1,y+1+h,0x2965);
		draw_line(x,y+h,x+w,y+h,0x2965);
		draw_line(x,y,x,y+h,0xEF7D);
		}
	if (mode==1)	{
		draw_line(x,y,x+w,y,0x2965);
		draw_line(x+w-1,y+1,x+w-1,y+1+h,0xEF7D);
		draw_line(x,y+h,x+w,y+h,0xEF7D);
		draw_line(x,y,x,y+h,0x2965);
	}
	if (mode==2)	{
		draw_line(x,y,x+w,y,0xffff);
		draw_line(x+w-1,y+1,x+w-1,y+1+h,0xffff);
		draw_line(x,y+h,x+w,y+h,0xffff);
		draw_line(x,y,x,y+h,0xffff);
	}
}

/**************************************************************************************
功能描述: 在屏幕显示一凸起的按钮框
输    入: int16_t x1,y1,x2,y2 按钮框左上角和右下角坐标
输    出: 无
**************************************************************************************/
void GUI::btn_down(int16_t x1,int16_t y1,int16_t x2,int16_t y2)
{
	draw_line(x1,  y1,  x2,y1, GUI_GRAY2);  //H
	draw_line(x1+1,y1+1,x2,y1+1, GUI_GRAY1);  //H
	draw_line(x1,  y1,  x1,y2, GUI_GRAY2);  //V
	draw_line(x1+1,y1+1,x1+1,y2, GUI_GRAY1);  //V
	draw_line(x1,  y2,  x2,y2, GUI_WHITE);  //H
	draw_line(x2,  y1,  x2,y2, GUI_WHITE);  //V
}

/**************************************************************************************
功能描述: 在屏幕显示一凹下的按钮框
输    入: int16_t x1,y1,x2,y2 按钮框左上角和右下角坐标
输    出: 无
**************************************************************************************/
void GUI::btn_up(int16_t x1,int16_t y1,int16_t x2,int16_t y2)
{
	draw_line(x1,  y1,  x2,y1, GUI_WHITE); //H
	draw_line(x1,  y1,  x1,y2, GUI_WHITE); //V
	
	draw_line(x1+1,y2-1,x2,y2-1, GUI_GRAY1);  //H
	draw_line(x1,  y2,  x2,y2, GUI_GRAY2);  //H
	draw_line(x2-1,y1+1,x2-1,y2, GUI_GRAY1);  //V
    draw_line(x2  ,y1  ,x2,y2, GUI_GRAY2); //V
}
