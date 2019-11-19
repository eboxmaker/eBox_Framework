#ifndef __BASICRTC_H
#define __BASICRTC_H

#include "ebox_core.h"
#include "string.h"
#include "ebox_uart.h"

 
 
 
class BasicRtc
{
    public:
        BasicRtc(){};
        virtual void begin()  = 0;
        virtual void loop()  = 0;
    
    
        virtual void write_dt(DateTime_t &dt)  = 0;
        virtual void set_dt(DateTime_t &dt);
        virtual void set_dt_mb(DataU16_t *buf);
        virtual void set_dt_string(String *buf);
        virtual void set_dt_buf(uint8_t *buf);

        virtual DateTime_t get_dt();
        virtual String	get_dt_string();
        virtual String	get_date();
        virtual String	get_time();

        virtual void get_date_only(char *buf);
        virtual void get_time_only(char *buf); 

    
        void dt_check(DateTime_t &dt);
        void print(Uart &uart);

        DateTime_t dateTime;//SSMMHHDDMMYY
        
        

    public:
        bool        is_leap_year(uint16_t _year);
        void        add_one_day(DateTime_t &dt);
        uint8_t     date_max_in_month(uint8_t year,uint8_t month);
        uint16_t    day_in_year(DateTime_t &_dt);
        uint16_t	days_between_2_date(DateTime_t &dt_current, DateTime_t &dt_target);
        uint32_t    sec_in_day(DateTime_t &_dt);
        uint32_t    seconds_between_2_time(DateTime_t &dt1, DateTime_t &dt2);
        void        sec_to_time(uint32_t sec_source, uint8_t &hour, uint8_t &min, uint8_t &sec);
        DateTime_t  date_next_n_days(uint16_t days);
    
        DateTime_t  get_greenwich(uint8_t timezone,DateTime_t &dt);
        uint32_t    get_unix_timestamp(uint8_t timezone,DateTime_t &dt);

        uint8_t time_buf[6];//


    protected:
        uint8_t calculate_week(DateTime_t &t);
        uint8_t bcd_to_dec(uint8_t bcd_code);
        uint8_t dec_to_bcd(uint8_t dec);    
    private:
        void mon_1_3_5_7_8_10_12_add_one_day(DateTime_t &dt);
        void mon_4_6_9_11_add_one_day(DateTime_t &dt);
        void mon_2_add_one_day(DateTime_t &dt);

};

#endif
