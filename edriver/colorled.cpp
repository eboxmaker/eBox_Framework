#include "colorled.h"


void COLORLED::begin()
{
    r = new PWM(r_pin);
    g = new PWM(g_pin);
    b = new PWM(b_pin);
    r->begin(1000, 0);
    g->begin(1000, 0);
    b->begin(1000, 0);
    r->set_oc_polarity(0);
    g->set_oc_polarity(0);
    b->set_oc_polarity(0);
}

void COLORLED::color_rgb(u8 r, u8 g, u8 b)
{

    this->r->set_duty(r * 3);
    this->g->set_duty(g * 3);
    this->b->set_duty(b * 3);

}
void COLORLED::color_hsl(int h, float s, float l)
{
    COLOR_HSL hsl;
    COLOR_RGB rgb;
    hsl.h = h;
    hsl.s = s;
    hsl.l = l;

    HSL_to_RGB(hsl, rgb);

    this->r->set_duty(rgb.r * 1.9);
    this->g->set_duty(rgb.g * 3.9);
    this->b->set_duty(rgb.b * 0.6);
}
void COLORLED::color_hsl(COLOR_HSL &hsl)
{
    COLOR_RGB rgb;

    HSL_to_RGB(hsl, rgb);

    this->r->set_duty(rgb.r * 1.9);
    this->g->set_duty(rgb.g * 3.9);
    this->b->set_duty(rgb.b * 0.6);

}
void COLORLED::color_hsv(int h, float s, float v)
{
    COLOR_HSV hsv;
    COLOR_RGB rgb;
    hsv.h = h;
    hsv.s = s;
    hsv.v = v;

    HSV_to_RGB(hsv, rgb);
    this->r->set_duty(rgb.r * 1.9);
    this->g->set_duty(rgb.g * 3.9);
    this->b->set_duty(rgb.b * 0.6);
}
void COLORLED::color_hsv(COLOR_HSV &hsv)
{
    COLOR_RGB rgb;

    HSV_to_RGB(hsv, rgb);

    this->r->set_duty(rgb.r * 1.9);
    this->g->set_duty(rgb.g * 3.9);
    this->b->set_duty(rgb.b * 0.6);
}
