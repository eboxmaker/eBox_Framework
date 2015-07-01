
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

void	DS3231::getTime(DateTime *t)
{
	readByte(DS3231_WriteAddress,DS3231_YEAR,&buf[6]);			//年份低两位
	readByte(DS3231_WriteAddress,DS3231_MONTH,&buf[5]);			//月份
	readByte(DS3231_WriteAddress,DS3231_DAY,&buf[4]);				//日期
	readByte(DS3231_WriteAddress,DS3231_WEEK,&buf[3]);			//周
	readByte(DS3231_WriteAddress,DS3231_HOUR,&buf[2]);			//时
	readByte(DS3231_WriteAddress,DS3231_MINUTE,&buf[1]);		//分
	readByte(DS3231_WriteAddress,DS3231_SECOND,&buf[0]);		//秒


//	timer.w_year,timer.w_month,timer.w_date,timer.hour,timer.min,timer.sec
/******将读取的十六进制数据转换为十进制数据******/
	t->year 	=BcdToDec(buf[6]);
	t->month	=BcdToDec(buf[5]);
	t->date	=BcdToDec(buf[4]);
	t->week 		=BcdToDec(buf[3]);
	t->hour		=BcdToDec(buf[2]);
	t->min			=BcdToDec(buf[1]);
	t->sec	    =BcdToDec(buf[0]);	
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
   
    writeByte(DS3231_WriteAddress,DS3231_WEEK,tBCD.week);   //修改周
		writeByte(DS3231_WriteAddress,DS3231_YEAR,tBCD.year);   //修改年
    writeByte(DS3231_WriteAddress,DS3231_MONTH,tBCD.month);  //修改月
    writeByte(DS3231_WriteAddress,DS3231_DAY,tBCD.date);    //修改日
    writeByte(DS3231_WriteAddress,DS3231_HOUR,tBCD.hour);   //修改时
    writeByte(DS3231_WriteAddress,DS3231_MINUTE,tBCD.min); //修改分
    writeByte(DS3231_WriteAddress,DS3231_SECOND,tBCD.sec ); //修改秒
}
	
