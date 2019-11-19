#include "basicRtc.h"

void BasicRtc::set_dt(DateTime_t &dt)
{
    write_dt(dt);
}
void BasicRtc::set_dt_mb(DataU16_t *buf)
{
    DateTime_t dt;

    dt.year = buf[2].byte[1];
    dt.month = buf[2].byte[0];
    dt.date = buf[1].byte[1];
    dt.hour = buf[1].byte[0];
    dt.min = buf[0].byte[1];
    dt.sec = buf[0].byte[0];
    write_dt(dt);
}
void BasicRtc::set_dt_string(String *buf)
{
    DateTime_t dt;
    
    
    write_dt(dt);
}
void BasicRtc::set_dt_buf(uint8_t *buf)
{
    DateTime_t dt;

    int i = 0; 
    dt.sec = buf[i++];
    dt.min = buf[i++];
    dt.hour = buf[i++];
    dt.date = buf[i++];
    dt.month = buf[i++];
    dt.year = buf[i++];
    
    write_dt(dt);

}
DateTime_t BasicRtc::get_dt()
{
    return dateTime;
}
String	BasicRtc::get_dt_string()
{
    String dt_str;
    char buf[40];
    sprintf(buf,"20%02d-%02d-%02d %02d:%02d:%02d",dateTime.year,dateTime.month,dateTime.date,\
        dateTime.hour,dateTime.min,dateTime.sec);
    dt_str = buf;
    return dt_str;

}
String	BasicRtc::get_date()
{
    String dt_str;
    char buf[20];
    sprintf(buf,"20%02d-%02d-%02d",dateTime.year,dateTime.month,dateTime.date);
    dt_str = buf;
    return dt_str;

}
String	BasicRtc::get_time()
{
    String dt_str;
    char buf[20];
    sprintf(buf,"%02d:%02d:%02d",dateTime.hour,dateTime.min,dateTime.sec);
    dt_str = buf;
    return dt_str;

}



void BasicRtc::get_date_only(char *buf)
{
    buf[0] = uint8_t( (time_buf[0] % 10) + 0x30);
    buf[1] = uint8_t( (time_buf[0] / 10) + 0x30);
    buf[2] = uint8_t( (time_buf[1] % 10) + 0x30);
    buf[3] = uint8_t( (time_buf[1] / 10) + 0x30);
    buf[4] = uint8_t( (time_buf[2] % 10) + 0x30);
    buf[5] = uint8_t( (time_buf[2] / 10) + 0x30);
    buf[6] = '\0';
}
void BasicRtc::get_time_only(char *buf)
{

    buf[0] = uint8_t( (time_buf[3] % 10) + 0x30);
    buf[1] = uint8_t( (time_buf[3] / 10) + 0x30);
    buf[2] = uint8_t( (time_buf[4] % 10) + 0x30);
    buf[3] = uint8_t( (time_buf[4] / 10) + 0x30);
    buf[4] = uint8_t( (time_buf[5] % 10) + 0x30);
    buf[5] = uint8_t( (time_buf[5] / 10) + 0x30);
    buf[6] = '\0';


}
void BasicRtc::dt_check(DateTime_t &dt)
{
    uint8_t max_date;
    limit(dt.sec,(uint8_t)0,(uint8_t)59);
    limit(dt.min,(uint8_t)0,(uint8_t)59);
    limit(dt.hour,(uint8_t)0,(uint8_t)23);
    limit(dt.date,(uint8_t)1,(uint8_t)31);
    limit(dt.month,(uint8_t)1,(uint8_t)12);
    limit(dt.year,(uint8_t)0,(uint8_t)99);
    max_date = date_max_in_month(dt.year,dt.month);
    if(dt.date >  max_date)
    {
        dt.date = max_date;
    }
    calculate_week(dt);

}

void BasicRtc::print(Uart &uart)
{
    uart.printf("=========system date time==========\r\n");
    uart.printf("20%02d-%02d-%02d %02d:%02d:%02d weed:%d\r\n",dateTime.year,dateTime.month,dateTime.date,dateTime.hour,dateTime.min,dateTime.sec,dateTime.week);
    uart.printf("===================================\r\n");

}

//使用基姆拉尔森计算公式
//把一月和二月看成上一年的十三月和十四月
uint8_t BasicRtc::calculate_week(DateTime_t &dt)
{
    uint8_t week;
    uint16_t year = dt.year + 2000;
    uint8_t month = dt.month;
    if(month == 1 || month == 2)
    {
        month += 12;
        year--;
    }
    dt.week = (dt.date + 2 * month + 3 * (month + 1) / 5 + year + year / 4 - year / 100 + year / 400) % 7 + 1;
    return dt.week;
}
bool BasicRtc::is_leap_year(uint16_t _year)
{
    if(_year % 4 == 0) //必须能被4整除
    {
        if(_year % 100 == 0)
        {
            if(_year % 400 == 0)return 1; //如果以00结尾,还要能被400整除
            else return false;
        }
        else return true;
    }
    else return false;
}


