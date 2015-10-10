#ifndef __COLORLED_H
#define __COLORLED_H
#include "ebox.h"
#include "color_convert.h"

	
class COLORLED
{
	
	public:
		void color_rgb(u8 r,u8 g,u8 b);
		void color_hsl(int h,float s,float l);
		void color_hsl(COLOR_HSL &hsl);

		void color_hsv(int h,float s,float v);
		void color_hsv(COLOR_HSV &hsv);
	void color_change_to();


		
};
#endif
