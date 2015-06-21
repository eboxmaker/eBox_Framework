
#include "DS3231.h"
	
	

uint8_t DS3231::write_byte(uint8_t addr, uint8_t write_data)
{
		_start();
		_send_byte(DS3231_WriteAddress);
    if (_wait_ack() == 0)
        return 0;
		
		_send_byte(addr);
    if (_wait_ack() == 0)
        return 0;

   
		_send_byte(write_data);
    if (_wait_ack() == 0)
        return 0;
		_stop();

    delay_us(10);      
    return 1;
}
uint8_t DS3231::read_current(void)
{
    unsigned char read_data;
		_start();
		_send_byte(DS3231_ReadAddress);
		if (_wait_ack() == 0)
       return (0);

		read_data = _read_byte();
		_send_no_ack();
		_stop();
    return read_data;
}
uint8_t DS3231::read_random(uint8_t random_addr)
{
		_start();
		_send_byte(DS3231_WriteAddress);
		if (_wait_ack() == 0)
       return (0);
		
		_send_byte(random_addr);
		if (_wait_ack() == 0)
       return (0);
		

    return(read_current());
}

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

void	DS3231::get_time(DateTime *t)
{
	buf[6]=read_random(DS3231_YEAR);			//年份低两位
	buf[5]=read_random(DS3231_MONTH);	   	//月份
	buf[4]=read_random(DS3231_DAY);		//日期
	buf[3]=read_random(DS3231_WEEK);
	buf[2]=read_random(DS3231_HOUR) & 0x3f;		//时
	buf[1]=read_random(DS3231_MINUTE);		//分
	buf[0]=read_random(DS3231_SECOND);	    	//秒

//	timer.w_year,timer.w_month,timer.w_date,timer.hour,timer.min,timer.sec
/******将读取的十六进制数据转换为十进制数据******/
	t->year 	=BcdToDec(buf[6])+2000;
	t->month	=BcdToDec(buf[5]);
	t->date	=BcdToDec(buf[4]);
	t->week 		=BcdToDec(buf[3]);
	t->hour		=BcdToDec(buf[2]);
	t->min			=BcdToDec(buf[1]);
	t->sec	    =BcdToDec(buf[0]);	
}
void	DS3231::set_time(DateTime *t)
{
		DateTime tBCD;
		tBCD.week = DecToBcd(t->week);
		tBCD.year = DecToBcd(t->year-2000);
		tBCD.month = DecToBcd(t->month);
		tBCD.date = DecToBcd(t->date);
		tBCD.hour = DecToBcd(t->hour);
		tBCD.min = DecToBcd(t->min);
		tBCD.sec = DecToBcd(t->sec);
   
    write_byte(DS3231_WEEK,tBCD.week);   //修改周
		write_byte(DS3231_YEAR,tBCD.year);   //修改年
    write_byte(DS3231_MONTH,tBCD.month);  //修改月
   
    write_byte(DS3231_DAY,tBCD.date);    //修改日
   
    write_byte(DS3231_HOUR,tBCD.hour);   //修改时
   
    write_byte(DS3231_MINUTE,tBCD.min); //修改分
   
    write_byte(DS3231_SECOND,tBCD.sec ); //修改秒
}
	
