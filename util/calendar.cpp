#include "calendar.h"
#include "string.h"
void CALENDAR::begin()
{
		memset(&time_string,0x00,sizeof(time_string));//清除
		memset(&date_string,0x00,sizeof(date_string));//清除
		memset(&cutdown_dddhhmmss_string,0x00,sizeof(cutdown_dddhhmmss_string));//清除	
		memset(&cutdown_hhmmss_string,0x00,sizeof(cutdown_hhmmss_string));//清除	
		memset(&dt,0x00,sizeof(dt));//清除	
		memset(&alarm_dt,0x00,sizeof(alarm_dt));//清除	
}

//GPS接收数据后执行,将GPS接收的数据更新时钟
void CALENDAR::sec_process(u8 *date,u8 *time,u8 timezone_flag,u8 sec_flag)
{
	set_date(date);
	set_time(time);
	
	//时区调整
	if(timezone_flag == 1)//如果是格林威治时间，需要转换为东八区时间
		timezone_adjust(8);
	
	//矫正延时，当中断时，开始接受数据，任务再解析出来的时钟，是延后的，为了弥补这一秒，将时钟调节一秒。
	//秒中断输出到显示器上的时钟才是正确的
	if(sec_flag == 1)
		add_one_sec();
	//更新倒计时字符串
	update_cutdown();
}


//秒中断后执行，系统将自加一秒，驱动时钟的运行
void CALENDAR::sec_process()
{
	add_one_sec();
	//更新倒计时字符串
	update_cutdown();
}

void CALENDAR::set(uint8_t year,uint8_t mon,uint8_t date,uint8_t hour,uint8_t min,uint8_t sec)
{
	dt.year	= year;
	dt.month= mon;
	dt.date	= date;
	dt.hour	= hour;
	dt.min 	= min;
	dt.sec 	= sec;
	get_week(dt.year,dt.month,dt.date,dt.week);
	time_to_str(dt);
	date_to_str(dt);
};

void CALENDAR::set_time(u8 *time)
{

	dt.hour		= uint8_t ((time[0]-0x30)*10 + (time[1]-0x30));
	dt.min		= uint8_t ((time[2]-0x30)*10 + (time[3]-0x30));
	dt.sec		= uint8_t ((time[4]-0x30)*10 + (time[5]-0x30));
	time_to_str(dt);
};
void CALENDAR::set_date(u8 *date)
{
	dt.year		= uint8_t ((date[5]-0x30) + (date[4]-0x30)*10);
	dt.month	= uint8_t ((date[3]-0x30) + (date[2]-0x30)*10);
	dt.date		= uint8_t ((date[1]-0x30) + (date[0]-0x30)*10);
	get_week(dt.year,dt.month,dt.date,dt.week);
	date_to_str(dt);
};
void CALENDAR::timezone_adjust(uint8_t timezone)
{
	dt.hour += timezone;
	if(dt.hour >= 24)
	{
		dt.hour -= 24;
		add_one_day();
	}
	get_week(dt.year,dt.month,dt.date,dt.week);
	time_to_str(dt);
	date_to_str(dt);

}
void CALENDAR::add_one_sec()
{
	if(dt.sec == 59)
	{
		dt.sec = 0;
		if(dt.min == 59)
		{
			dt.min = 0;
			if(dt.hour == 23)
			{
				dt.hour = 0;
				add_one_day();
			}
			else
			{
				dt.hour ++;
			}		
		}
		else
		{
			dt.min ++;
		}			
	}
	else
	{
		dt.sec ++;
	}
	time_to_str(dt);
	date_to_str(dt);
}
void CALENDAR::add_one_day()
{
	switch(dt.month)
	{
		case 1:mon_1_3_5_7_8_10_12_add_one_day();break;
		case 2:mon_2_add_one_day();break;
		case 3:mon_1_3_5_7_8_10_12_add_one_day();break;
		case 4:mon_4_6_9_11_add_one_day();break;
		case 5:mon_1_3_5_7_8_10_12_add_one_day();break;
		case 6:mon_4_6_9_11_add_one_day();break;
		case 7:mon_1_3_5_7_8_10_12_add_one_day();break;
		case 8:mon_1_3_5_7_8_10_12_add_one_day();break;
		case 9:mon_4_6_9_11_add_one_day();break;
		case 10:mon_1_3_5_7_8_10_12_add_one_day();break;
		case 11:mon_4_6_9_11_add_one_day();break;
		case 12:mon_1_3_5_7_8_10_12_add_one_day();break;
	}
	
	get_week(dt.year,dt.month,dt.date,dt.week);
}
int	CALENDAR::dt_changed(DATA_TIME_STRUCT &_dt)
{
	int ret = 0;
	
	if(temp_dt.hour != _dt.hour || temp_dt.min != _dt.min || temp_dt.sec != _dt.sec ||\
			temp_dt.year != _dt.year || temp_dt.month != _dt.month || temp_dt.date != _dt.date
	)
		ret = 1;
	temp_dt = _dt;
	
	return ret;
}

