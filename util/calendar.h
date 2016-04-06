#ifndef __CLOCK_H
#define __CLOCK_H
#include "ebox.h"
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
		//ÿ��ִ��һ������ʱ�ӵ�����
		//GPS�������ݺ�ִ��,��GPS���յ����ݸ���ʱ��.����ѡ������ʱ��Դ������DS3231��DS1302�ȵ�
		void sec_process(u8 *date,u8 *time,u8 timezone_flag,u8 sec_flag);//��ʽdate:ddmmyy
																			//time:hhmmss
		//���жϺ�ִ�У�ϵͳ���Լ�һ�룬����ʱ�ӵ�����
		void sec_process();
	
		//����ʱ�Ӻ���
		void	set(uint8_t year,uint8_t mon,uint8_t date,uint8_t hour,uint8_t min,uint8_t sec);
		void	set_time(u8 *time);//1.�ַ�������ʱ�䡣
								   //2.GPS����ʱ�ӣ�����Ҫ���õ��Ӻ���
		void	set_date(u8 *date);//1.�ַ����������ڡ�
                                    //2.GPS����ʱ�ӣ�����Ҫ���õ��Ӻ���
		void	timezone_adjust(uint8_t timezone);//ʱ���������Զ��������ڵ�ת��
		void	add_one_sec();//1.����ʱ��Ľ�����
                                //2.���ϵͳ�ṩ���жϷ��񣬿�ʹ�����ж���������������ʵ����������������
		void	add_one_day();//���ڵĽ�����
	
		int		dt_changed(DATA_TIME_STRUCT &_dt);//���ʱ�䷢���仯
	
		
	//��ȡĳ�������ڼ�
		void	get_week(u16 year,u8 month,u8 date,u8 &_week);//��ȡĳ�������ڼ�
	
    
	public:
        //����ʱ��������غ���
		DATA_TIME_STRUCT alarm_dt;
		u8 cutdown_dddhhmmss_string[10];//"dddhhmmss"������ʱ���ַ���
		u8 cutdown_hhmmss_string[7];//"hhmmss"������ʱ���ַ���


        void	alarm_set(uint8_t year,uint8_t mon,uint8_t date,uint8_t hour,uint8_t min,uint8_t sec);
		u8		alarm_check();//�������ʱ��ʱ��
		u8		alarm_check_only_time();//�������ʱ��ʱ��
		void	alarm_enable(u8 enable);//����ʱ��ʹ��
		void	update_cutdown();//���µ���ʱ�ַ���
        
        
        
        
        
        
	private:
	  ///////���������������˽�к���///////////////////////
		u8 		is_leap_year(u16 _year);//�Ƿ�������
		void	mon_1_3_5_7_8_10_12_add_one_day();//��Ľ�λ
		void	mon_4_6_9_11_add_one_day();//��Ľ�λ
		void	mon_2_add_one_day();//��Ľ�λ
		void	date_to_str(DATA_TIME_STRUCT &_dt);//����ת��Ϊddmmyy��ʽ���ַ���
		void	time_to_str(DATA_TIME_STRUCT &_dt);//ʱ��ת��Ϊhhmmss��ʽ���ַ���
		///////����ʱ������صĺ���////////////////////////
		u8		_alarm_enable;
		void	swap(u16 *a,u16 *b);//��������u16�ͱ���������
		u16		day_in_year(DATA_TIME_STRUCT &_dt);//����������ĵڼ���
		u16		days_between_2_date(DATA_TIME_STRUCT &dt_current,DATA_TIME_STRUCT &dt_target);//��������֮���������
		u32		sec_in_day(DATA_TIME_STRUCT &_dt);//ʱ���ǵ���ĵڼ���
		u32		seconds_between_2_time(DATA_TIME_STRUCT &dt1,DATA_TIME_STRUCT &dt2);//����ʱ��֮���������
		void	sec_to_time(u32 sec_source,u8 &hour,u8 &min,u8 &sec);//��ת����hms��ʽ��ʱ��
		///////////////////////////////////////////////////////
		DATA_TIME_STRUCT temp_dt;	//���ڼ��ʱ��仯
};


#endif

