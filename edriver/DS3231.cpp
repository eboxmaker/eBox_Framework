
/*
file   : ds3231.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentqlf@163.com.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentqlf@163.com assumes no responsibility for any errors contained herein.
*/

#include "DS3231.h"
	

uint8_t DS3231::bcd_to_dec(uint8_t bcd_code)
{
	u8 temp,dec;
	temp = (bcd_code >> 4) * 10;
	dec = (bcd_code & 0x0f) + temp;
	return dec;
}
uint8_t DS3231::dec_to_bcd(uint8_t dec)
{
	u8 temp,temp2,bcd;
	temp = dec / 10;
	temp2 = dec % 10;
	
	bcd =  (temp << 4) + temp2;
	
	return bcd;
}

void	DS3231::get_date_time(date_time_typedef *t)
{
	uint8_t buf[8];

	i2c->read_byte(DS3231_ADDRESS,DS3231_SECOND,buf,7);


//	timer.w_year,timer.w_month,timer.w_date,timer.hour,timer.min,timer.sec
/******将读取的十六进制数据转换为十进制数据******/
	t->year 	=bcd_to_dec(buf[6]);
	t->month	=bcd_to_dec(buf[5]);
	t->date		=bcd_to_dec(buf[4]);
	t->week 	=bcd_to_dec(buf[3]);
	t->hour		=bcd_to_dec(buf[2]);
	t->min		=bcd_to_dec(buf[1]);
	t->sec	  =bcd_to_dec(buf[0]);	
}
void DS3231::get_date(char* buf)
{		
	uint8_t tmpbuf[3];

	i2c->read_byte(DS3231_ADDRESS,DS3231_DAY,tmpbuf,3);				//日期

	buf[0] =char( (tmpbuf[2]>>4) + 0x30);
	buf[1] =char( (tmpbuf[2]&0x0f) + 0x30);
	buf[2] = '-';
	buf[3] =char( (tmpbuf[1]>>4) + 0x30);
	buf[4] =char( (tmpbuf[1]&0x0f) + 0x30);
	buf[5] = '-';
	buf[6] =char( (tmpbuf[0]>>4) + 0x30);
	buf[7] =char( (tmpbuf[0]&0x0f) + 0x30);
	buf[8] ='\0';
}
void DS3231::get_time(char* buf)
{
	uint8_t tmpbuf[3];

	i2c->read_byte(DS3231_ADDRESS,DS3231_SECOND,tmpbuf,3);		

	buf[0] =char( (tmpbuf[2]>>4) + 0x30);
	buf[1] =char( (tmpbuf[2]&0x0f) + 0x30);
	buf[2] = ':';
	buf[3] =char( (tmpbuf[1]>>4) + 0x30);
	buf[4] =char( (tmpbuf[1]&0x0f) + 0x30);
	buf[5] = ':';
	buf[6] =char( (tmpbuf[0]>>4) + 0x30);
	buf[7] =char( (tmpbuf[0]&0x0f) + 0x30);
	buf[8] ='\0';

	
}
void	DS3231::set_time(date_time_typedef *t)
{
		date_time_typedef tBCD;
		tBCD.week = dec_to_bcd(t->week);
		tBCD.year = dec_to_bcd(t->year);
		tBCD.month = dec_to_bcd(t->month);
		tBCD.date = dec_to_bcd(t->date);
		tBCD.hour = dec_to_bcd(t->hour);
		tBCD.min = dec_to_bcd(t->min);
		tBCD.sec = dec_to_bcd(t->sec);
	

    i2c->write_byte(DS3231_ADDRESS,DS3231_WEEK,tBCD.week);   //修改周
		i2c->write_byte(DS3231_ADDRESS,DS3231_YEAR,tBCD.year);   //修改年
    i2c->write_byte(DS3231_ADDRESS,DS3231_MONTH,tBCD.month);  //修改月
    i2c->write_byte(DS3231_ADDRESS,DS3231_DAY,tBCD.date);    //修改日
    i2c->write_byte(DS3231_ADDRESS,DS3231_HOUR,tBCD.hour);   //修改时
    i2c->write_byte(DS3231_ADDRESS,DS3231_MINUTE,tBCD.min); //修改分
    i2c->write_byte(DS3231_ADDRESS,DS3231_SECOND,tBCD.sec ); //修改秒
}
	
