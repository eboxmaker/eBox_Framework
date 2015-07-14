/*
file   : ds3231.h
author : shentq
version: V1.0
date   : 2015/7/5

Copyright (c) 2015, eBox by shentqlf@163.com. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentqlf@163.com.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentqlf@163.com assumes no responsibility for any errors contained herein.
*/

#ifndef __DS3231_H__
#define __DS3231_H__

#include "ebox.h"

		
#define DS3231_WriteAddress 0xD0    //����д��ַ 
#define DS3231_ReadAddress  0xD1    //��������ַ
#define DS3231_SECOND       0x00    //��
#define DS3231_MINUTE       0x01    //��
#define DS3231_HOUR         0x02    //ʱ
#define DS3231_WEEK         0x03    //����
#define DS3231_DAY          0x04    //��
#define DS3231_MONTH        0x05    //��
#define DS3231_YEAR         0x06    //��
//����1            
#define DS3231_SALARM1ECOND 0x07    //��
#define DS3231_ALARM1MINUTE 0x08    //��
#define DS3231_ALARM1HOUR   0x09    //ʱ
#define DS3231_ALARM1WEEK   0x0A    //����/��
//����2
#define DS3231_ALARM2MINUTE 0x0b    //��
#define DS3231_ALARM2HOUR   0x0c    //ʱ
#define DS3231_ALARM2WEEK   0x0d    //����/��
#define DS3231_CONTROL      0x0e    //���ƼĴ���
#define DS3231_STATUS       0x0f    //״̬�Ĵ���
#define BSY                 2       //æ
#define OSF                 7       //����ֹͣ��־
#define DS3231_XTAL         0x10    //�����ϻ��Ĵ���
#define DS3231_TEMPERATUREH 0x11    //�¶ȼĴ������ֽ�(8λ)
#define DS3231_TEMPERATUREL 0x12    //�¶ȼĴ������ֽ�(��2λ)  
//timer.w_year,timer.w_month,timer.w_date,timer.hour,timer.min,timer.sec
 typedef struct
 {
	 u8 w;
	 u8 year;
	 u8 month;
	 u8 date;
	 u8 hour;
	 u8 min;
	 u8 sec;
	 u8 week;
 
 }DateTime;

 class DS3231:public SOFTI2C
 {
	 public:
			DateTime t;
			DS3231(GPIO* SDApin,GPIO* SCLpin):SOFTI2C(SDApin,SCLpin){
			// do nothing;
			};	
			void begin(uint32_t speed)
			{
				i2cBegin(speed);
			};
			void getDateTime(DateTime *t);
			void getTime(char* buf);
			void getDate(char* buf);
			void setTime(DateTime *t);
 
	 private:
			uint8_t BcdToDec(uint8_t BCDCode);
			uint8_t DecToBcd(uint8_t Dec);
 };


#endif
