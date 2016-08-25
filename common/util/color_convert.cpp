/**
  ******************************************************************************
  * @file    Calendar.cpp
  * @author  shentq
  * @version V1.2
  * @date    2016/08/14
  * @brief   
  ******************************************************************************
  * @attention
  *
  * No part of this software may be used for any commercial activities by any form 
  * or means, without the prior written consent of shentq. This specification is 
  * preliminary and is subject to change at any time without notice. shentq assumes
  * no responsibility for any errors contained herein.
  * <h2><center>&copy; Copyright 2015 shentq. All Rights Reserved.</center></h2>
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "color_convert.h"
#include "math.h"
#include "util.h"

void RGB_to_HSL(/*[in]*/const COLOR_RGB &rgb, /*[out]*/COLOR_HSL &hsl)
{
    float h = 0, s = 0, l = 0;
    // normalizes red-green-blue values
    float r = rgb.r / 255.f;
    float g = rgb.g / 255.f;
    float b = rgb.b / 255.f;
    float maxVal = max3v(r, g, b);
    float minVal = min3v(r, g, b);

    // hue

    if(maxVal == minVal)
    {
        h = 0; // undefined
    }

    else if(maxVal == r && g >= b)
    {
        h = 60.0f * (g - b) / (maxVal - minVal);
    }

    else if(maxVal == r && g < b)
    {
        h = 60.0f * (g - b) / (maxVal - minVal) + 360.0f;
    }

    else if(maxVal == g)
    {
        h = 60.0f * (b - r) / (maxVal - minVal) + 120.0f;
    }
    else if(maxVal == b)
    {
        h = 60.0f * (r - g) / (maxVal - minVal) + 240.0f;
    }

    // luminance

    l = (maxVal + minVal) / 2.0f;
    // saturation

    if(l == 0 || maxVal == minVal)
    {
        s = 0;
    }

    else if(0 < l && l <= 0.5f)
    {
        s = (maxVal - minVal) / (maxVal + minVal);
    }
    else if(l > 0.5f)
    {
        s = (maxVal - minVal) / (2 - (maxVal + minVal)); //(maxVal-minVal > 0)?
    }
    hsl.h = (h > 360) ? 360 : ((h < 0) ? 0 : h);
    hsl.s = ((s > 1) ? 1 : ((s < 0) ? 0 : s));
    hsl.l = ((l > 1) ? 1 : ((l < 0) ? 0 : l));
}
void HSL_to_RGB(const COLOR_HSL &hsl, COLOR_RGB &rgb)
{
    float h = hsl.h;                  // h must be [0, 360]
    float s = hsl.s; // s must be [0, 1]
    float l = hsl.l;      // l must be [0, 1]
    float R, G, B;

    if(hsl.s == 0)
    {
        // achromatic color (gray scale)
        R = G = B = l * 255.f;
    }
    else
    {
        float q = (l < 0.5f) ? (l * (1.0f + s)) : (l + s - (l * s));
        float p = (2.0f * l) - q;
        float Hk = h / 360.0f;
        float T[3];
        T[0] = Hk + 0.3333333f; // Tr   0.3333333f=1.0/3.0
        T[1] = Hk;              // Tb
        T[2] = Hk - 0.3333333f; // Tg

        for(int i = 0; i < 3; i++)
        {
            if(T[i] < 0) T[i] += 1.0f;
            if(T[i] > 1) T[i] -= 1.0f;

            if((T[i] * 6) < 1)
            {
                T[i] = p + ((q - p) * 6.0f * T[i]);
            }
            else if((T[i] * 2.0f) < 1) //(1.0/6.0)<=T[i] && T[i]<0.5
            {
                T[i] = q;
            }
            else if((T[i] * 3.0f) < 2) // 0.5<=T[i] && T[i]<(2.0/3.0)
            {
                T[i] = p + (q - p) * ((2.0f / 3.0f) - T[i]) * 6.0f;
            }
            else T[i] = p;
        }
        R = T[0] * 255.0f;
        G = T[1] * 255.0f;
        B = T[2] * 255.0f;

    }
    rgb.r = (uint8_t)((R > 255) ? 255 : ((R < 0) ? 0 : R));
    rgb.g = (uint8_t)((G > 255) ? 255 : ((G < 0) ? 0 : G));
    rgb.b = (uint8_t)((B > 255) ? 255 : ((B < 0) ? 0 : B));

}
void RGB_to_HSV(const COLOR_RGB &rgb, COLOR_HSV &hsv )
{
    uint8_t min, max;
    float delta;
    float h;

    min = min3v( rgb.r, rgb.g, rgb.b );
    max = max3v( rgb.r, rgb.g, rgb.b );
    hsv.v = max / 255.0f; // v
    delta = max - min;

    if( max != 0 )
    {
        hsv.s = (float)delta / (float)max; // s
    }
    else
    {
        // r = g = b = 0 // s = 0, v is undefined
        hsv.s = 0;
        hsv.h = -1;
        return;
    }

    if( rgb.r == max )
    {
        h = ( rgb.g - rgb.b ) / delta; // between yellow & magenta
    }
    else if( rgb.g == max )
    {
        h = 2 + ( rgb.b - rgb.r ) / delta; // between cyan & yellow
    }
    else
    {
        h = 4 + ( rgb.r - rgb.g ) / delta; // between magenta & cyan
    }

    h *= 60; // degrees

    if( h < 0 )
    {
        h += 360;
    }
    hsv.h = h;
}
//RGB(BGR: 0~255)?HSV(H: [0~360), S: [0~1], V: [0~1])
void HSV_to_RGB(const COLOR_HSV &hsv, COLOR_RGB &rgb)
{
    int 	h = hsv.h;//
    float s = hsv.s;
    float v = hsv.v;
    float r = 0.0;
    float b = 0.0;
    float g = 0.0;

    uint8_t flag = ( int) abs( h / 60.0 );
    float f = h / 60.0 - flag;
    float p = v * (1 - s);
    float q = v * (1 - f * s);
    float t = v * (1 - (1 - f) * s);

    switch ( flag)//
    {
    case 0:
        b = p;
        g = t;
        r = v;
        break;
    case 1:
        b = p;
        g = v;
        r = q;
        break;
    case 2:
        b = t;
        g = v;
        r = p;
        break;
    case 3:
        b = v;
        g = q;
        r = p;
        break;
    case 4:
        b = v;
        g = p;
        r = t;
        break;
    case 5:
        b = q;
        g = p;
        r = v;
        break;
    default:
        break;
    }

    uint16_t blue = int( b * 255);
    rgb.b = ( blue >= 255) ? 255 : blue;//
    rgb.b = ( blue <= 0) ? 0 : rgb. b;

    uint16_t green = int( g * 255);
    rgb.g = ( green >= 255) ? 255 : green;
    rgb.g = ( green <= 0) ? 0 : rgb. g;

    uint16_t red = int( r * 255);
    rgb.r = ( red >= 255) ? 255 : red;
    rgb.r = ( red <= 0) ? 0 : rgb. r;
}
void RGB_to_GRB(COLOR_RGB &rgb, COLOR_GRB &grb)
{

    grb.g = rgb.g;
    grb.r = rgb.r;
    grb.b = rgb.b;

}
void RGB_BUF_to_GRB(COLOR_BUF &c_buf)
{
    uint8_t tmp;
    if(c_buf.color_type == RGB_TYPE)
    {
        for(uint8_t i = 0; i < 100; i ++)
        {
            tmp = c_buf.data[i][0];
            c_buf.data[i][0] = c_buf.data[i][1];
            c_buf.data[i][1] = tmp;
        }
    }
}
void rgb_to_565(COLOR_RGB &rgb, uint16_t &color)
{
    color |= (rgb.r >> 3) << 11;
    color |= (rgb.g >> 2) << 5;
    color |= (rgb.b >> 3) << 0;
}
