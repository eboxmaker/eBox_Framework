#ifndef __BASICRTC_H
#define __BASICRTC_H

#include "ebox_core.h"
#include "string.h"
#include "ebox_uart.h"

extern const unsigned char day_code1[];
extern const unsigned int day_code2[];
extern const unsigned char year_code[];
extern const unsigned char  table_week[]; //月修正数据表
extern const  char  *sky[];//天干
extern const  char  *earth[];//地支
extern const  char  *zodiac_table[12];
extern const  char  *monthcode[];//农历月份
extern const float CENTURY_ARRAY[2][24];
extern const char *JieQiStr[];


#define UTC_BASE_YEAR   1970
#define MONTH_PER_YEAR  12
#define DAY_PER_YEAR    365
#define SEC_PER_DAY     86400
#define SEC_PER_HOUR    3600
#define SEC_PER_MIN     60
 
bool        is_leap_year(uint16_t _year);
uint8_t     calculate_week(DateTime_t &t);



void        sec_to_time(uint32_t sec_source, uint8_t &hour, uint8_t &min, uint8_t &sec);
uint32_t    sec_in_day(DateTime_t &_dt);
uint16_t    day_in_year(DateTime_t &_dt);
uint32_t    seconds_between_2_time(DateTime_t &dt1, DateTime_t &dt2);
uint16_t	days_between_2_date(DateTime_t &dt_current, DateTime_t &dt_target);
uint8_t     get_max_days_in_month(uint16_t year,uint8_t month);

uint32_t    get_unix_timestamp(DateTime_t &dt);
DateTime_t  unix_timestamp_to_dt(uint32_t stamp,uint8_t time_zone = 0);
DateTime_t  get_utc_dt(uint32_t stamp);
DateTime_t  get_utc_dt(DateTime_t &dt,uint8_t time_zone = 0);

void        add_one_day(DateTime_t &dt);
void        mon_1_3_5_7_8_10_12_add_one_day(DateTime_t &dt);
void        mon_4_6_9_11_add_one_day(DateTime_t &dt);
void        mon_2_add_one_day(DateTime_t &dt);
DateTime_t  date_next_n_days(DateTime_t &dt,uint16_t days);
DateTime_t  date_before_n_days(DateTime_t &dt,uint16_t days);
        
class BasicRtc
{
    public:
        BasicRtc(){
            time_zone = 8;
        };
        virtual void    begin()  = 0;
        virtual void    loop()  = 0;
        void            set_dt(DateTime_t &dt);
        void            set_dt_mb(DataU16_t *buf);
        void            set_dt_string(String &buf);
        void            set_dt_string(const char *buf);
        void            set_dt_buf(uint8_t *buf);

        DateTime_t      get_dt();
        String	        get_dt_string();
        String	        get_date_string();
        String	        get_time_string();
        void            get_date_only(char *buf);
        void            get_time_only(char *buf); 

    
        void            dt_check(DateTime_t &dt);
        void            print(Uart &uart);
        void            print(Uart &uart,DateTime_t &dt);

        
        uint8_t         time_buf[6];//SMHDMY
                
    private:
        virtual void        write_dt(DateTime_t &dt)  = 0;
        virtual DateTime_t  read_dt()  = 0;
                  
        uint8_t time_zone;
    protected:
        DateTime_t      dateTime;

};





class ChinaCalendar
{
    public:

        typedef struct 
        {
            uint8_t month;
            uint8_t date;
            String name;
        }Festival_t;

    public:
        ChinaCalendar(){};
            
        DateTime_t update_cdt(DateTime_t &dt);
        DateTime_t get_cdt();
            
        String get_str();
            
        uint8_t get_jieqi_mday(DateTime_t &dt);
        String get_jieqi_str(DateTime_t &dt);
            

        uint8_t get_sky_earth_year(uint16_t year);//获取天干地支年
        uint8_t get_zodiac();//获取生肖索引值0-11
        String get_zodiac_str();//获取生肖字符串

        String get_year_str();
        String get_month_str();
        String get_date_str();
        
        String get_fastival();
        void print(Uart &uart);
    private:
        
        uint8_t GetMoonDay(unsigned char month_p,unsigned short table_addr);
        int spacil_year_offset(DateTime_t dt);
        DateTime_t cdt;
        String cdtStr;
        String solar_terms;
        String zodiac;
    
};
extern const ChinaCalendar::Festival_t c_festival_table[9];

#endif
