
/*
file   : ds3231.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright (c) 2015, eBox by shentqlf@163.com. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentqlf@163.com.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentqlf@163.com assumes no responsibility for any errors contained herein.
*/

#include "DS3231.h"
	


uint8_t DS3231::BcdToDec(uint8_t BCDCode)
{
	u8 temp,Dec;
	temp = (BCDCode >> 4) * 10;
	Dec = (BCDCode & 0x0f) + temp;
	return Dec;
}
uint8_t DS3231::DecToBcd(uint8_t Dec)
{
	u8 temp,temp2,Bcd;
	temp = Dec / 10;
	temp2 = Dec % 10;
	
	Bcd =  (temp << 4) + temp2;
	
	return Bcd;
}

void	DS3231::getDateTime(DateTime *t)
{
	uint8_t buf[8];

	readByte(DS3231_WriteAddress,DS3231_SECOND,buf,7);


//	timer.w_year,timer.w_month,timer.w_date,timer.hour,timer.min,timer.sec
/******����ȡ��ʮ����������ת��Ϊʮ��������******/
	t->year 	=BcdToDec(buf[6]);
	t->month	=BcdToDec(buf[5]);
	t->date	=BcdToDec(buf[4]);
	t->week 		=BcdToDec(buf[3]);
	t->hour		=BcdToDec(buf[2]);
	t->min			=BcdToDec(buf[1]);
	t->sec	    =BcdToDec(buf[0]);	
}
void DS3231::getDate(char* buf)
{		
	uint8_t tmpbuf[3];

	readByte(DS3231_WriteAddress,DS3231_DAY,tmpbuf,3);				//����

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
void DS3231::getTime(char* buf)
{
	uint8_t tmpbuf[3];

	readByte(DS3231_WriteAddress,DS3231_SECOND,tmpbuf,3);		//��

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
void	DS3231::setTime(DateTime *t)
{
		DateTime tBCD;
		tBCD.week = DecToBcd(t->week);
		tBCD.year = DecToBcd(t->year);
		tBCD.month = DecToBcd(t->month);
		tBCD.date = DecToBcd(t->date);
		tBCD.hour = DecToBcd(t->hour);
		tBCD.min = DecToBcd(t->min);
		tBCD.sec = DecToBcd(t->sec);
	

    writeByte(DS3231_WriteAddress,DS3231_WEEK,tBCD.week);   //�޸���
		writeByte(DS3231_WriteAddress,DS3231_YEAR,tBCD.year);   //�޸���
    writeByte(DS3231_WriteAddress,DS3231_MONTH,tBCD.month);  //�޸���
    writeByte(DS3231_WriteAddress,DS3231_DAY,tBCD.date);    //�޸���
    writeByte(DS3231_WriteAddress,DS3231_HOUR,tBCD.hour);   //�޸�ʱ
    writeByte(DS3231_WriteAddress,DS3231_MINUTE,tBCD.min); //�޸ķ�
    writeByte(DS3231_WriteAddress,DS3231_SECOND,tBCD.sec ); //�޸���
}
	
