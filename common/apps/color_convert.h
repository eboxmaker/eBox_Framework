/**
  ******************************************************************************
  * @file    color_convert.h
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

/* Define to prevent recursive inclusion -------------------------------------*/
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
    uint8_t data[100][3];
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
