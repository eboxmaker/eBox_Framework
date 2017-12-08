/**
  ******************************************************************************
  * @file    calendar.h
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


#define TIMEZONE 8
class Calendar
{
	public:
		DateTime_t dt;
	
	public:
		void begin();
		//每秒执行一次驱动时钟的运行
		//GPS接收数据后执行,将GPS接收的数据更新时钟.或者选用其他时钟源，比如DS3231，DS1302等等
		void sec_process(uint8_t *date,uint8_t *time,uint8_t timezone_flag,uint8_t sec_flag);//格式date:ddmmyy
																			//time:hhmmss
		//秒中断后执行，系统将自加一秒，可实现整个日历的运行
		void sec_process();
	
		//基础时钟函数
		void	set(uint8_t year,uint8_t mon,uint8_t date,uint8_t hour,uint8_t min,uint8_t sec);
		void	set_time(uint8_t *time);//1.字符串设置时间。
								   //2.GPS驱动时钟，所需要调用的子函数
		void	set_date(uint8_t *date);//1.字符串设置日期。
                                    //2.GPS驱动时钟，所需要调用的子函数
		void	timezone_adjust(uint8_t timezone);//时区调整，自动调整日期的转换
		void	add_one_sec();//1.用于时间的矫正。
		void	add_one_day();//日期的矫正。
    
		int		dt_changed(DateTime_t &_dt);//检测时间发生变化
    
        DateTime_t  get_time();
        uint32_t    get_unix_timestamp();

	
        //字符串输出格式
        String    get_time_to_string();
        String    get_date_to_string();
        String    get_date_time_to_string();
        String    get_unix_timestamp_to_string();
	
		
	//获取某天是星期几
		void	get_week(u16 year,uint8_t month,uint8_t date,uint8_t &_week);//获取某天是星期几
	
    
	public:
        //倒计时、闹钟相关函数
		DateTime_t alarm_dt;

        void	    alarm_set(uint8_t year,uint8_t mon,uint8_t date,uint8_t hour,uint8_t min,uint8_t sec);
		uint8_t		alarm_check();//检查闹钟时候到时间
		uint8_t		alarm_check_only_time();//检查闹钟时候到时间
		void	    alarm_enable(uint8_t enable);//倒计时的使能
        
        String      get_cutdown_dddhhmmss_to_string();
        String      get_cutdown_hhmmss_to_string();
        
        
        
        
	private:
	  ///////以下是日历计算的私有函数///////////////////////
		uint8_t 		is_leap_year(u16 _year);//是否是闰年
		void	mon_1_3_5_7_8_10_12_add_one_day();//天的进位
		void	mon_4_6_9_11_add_one_day();//天的进位
		void	mon_2_add_one_day();//天的进位
		///////倒计时计算相关的函数////////////////////////
		uint8_t         _alarm_enable;
		void	        swap(u16 *a,u16 *b);//交换两个u16型变量的数据
		u16		        day_in_year(DateTime_t &_dt);//日期是那年的第几天
		u16		        days_between_2_date(DateTime_t &dt_current,DateTime_t &dt_target);//两个日期之间的天数差
		uint32_t		sec_in_day(DateTime_t &_dt);//时间是当天的第几秒
		uint32_t		seconds_between_2_time(DateTime_t &dt1,DateTime_t &dt2);//两个时间之间的秒数差
		void	        sec_to_time(uint32_t sec_source,uint8_t &hour,uint8_t &min,uint8_t &sec);//秒转换成hms格式的时间
		///////////////////////////////////////////////////////
		DateTime_t temp_dt;	//用于检测时间变化
};


#endif