//使用基姆拉尔森计算公式
//把一月和二月看成上一年的十三月和十四月
void CALENDAR::get_week(u16 year,u8 month,u8 date,u8 &_week)
{	
		year += 2000;
    if(month==1||month==2) {
        month+=12;
        year--;
    }
    _week=(date+2*month+3*(month+1)/5+year+year/4-year/100+year/400)%7 + 1;
}

void CALENDAR::alarm_set(uint8_t year,uint8_t mon,uint8_t date,uint8_t hour,uint8_t min,uint8_t sec)
{
	 alarm_dt.year	= year;
	 alarm_dt.month	= mon;
	 alarm_dt.date	= date;
	 alarm_dt.hour	= hour;
	 alarm_dt.min		= min;
	 alarm_dt.sec		= sec;
};
uint8_t CALENDAR::alarm_check()
{
	uint8_t alarm = 0;
	if(_alarm_enable == 1)
	{
		if((alarm_dt.year == dt.year) && (alarm_dt.month == dt.month) && (alarm_dt.date == dt.date) && \
			 (alarm_dt.hour == dt.hour) && (alarm_dt.min == dt.min) && (alarm_dt.sec == dt.sec))
			alarm = 1;
		else
			alarm = 0;
	}
	return alarm;
}
uint8_t CALENDAR::alarm_check_only_time()
{
	uint8_t alarm = 0;
	if(_alarm_enable == 1)
	{
		if((alarm_dt.hour == dt.hour) && (alarm_dt.min == dt.min) && (alarm_dt.sec == dt.sec))
			alarm = 1;
		else
			alarm = 0;
	}
	return alarm;
}
void CALENDAR::alarm_enable(u8 enable)
{

	_alarm_enable = enable;
}
void CALENDAR::update_cutdown()
{
	int i = 0;
	u32 diff_days;
	u32 diff_sec;
	u8 h,m,s;
	if(dt.year > alarm_dt.year)
    {
		//stop
		diff_days = 0;
		diff_sec = 0;
    }
    else if(dt.year == alarm_dt.year)
    {
        if(day_in_year(dt) > day_in_year(alarm_dt))
        {
            //stop
            diff_days = 0;
            diff_sec = 0;
        
        }
        if(day_in_year(dt) < day_in_year(alarm_dt))
        {
            if(sec_in_day(dt) <  sec_in_day(alarm_dt))
            {
                diff_days = days_between_2_date(dt,alarm_dt);
                diff_sec = seconds_between_2_time(dt,alarm_dt);
            }
            else
            {
                diff_days = days_between_2_date(dt,alarm_dt);
                diff_sec = seconds_between_2_time(dt,alarm_dt);
                diff_days--;
                diff_sec = 24*60*60 - diff_sec;
            }		
        }
        if(day_in_year(dt) == day_in_year(alarm_dt))
        {
            diff_days = 0;
            if(sec_in_day(dt) <  sec_in_day(alarm_dt))
            {
                diff_sec = seconds_between_2_time(dt,alarm_dt);
            }
            else
            {
                //stop
                diff_sec = 0;
            }
        }
    }
    else if(dt.year < alarm_dt.year)
    {
        uint16_t year_days_reamain;
        if(is_leap_year(dt.year) == 1)
        {
            year_days_reamain = 366 - day_in_year(dt);
        }
        else
            year_days_reamain = 365 - day_in_year(dt);
            
        diff_days = year_days_reamain + day_in_year(alarm_dt);        
        if(sec_in_day(dt) <  sec_in_day(alarm_dt))
        {
            diff_days = days_between_2_date(dt,alarm_dt);
            diff_sec = seconds_between_2_time(dt,alarm_dt);
        }
        else
        {
            diff_days = days_between_2_date(dt,alarm_dt);
            diff_sec = seconds_between_2_time(dt,alarm_dt);
            diff_days--;
            diff_sec = 24*60*60 - diff_sec;
        }		        
    }
	sec_to_time(diff_sec,h,m,s);
    i=0;
	cutdown_dddhhmmss_string[9] = 0;
	cutdown_dddhhmmss_string[i++] = diff_days/100      + 0x30;
	cutdown_dddhhmmss_string[i++] = (diff_days%100)/10 + 0x30;
	cutdown_dddhhmmss_string[i++] = (diff_days%100)%10 + 0x30;
	cutdown_dddhhmmss_string[i++] = h/10 + 0x30;
	cutdown_dddhhmmss_string[i++] = h%10 + 0x30;
	cutdown_dddhhmmss_string[i++] = m/10 + 0x30;
	cutdown_dddhhmmss_string[i++] = m%10 + 0x30;
	cutdown_dddhhmmss_string[i++] = s/10 + 0x30;
	cutdown_dddhhmmss_string[i++] = s%10 + 0x30;
	cutdown_dddhhmmss_string[i++] = '\0';
	i=0;
	cutdown_hhmmss_string[i++] = h/10 + 0x30;
	cutdown_hhmmss_string[i++] = h%10 + 0x30;
	cutdown_hhmmss_string[i++] = m/10 + 0x30;
	cutdown_hhmmss_string[i++] = m%10 + 0x30;
	cutdown_hhmmss_string[i++] = s/10 + 0x30;
	cutdown_hhmmss_string[i++] = s%10 + 0x30;
	cutdown_hhmmss_string[i++] = '\0';

}

