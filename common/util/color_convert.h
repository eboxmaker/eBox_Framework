#ifndef __RGB_LED_H
#define __RGB_LED_H
#include "ebox.h"

typedef enum
{
    RGB_TYPE = 1,
    GRB_TYPE  = 2,
    GBR_TYPE  = 3
} COLOR_TYPE;
typedef struct
{
    u8 data[100][3];
    COLOR_TYPE color_type;
} COLOR_BUF;

typedef struct
{
    uint8_t  r;            // [0,255]
    uint8_t  g;            // [0,255]
    uint8_t  b;            // [0,255]

} COLOR_RGB;
typedef struct
{
    uint8_t  g;            // [0,255]
    uint8_t  r;            // [0,255]
    uint8_t  b;            // [0,255]

} COLOR_GRB;

typedef struct
{
    int h;         // [0,360]
    float s;       // [0,1]
    float l;       // [0,1]
} COLOR_HSL;

typedef struct//??hsv???
{
    int 	h;		// [0,360]
    float s;		// [0,1]
    float v;		// [0,1]
} COLOR_HSV;


void RGB_to_HSL(/*[in]*/const COLOR_RGB &rgb, /*[out]*/COLOR_HSL &hsl);
void HSL_to_RGB(const COLOR_HSL &hsl, COLOR_RGB &rgb);

void RGB_to_HSV(const COLOR_RGB &rgb, COLOR_HSV &hsv );
void HSV_to_RGB(const COLOR_HSV &hsv, COLOR_RGB &rgb);

void RGB_to_GRB(COLOR_RGB &rgb, COLOR_GRB &grb);
void RGB_BUF_to_GRB(COLOR_BUF &c_buf);
void rgb_to_565(COLOR_RGB &rgb, u16 &color);
#endif
