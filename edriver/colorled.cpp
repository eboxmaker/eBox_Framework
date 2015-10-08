#include"colorled.h"
	PWM led_r(&PA1,1000);
	PWM led_g(&PA0,1000);
	PWM led_b(&PA2,1000);


void COLORLED::color_rgb(u8 r,u8 g,u8 b)
{
	
		led_r.set_duty(r*3);
		led_g.set_duty(g*3);
		led_b.set_duty(b*3);

}
void COLORLED::color_hsl(int h,float s,float l)
{
	COLOR_HSL hsl;
	COLOR_RGB rgb;
	hsl.h = h;
	hsl.s = s;
	hsl.l = l;
	
	HSL_to_RGB(hsl,rgb);
	
	led_r.set_duty(rgb.r*1.9);
	led_g.set_duty(rgb.g*3.9);
	led_b.set_duty(rgb.b*0.6);
}
void COLORLED::color_hsl(COLOR_HSL &hsl)
{
	COLOR_RGB rgb;
	
	HSL_to_RGB(hsl,rgb);
	
	led_r.set_duty(rgb.r*1.9);
	led_g.set_duty(rgb.g*3.9);
	led_b.set_duty(rgb.b*0.6);

}
void COLORLED::color_hsv(int h,float s,float v)
{
	COLOR_HSV hsv;
	COLOR_RGB rgb;
	hsv.h = h;
	hsv.s = s;
	hsv.v = v;
		
	HSV_to_RGB(hsv,rgb);
	led_r.set_duty(rgb.r*1.9);
	led_g.set_duty(rgb.g*3.9);
	led_b.set_duty(rgb.b*0.6);
}
void COLORLED::color_hsv(COLOR_HSV &hsv)
{
	COLOR_RGB rgb;
	
	HSV_to_RGB(hsv,rgb);
	
	led_r.set_duty(rgb.r*1.9);
	led_g.set_duty(rgb.g*3.9);
	led_b.set_duty(rgb.b*0.6);
}