//////////////////////////////////////////////////////////////////////////////
void CALENDAR::swap(u16 *a,u16 *b)
{
	u16 temp;
	temp=*a;
	*a=*b;
	*b=temp;
}

//根据给定的日期算出他是该年的第几天
//1月1号算第一天
u16 CALENDAR::day_in_year(DATA_TIME_STRUCT &_dt)
{
	u16 temp_year;	
	u16 temp_month;	
	u16 temp_day;	
	
	temp_year 	= _dt.year + 2000; 
	temp_month	= _dt.month;	
	temp_day    = _dt.date;
	
	
	u8 DAY[12]={31,28,31,30,31,30,31,31,30,31,30,31};
	if(is_leap_year(temp_year))
			DAY[1] = 29;
	for(u8 i=0; i<temp_month - 1; ++i)
	{
			temp_day += DAY[i];
	}
	return temp_day;		
}
u16	CALENDAR::days_between_2_date(DATA_TIME_STRUCT &dt_current,DATA_TIME_STRUCT &dt_target)
{
	u16 temp_year_c,temp_year_t;	
	u16 temp_month_c,temp_month_t;	
	u16 temp_day_c,temp_day_t;	
	
	temp_year_c = dt_current.year + 2000; 
	temp_year_t = dt_target.year + 2000; 

	temp_month_c = dt_current.month; 
	temp_month_t = dt_target.month; 

	temp_day_c = dt_current.date; 
	temp_day_t = dt_target.date; 

	if((temp_year_c == temp_year_t) && (temp_month_c == temp_month_t))
	{
		return temp_day_t > temp_day_c ? temp_day_t - temp_day_c : temp_day_c - temp_day_t;
	}
	else if(temp_year_c == temp_year_t)
	{
		u16 c_day,t_day;
		c_day = day_in_year(dt_current);
		t_day = day_in_year(dt_target);
		return t_day > c_day ? t_day - c_day : c_day - t_day;
	
	}
	else
	{
		if(temp_year_c > temp_year_t)
		{
			swap(&temp_year_c,&temp_year_t);
			swap(&temp_month_c,&temp_month_t);
			swap(&temp_day_c,&temp_day_t);		
		}
		u16 d1,d2,d3;
		if(is_leap_year(temp_year_c))
		{
				d1 = 366 - day_in_year(dt_current);
		}
		else
			d1 = 365 - day_in_year(dt_current);
		d2 = day_in_year(dt_target);
		d3 = 0;
		for(int year = temp_year_c + 1; year < temp_year_t; year++)
		{
			if(is_leap_year(year))
				 d3 += 366;
			else
				 d3 += 365;
		}
		return d1+d2+d3;
	}
}
u32	CALENDAR::sec_in_day(DATA_TIME_STRUCT &_dt)
{
	return _dt.hour*3600 + _dt.min*60 + _dt.sec;
}
u32 CALENDAR::seconds_between_2_time(DATA_TIME_STRUCT &dt1,DATA_TIME_STRUCT &dt2)
{
	u32 temp_sec1,temp_sec2;

	temp_sec1 = sec_in_day(dt1);
	temp_sec2 = sec_in_day(dt2);
	if(temp_sec1 > temp_sec2)
		return temp_sec1 - temp_sec2;
	else
		return temp_sec2 - temp_sec1;
}
void CALENDAR::sec_to_time(u32 sec_source,u8 &hour,u8 &min,u8 &sec)
{
	hour	= sec_source/3600;
	min		= (sec_source%3600)/60;
	sec		= (sec_source%3600)%60;
}

