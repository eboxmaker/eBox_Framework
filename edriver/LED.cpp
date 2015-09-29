/*
file   : led.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentqlf@163.com.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentqlf@163.com assumes no responsibility for any errors contained herein.
*/

#include "led.h"

uint8_t led_table_a[]   ={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e,0xff}; //共阳极
uint8_t led_table_k[]  ={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e,0xff};//共阴极
void LED::begin()
{
	_74hc595::begin();
}

void LED::show_time_HMS(uint8_t hour,uint8_t min,uint8_t sec)
{
	int i=0;
		//for( int i = 0; i < 8; i++)
		led_buf[i++] = led_table_a[16];
		led_buf[i++] = led_table_a[16];
		led_buf[i++] = led_table_a[sec%10];
		led_buf[i++] = led_table_a[sec/10];
		led_buf[i++] = led_table_a[min%10];
		led_buf[i++] = led_table_a[min/10];
		led_buf[i++] = led_table_a[hour%10];
		led_buf[i++] = led_table_a[hour/10];

		write(led_buf,8,MSB_FIRST);
		update();
}
void LED::show_date_time(uint16_t year,uint8_t month,uint8_t date)
{
	int i=0;
		//for( int i = 0; i < 8; i++)
		led_buf[i++] = led_table_a[date%10];
		led_buf[i++] = led_table_a[date/10];
		led_buf[i++] = led_table_a[month%10];
		led_buf[i++] = led_table_a[month/10];
		led_buf[i++] = led_table_a[year%10];
		led_buf[i++] = led_table_a[year/10%10];
		led_buf[i++] = led_table_a[year/100%10];
		led_buf[i++] = led_table_a[year/1000];


		write(led_buf,8,MSB_FIRST);
	  update();
}
