#ifndef __DS3231_H__
#define __DS3231_H__

#include "ebox.h"
#include "softi2c.h"
		
#define DS3231_WriteAddress 0xD0    //器件写地址 
#define DS3231_ReadAddress  0xD1    //器件读地址
#define DS3231_SECOND       0x00    //秒
#define DS3231_MINUTE       0x01    //分
#define DS3231_HOUR         0x02    //时
#define DS3231_WEEK         0x03    //星期
#define DS3231_DAY          0x04    //日
#define DS3231_MONTH        0x05    //月
#define DS3231_YEAR         0x06    //年
//闹铃1            
#define DS3231_SALARM1ECOND 0x07    //秒
#define DS3231_ALARM1MINUTE 0x08    //分
#define DS3231_ALARM1HOUR   0x09    //时
#define DS3231_ALARM1WEEK   0x0A    //星期/日
//闹铃2
#define DS3231_ALARM2MINUTE 0x0b    //分
#define DS3231_ALARM2HOUR   0x0c    //时
#define DS3231_ALARM2WEEK   0x0d    //星期/日
#define DS3231_CONTROL      0x0e    //控制寄存器
#define DS3231_STATUS       0x0f    //状态寄存器
#define BSY                 2       //忙
#define OSF                 7       //振荡器停止标志
#define DS3231_XTAL         0x10    //晶体老化寄存器
#define DS3231_TEMPERATUREH 0x11    //温度寄存器高字节(8位)
#define DS3231_TEMPERATUREL 0x12    //温度寄存器低字节(高2位)  
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

 class DS3231:public Softi2c
 {
	 private:
			uint8_t BcdToDec(uint8_t BCDCode);
			uint8_t DecToBcd(uint8_t Dec);
	 public:
			DateTime t;
			DS3231(uint8_t SDApin,uint8_t SCLpin):Softi2c(SDApin,SCLpin){
			// do nothing;
			};	
			void begin()
			{
				i2cBegin();
			};
			void getDateTime(DateTime *t);
			void getTime(char* buf);
			void getDate(char* buf);
			void setTime(DateTime *t);
 
 };


#endif