u8 CALENDAR::is_leap_year(u16 _year)
{
	if(_year%4==0) //必须能被4整除
	{ 
		if(_year%100==0) 
		{ 
			if(_year%400==0)return 1;//如果以00结尾,还要能被400整除 	   
			else return 0;   
		}else return 1;   
	}else return 0;	
}
///////以下是日历计算的私有函数/////////////////////////////////////////////
void CALENDAR::mon_1_3_5_7_8_10_12_add_one_day()
{
	dt.date += 1;
	if(dt.date > 31)
	{
		dt.date -= 31;
		dt.month += 1;
	}
}	
void CALENDAR::mon_4_6_9_11_add_one_day()
{					
	dt.date += 1;
	if(dt.date > 30)
	{
		dt.date -= 30;
		dt.month += 1;
	}
}	
void CALENDAR::mon_2_add_one_day()
{
	dt.date += 1;
	if(is_leap_year(dt.year))
	{
		if(dt.date > 29)
		{
			dt.date -= 29;
			dt.month += 1;
		}							
	}
	else
	{
		if(dt.date > 28)
		{
			dt.date -= 28;
			dt.month += 1;
		}								
	}
}

void CALENDAR::date_to_str(DATA_TIME_STRUCT &_dt)
{
	date_string[0] = _dt.date/10 + 0x30;
	date_string[1] = _dt.date%10 + 0x30;
	date_string[2] = _dt.month/10 + 0x30;
	date_string[3] = _dt.month%10 + 0x30;
	date_string[4] = _dt.year/10 + 0x30;
	date_string[5] = _dt.year%10 + 0x30;
	
	date_string[6] = '\0';
}
void CALENDAR::time_to_str(DATA_TIME_STRUCT &_dt)
{
	time_string[0] = _dt.hour/10 + 0x30;
	time_string[1] = _dt.hour%10 + 0x30;
	time_string[2] = _dt.min/10 + 0x30;
	time_string[3] = _dt.min%10 + 0x30;
	time_string[4] = _dt.sec/10 + 0x30;
	time_string[5] = _dt.sec%10 + 0x30;
	
	time_string[6] = '\0';
}
