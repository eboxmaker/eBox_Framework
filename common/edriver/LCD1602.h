/**
  ******************************************************************************
  * @file    lcd1602.h
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

#ifndef __LCD1602_1_H
#define __LCD1602_1_H

#include "ebox.h"
class Lcd1602
{

public:
    Lcd1602(Gpio *led, Gpio *en, Gpio *rw, Gpio *rs, Gpio *d0, Gpio *d1, Gpio *d2, Gpio *d3, Gpio *d4, Gpio *d5, Gpio *d6, Gpio *d7);

    Lcd1602(Gpio *LEDPin, Gpio *ENPin, Gpio *RWPin, Gpio *RSPin, Gpio *DB0, Gpio *DB1, Gpio *DB2, Gpio *DB3);
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
    void back_light(uint8_t i);
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
    Gpio *led;
    Gpio *en;
    Gpio *rw;
    Gpio *rs;
    Gpio *d0;
    Gpio *d1;
    Gpio *d2;
    Gpio *d3;
    Gpio *d4;
    Gpio *d5;
    Gpio *d6;
    Gpio *d7;

    uint16_t delay_times;
    inline void lcd_delay(uint32_t nCount);
    inline uint8_t bz();
    void wcmd(uint8_t cmd);
    void wdat(uint8_t dat) ;
    void pos(uint8_t pos);



};


#endif