void BasicRtc::add_one_day(DateTime_t &dt)
{
    switch(dt.month)
    {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            mon_1_3_5_7_8_10_12_add_one_day(dt);
            break;

        case 4:
        case 6:
        case 9:
        case 11:
            mon_4_6_9_11_add_one_day(dt);
            break;

        case 2:
            mon_2_add_one_day(dt);
            break;
    }
    if(dt.month > 12)
    {
        dt.year += 1;
        dt.month -= 12;
    }
    calculate_week(dt);
}
///////以下是日历计算的私有函数/////////////////////////////////////////////
void BasicRtc::mon_1_3_5_7_8_10_12_add_one_day(DateTime_t &dt)
{
    dt.date += 1;
    if(dt.date > 31)
    {
        dt.date -= 31;
        dt.month += 1;
    }
}
void BasicRtc::mon_4_6_9_11_add_one_day(DateTime_t &dt)
{
    dt.date += 1;
    if(dt.date > 30)
    {
        dt.date -= 30;
        dt.month += 1;
    }
}
void BasicRtc::mon_2_add_one_day(DateTime_t &dt)
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
uint8_t BasicRtc::date_max_in_month(uint8_t year,uint8_t month)
{
    bool flag; 
    
    switch(month)
    {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            return 31;
        case 4:
        case 6:
        case 9:
        case 11:
            return 30;
        case 2:
            if(is_leap_year(year))
            {
                return 29;
            }
            else
            {
                return 28;
            }
    }
}


//根据给定的日期算出他是该年的第几天
//1月1号算第一天
uint16_t BasicRtc::day_in_year(DateTime_t &_dt)
{
    uint16_t temp_year;
    uint16_t temp_month;
    uint16_t temp_day;

    temp_year 	= _dt.year + 2000;
    temp_month	= _dt.month;
    temp_day    = _dt.date;


    uint8_t DAY[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if(is_leap_year(temp_year))
        DAY[1] = 29;
    for(uint8_t i = 0; i < temp_month - 1; ++i)
    {
        temp_day += DAY[i];
    }
    return temp_day;
}
uint16_t	BasicRtc::days_between_2_date(DateTime_t &dt_current, DateTime_t &dt_target)
{
    uint16_t temp_year_c, temp_year_t;
    uint16_t temp_month_c, temp_month_t;
    uint16_t temp_day_c, temp_day_t;

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
        uint16_t c_day, t_day;
        c_day = day_in_year(dt_current);
        t_day = day_in_year(dt_target);
        return t_day > c_day ? t_day - c_day : c_day - t_day;

    }
    else
    {
        if(temp_year_c > temp_year_t)
        {
            swap(&temp_year_c, &temp_year_t);
            swap(&temp_month_c, &temp_month_t);
            swap(&temp_day_c, &temp_day_t);
        }
        uint16_t d1, d2, d3;
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
        return d1 + d2 + d3;
    }
}
uint32_t	BasicRtc::sec_in_day(DateTime_t &_dt)
{
    return _dt.hour * 3600 + _dt.min * 60 + _dt.sec;
}
uint32_t BasicRtc::seconds_between_2_time(DateTime_t &dt1, DateTime_t &dt2)
{
    uint32_t temp_sec1, temp_sec2;

    temp_sec1 = sec_in_day(dt1);
    temp_sec2 = sec_in_day(dt2);
    if(temp_sec1 > temp_sec2)
        return temp_sec1 - temp_sec2;
    else
        return temp_sec2 - temp_sec1;
}
void BasicRtc::sec_to_time(uint32_t sec_source, uint8_t &hour, uint8_t &min, uint8_t &sec)
{
    hour	= sec_source / 3600;
    min		= (sec_source % 3600) / 60;
    sec		= (sec_source % 3600) % 60;
}
DateTime_t  BasicRtc::date_next_n_days(uint16_t days)
{
    DateTime_t dtTarget = dateTime;;
    
    for(int i = 0; i < days; i++)
    {
        add_one_day(dtTarget);
    }
    return dtTarget;
}

DateTime_t  BasicRtc::get_greenwich(uint8_t timezone,DateTime_t &_dt)
{
    DateTime_t dt = _dt;
//    dt.hour += timezone;
//    if(dt.hour >= 24)
//    {
//        dt.hour -= 24;
//        add_one_day(dt);
//    }
//    calculate_week(dt);
    return dt;
}

uint32_t BasicRtc::get_unix_timestamp(uint8_t timezone,DateTime_t &dt)
{
//    DateTime_t gw_dt = get_greenwich(timezone,dt);
    DateTime_t dt0 = {0};
    uint32_t timestamp = 946656000;
    uint16_t diff_days;
    diff_days = days_between_2_date(dt0, dt);

    timestamp += seconds_between_2_time( dt0, dt) + (diff_days - 1) * 3600 * 24; //两个时间之间的秒数差;
//    timestamp -= timezone * 3600;
    return timestamp;
}













uint8_t BasicRtc::bcd_to_dec(uint8_t bcd_code)
{
    uint8_t temp, dec;
    temp = (bcd_code >> 4) * 10;
    dec = (bcd_code & 0x0f) + temp;
    return dec;
}
uint8_t BasicRtc::dec_to_bcd(uint8_t dec)
{
    uint8_t temp, temp2, bcd;
    temp = dec / 10;
    temp2 = dec % 10;

    bcd =  (temp << 4) + temp2;

    return bcd;
}