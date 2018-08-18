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
#include "ebox_core.h"
/******************************************************
��������GPS,BDģ������ʱ��
Ҳ��������RTC���ж�����ʱ��
---------------------------------
1.��׼��ʱ�ӷ���
2.ʱ������
3.����ʱ����
4.����ʱ�ַ�����ʱ���ַ����������ַ���
5.���ڵļ���
---------------------------------
�����е��������λ�����档ֻ�ܴ�2000�����
*******************************************************/


#define TIMEZONE 8
class Calendar
{
	public:
		DateTime_t dt;
	
	public:
		void begin();
		//ÿ��ִ��һ������ʱ�ӵ�����
		//GPS�������ݺ�ִ��,��GPS���յ����ݸ���ʱ��.����ѡ������ʱ��Դ������DS3231��DS1302�ȵ�
		void sec_process(uint8_t *date,uint8_t *time,uint8_t timezone_flag,uint8_t sec_flag);//��ʽdate:ddmmyy
																			//time:hhmmss
		//���жϺ�ִ�У�ϵͳ���Լ�һ�룬��ʵ����������������
		void sec_process();
	
		//����ʱ�Ӻ���
		void	set(uint8_t year,uint8_t mon,uint8_t date,uint8_t hour,uint8_t min,uint8_t sec);
		void	set_time(uint8_t *time);//1.�ַ�������ʱ�䡣
								   //2.GPS����ʱ�ӣ�����Ҫ���õ��Ӻ���
		void	set_date(uint8_t *date);//1.�ַ����������ڡ�
                                    //2.GPS����ʱ�ӣ�����Ҫ���õ��Ӻ���
		void	timezone_adjust(uint8_t timezone);//ʱ���������Զ��������ڵ�ת��
		void	add_one_sec();//1.����ʱ��Ľ�����
		void	add_one_day();//���ڵĽ�����
    
		int		dt_changed(DateTime_t &_dt);//���ʱ�䷢���仯
    
        DateTime_t  get_time();
        uint32_t    get_unix_timestamp();

	
        //�ַ��������ʽ
        String    get_time_to_string();
        String    get_date_to_string();
        String    get_date_time_to_string();
        String    get_unix_timestamp_to_string();
	
		
	//��ȡĳ�������ڼ�
		void	get_week(uint16_t year,uint8_t month,uint8_t date,uint8_t &_week);//��ȡĳ�������ڼ�
	
    
	public:
        //����ʱ��������غ���
		DateTime_t alarm_dt;

        void	    alarm_set(uint8_t year,uint8_t mon,uint8_t date,uint8_t hour,uint8_t min,uint8_t sec);
		uint8_t		alarm_check();//�������ʱ��ʱ��
		uint8_t		alarm_check_only_time();//�������ʱ��ʱ��
		void	    alarm_enable(uint8_t enable);//����ʱ��ʹ��
        
        String      get_cutdown_dddhhmmss_to_string();
        String      get_cutdown_hhmmss_to_string();
        
        
        
        
	private:
	  ///////���������������˽�к���///////////////////////
		uint8_t 		is_leap_year(uint16_t _year);//�Ƿ�������
		void	mon_1_3_5_7_8_10_12_add_one_day();//��Ľ�λ
		void	mon_4_6_9_11_add_one_day();//��Ľ�λ
		void	mon_2_add_one_day();//��Ľ�λ
		///////����ʱ������صĺ���////////////////////////
		uint8_t         _alarm_enable;
		void	        swap(uint16_t *a,uint16_t *b);//��������uint16_t�ͱ���������
		uint16_t		        day_in_year(DateTime_t &_dt);//����������ĵڼ���
		uint16_t		        days_between_2_date(DateTime_t &dt_current,DateTime_t &dt_target);//��������֮���������
		uint32_t		sec_in_day(DateTime_t &_dt);//ʱ���ǵ���ĵڼ���
		uint32_t		seconds_between_2_time(DateTime_t &dt1,DateTime_t &dt2);//����ʱ��֮���������
		void	        sec_to_time(uint32_t sec_source,uint8_t &hour,uint8_t &min,uint8_t &sec);//��ת����hms��ʽ��ʱ��
		///////////////////////////////////////////////////////
		DateTime_t temp_dt;	//���ڼ��ʱ��仯
};


#endif

