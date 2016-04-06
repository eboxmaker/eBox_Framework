#ifndef __CLOCK_H
#define __CLOCK_H
#include "ebox.h"
/******************************************************
可以用于GPS,BD模块驱动时间
也可以用于RTC秒中断驱动时间
---------------------------------
1.标准的时钟服务
2.时区调整
3.倒计时闹钟
4.倒计时字符串、时间字符串、日期字符串
5.星期的计算
---------------------------------
日历中的年份以两位数代替。只能从2000年记起
*******************************************************/
typedef struct
{
	uint8_t year;
	uint8_t month;
	uint8_t date;
	uint8_t hour;
	uint8_t min;
	uint8_t sec;
	uint8_t week;
}DATA_TIME_STRUCT;

#define TIMEZONE 8
class CALENDAR
{
	public:
		DATA_TIME_STRUCT dt;
	
		u8 time_string[7];//"hhmmss"
		u8 date_string[7];//"ddmmyy"
	public:
		void begin();
		//每秒执行一次驱动时钟的运行
		//GPS接收数据后执行,将GPS接收的数据更新时钟.或者选用其他时钟源，比如DS3231，DS1302等等
		void sec_process(u8 *date,u8 *time,u8 timezone_flag,u8 sec_flag);//格式date:ddmmyy
																			//time:hhmmss
		//秒中断后执行，系统将自加一秒，驱动时钟的运行
		void sec_process();
	
		//基础时钟函数
		void	set(uint8_t year,uint8_t mon,uint8_t date,uint8_t hour,uint8_t min,uint8_t sec);
		void	set_time(u8 *time);//1.字符串设置时间。
								   //2.GPS驱动时钟，所需要调用的子函数
		void	set_date(u8 *date);//1.字符串设置日期。
                                    //2.GPS驱动时钟，所需要调用的子函数
		void	timezone_adjust(uint8_t timezone);//时区调整，自动调整日期的转换
		void	add_one_sec();//1.用于时间的矫正。
                                //2.如果系统提供秒中断服务，可使用秒中断驱动本函数，可实现整个日历的运行
		void	add_one_day();//日期的矫正。
	
		int		dt_changed(DATA_TIME_STRUCT &_dt);//检测时间发生变化
	
		
	//获取某天是星期几
		void	get_week(u16 year,u8 month,u8 date,u8 &_week);//获取某天是星期几
	
    
	public:
        //倒计时、闹钟相关函数
		DATA_TIME_STRUCT alarm_dt;
		u8 cutdown_dddhhmmss_string[10];//"dddhhmmss"。倒计时的字符串
		u8 cutdown_hhmmss_string[7];//"hhmmss"。倒计时的字符串


        void	alarm_set(uint8_t year,uint8_t mon,uint8_t date,uint8_t hour,uint8_t min,uint8_t sec);
		u8		alarm_check();//检查闹钟时候到时间
		u8		alarm_check_only_time();//检查闹钟时候到时间
		void	alarm_enable(u8 enable);//倒计时的使能
		void	update_cutdown();//更新倒计时字符串
        
        
        
        
        
        
	private:
	  ///////以下是日历计算的私有函数///////////////////////
		u8 		is_leap_year(u16 _year);//是否是闰年
		void	mon_1_3_5_7_8_10_12_add_one_day();//天的进位
		void	mon_4_6_9_11_add_one_day();//天的进位
		void	mon_2_add_one_day();//天的进位
		void	date_to_str(DATA_TIME_STRUCT &_dt);//日期转换为ddmmyy格式的字符串
		void	time_to_str(DATA_TIME_STRUCT &_dt);//时间转换为hhmmss格式的字符串
		///////倒计时计算相关的函数////////////////////////
		u8		_alarm_enable;
		void	swap(u16 *a,u16 *b);//交换两个u16型变量的数据
		u16		day_in_year(DATA_TIME_STRUCT &_dt);//日期是那年的第几天
		u16		days_between_2_date(DATA_TIME_STRUCT &dt_current,DATA_TIME_STRUCT &dt_target);//两个日期之间的天数差
		u32		sec_in_day(DATA_TIME_STRUCT &_dt);//时间是当天的第几秒
		u32		seconds_between_2_time(DATA_TIME_STRUCT &dt1,DATA_TIME_STRUCT &dt2);//两个时间之间的秒数差
		void	sec_to_time(u32 sec_source,u8 &hour,u8 &min,u8 &sec);//秒转换成hms格式的时间
		///////////////////////////////////////////////////////
		DATA_TIME_STRUCT temp_dt;	//用于检测时间变化
};


#endif

