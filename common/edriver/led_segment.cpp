/**
  ******************************************************************************
  * @file    LedSegment.cpp
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
#include "led_segment.h"

uint8_t led_table_a[] = { 0x7d, 0x44, 0x7a, 0x6e, 0x47, 0x2f, 0x3f, 0x64, 0x7f, 0x6f, 0x00}; //共阴 时钟系统母钟专用

//uint8_t led_table_a[]   ={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e,0xff}; //共阳极
//uint8_t led_table_k[]={ 0x3f,0x06,0x5b,0x4f, 0x66,0x6d,0x7d,0x07, 0x7f,0x6f,0x77,0x7c, 0x39,0x5e,0x79,0x71}; //共阴
void LedSegment::begin()
{
    _74hc595::begin();
}






void LedSegment::show_time_HMS(uint8_t hour, uint8_t min, uint8_t sec)
{
    int i = 0;
    //for( int i = 0; i < 8; i++)
    led_buf[i++] = led_table_a[10];
    led_buf[i++] = led_table_a[10];
    led_buf[i++] = led_table_a[sec % 10];
    led_buf[i++] = led_table_a[sec / 10];
    led_buf[i++] = led_table_a[min % 10];
    led_buf[i++] = led_table_a[min / 10];
    led_buf[i++] = led_table_a[hour % 10];
    led_buf[i++] = led_table_a[hour / 10];

    write(led_buf, 8, MSB_FIRST);
    update();
}
void LedSegment::show_date_time(uint16_t year, uint8_t month, uint8_t date)
{
    int i = 0;
    //for( int i = 0; i < 8; i++)
    led_buf[i++] = led_table_a[date % 10];
    led_buf[i++] = led_table_a[date / 10];
    led_buf[i++] = led_table_a[month % 10];
    led_buf[i++] = led_table_a[month / 10];
    led_buf[i++] = led_table_a[year % 10];
    led_buf[i++] = led_table_a[year / 10 % 10];
    led_buf[i++] = led_table_a[year / 100 % 10];
    led_buf[i++] = led_table_a[year / 1000];


    write(led_buf, 8, MSB_FIRST);
    update();
}
/*********************************
格式：ddmmyy
			261015
*********************************/
void LedSegment::show_date(uint8_t *buf)
{
    int i = 0;
    //for( int i = 0; i < 8; i++)
    led_buf[i++] = led_table_a[(buf[1]) - 0x30];
    led_buf[i++] = led_table_a[(buf[0]) - 0x30];
    led_buf[i++] = led_table_a[(buf[3]) - 0x30];
    led_buf[i++] = led_table_a[(buf[2]) - 0x30];
    led_buf[i++] = led_table_a[(buf[5]) - 0x30];
    led_buf[i++] = led_table_a[(buf[4]) - 0x30];
    led_buf[i++] = led_table_a['0' - 0x30];
    led_buf[i++] = led_table_a['2' - 0x30];


    write(led_buf, 8, MSB_FIRST);
    update();
}
void LedSegment::show_time(uint8_t *buf)
{
    int i = 0;
    //for( int i = 0; i < 8; i++)
    led_buf[i++] = led_table_a[(buf[5]) - 0x30];
    led_buf[i++] = led_table_a[(buf[4]) - 0x30];
    led_buf[i++] = led_table_a[(buf[3]) - 0x30];
    led_buf[i++] = led_table_a[(buf[2]) - 0x30];
    led_buf[i++] = led_table_a[(buf[1]) - 0x30];
    led_buf[i++] = led_table_a[(buf[0]) - 0x30];
    led_buf[i++] = led_table_a[10];
    led_buf[i++] = led_table_a[10];


    write(led_buf, 8, MSB_FIRST);
    update();

}
