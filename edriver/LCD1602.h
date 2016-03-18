/*
file   : lcd1602.h
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentqlf@163.com.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentqlf@163.com assumes no responsibility for any errors contained herein.
*/

#ifndef __LCD1602_1_H
#define __LCD1602_1_H

#include "ebox.h"
class LCD1602
{

public:
    LCD1602(GPIO *led, GPIO *en, GPIO *rw, GPIO *rs, GPIO *d0, GPIO *d1, GPIO *d2, GPIO *d3, GPIO *d4, GPIO *d5, GPIO *d6, GPIO *d7);

    LCD1602(GPIO *LEDPin, GPIO *ENPin, GPIO *RWPin, GPIO *RSPin, GPIO *DB0, GPIO *DB1, GPIO *DB2, GPIO *DB3);
    //基本功能
    void begin(void);
    void set_pos(uint8_t row, uint8_t col);

    void disp_char(char ch);
    void disp_char(uint8_t row, uint8_t col, char ch);

    void disp_string(char str[]);
    void disp_string(uint8_t row, uint8_t col, char str[]);

    void printf(const char *fmt, ...);
    void printf(uint8_t row, uint8_t col, const char *fmt, ...);
    void test(void);
    //基本控制函数
    void back_light(u8 i);
    void shift_left();
    void shift_right(void);
    void clear(void);
    void Return(void);
    void close(void);
    void open(void);
    void flicker(void);
    void flicker_char(uint8_t row, uint8_t col);
    void close_flicker(void);
    void flicker_screen(void);
    //用户自定义功能
    void disp_time_HMS(uint8_t hour, uint8_t min, uint8_t sec);
    void disp_time_YMD(uint32_t year, uint8_t month, uint8_t day);
    void disp_date_time(uint32_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec);
private:
    GPIO *led;
    GPIO *en;
    GPIO *rw;
    GPIO *rs;
    GPIO *d0;
    GPIO *d1;
    GPIO *d2;
    GPIO *d3;
    GPIO *d4;
    GPIO *d5;
    GPIO *d6;
    GPIO *d7;

    uint16_t delay_times;
    inline void lcd_delay(uint32_t nCount);
    inline uint8_t bz();
    void wcmd(uint8_t cmd);
    void wdat(uint8_t dat) ;
    void pos(uint8_t pos);



};


#endif
