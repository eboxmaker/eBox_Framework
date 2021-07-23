#include "DateTime.h"

const uint8_t days_in_month_table[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};


#define SECONDS_PER_MINUTE  60
#define SECONDS_PER_HOUR    3600
#define SECONDS_PER_DAY     86400
#define SECONDS_PER_YEAR    (365*86400)
#define SECONDS_PER_LEAP_YEAR (366*86400)

#define MINUTE_PER_HOUR (60)
#define MINUTE_PER_DAY  (60*24)

bool IsLeapYear(int year)
{
    if(year % 4 == 0) //必须能被4整除
    {
        if(year % 100 == 0)
        {
            if(year % 400 == 0)return true; //如果以00结尾,还要能被400整除
            else return false;
        }
        else return true;
    }
    else return false;
}
int days_in_month(int year,int month)
{
    if(month == 2 && IsLeapYear(year))
    {
         return  29;
    }
    else
    {
        return days_in_month_table[month - 1];
    }
}
int days_in_year(int year)
{
    return IsLeapYear(year)?366:365;
}

int seconds_in_year(int year)
{
    return IsLeapYear(year)?SECONDS_PER_YEAR:SECONDS_PER_LEAP_YEAR;
}

DateTimeClass::DateTimeClass():
kind(Local)
{

}

DateTimeClass::DateTimeClass(String str,DateTimeKind_t _kind):
kind(Local)
{
  *this = parse(str);
}
DateTimeClass DateTimeClass::parse(String str)
{
    DateTimeClass dt;
    String temp[10];
    int seek = 0;
    int last = 0;
    seek = str.indexOf("-",last);
    temp[0] = str.substring(last,seek);
    last = seek + 1;
    
    seek = str.indexOf("-",last);
    temp[1] = str.substring(last,seek);
    last = seek + 1;

    seek = str.indexOf(" ",last);
    temp[2] = str.substring(last,seek);
    last = seek + 1;
    
    seek = str.indexOf(":",last);
    temp[3] = str.substring(last,seek);
    last = seek + 1;

    seek = str.indexOf(":",last);
    temp[4] = str.substring(last,seek);
    last = seek + 1;

    temp[5] = str.substring(last);
    
    uint16_t y = temp[0].toInt();

    dt.year = y;
    dt.month = temp[1].toInt();
    dt.day = temp[2].toInt();
    dt.hour = temp[3].toInt();
    dt.minute = temp[4].toInt();
    dt.second = temp[5].toInt();
    return dt;
}
String DateTimeClass::toString(TimeFormat_t format ,TimeSeparatorFormat_t gap)
{
    String str;
    char buf[32];
    const char* snap[][6]= {
        {"-","-"," ",":",":",""},
        {"/","/"," ",":",":",""},
        {"年","月","日","时","分","秒"},
        };
    bool am;
    int newHour;
    switch((int)format)
    {
        case YYYY_MM_DD_HH_MM_SS:
            snprintf(buf,32,"%04d%s%02d%s%02d%s%02d%s%02d%s%02d%s",year,snap[gap][0],month,snap[gap][1],day,snap[gap][2],\
                        hour,snap[gap][3],minute,snap[gap][4],second,snap[gap][5]);break;
        case YYYY_MM_DD_H12_MM_SS:

        case YY_MM_DD_HH_MM_SS:
            snprintf(buf,32,"%02d%s%02d%s%02d%s%02d%s%02d%s%02d%s",year%100,snap[gap][0],month,snap[gap][1],day,snap[gap][2],\
                        hour,snap[gap][3],minute,snap[gap][4],second,snap[gap][5]);break;
        case YY_MM_DD_H12_MM_SS:
            am = (hour <= 12)?true:false;
            if(am)
            {
                snprintf(buf,32,"%02d%s%02d%s%02d%s%02d%s%02d%s%02d%s",year%100,snap[gap][0],month,snap[gap][1],day,snap[gap][2],\
                            hour,snap[gap][3],minute,snap[gap][4],second,snap[gap][5]);break;
            }
            else
            {
                newHour = hour - 12;
                snprintf(buf,32,"%04d%s%02d%s%02d%s%02d%s%02d%s%02d%s",year%100,snap[gap][0],month,snap[gap][1],day,snap[gap][2],\
                            newHour,snap[gap][3],minute,snap[gap][4],second,snap[gap][5]);break;
            }
        case YY_MM_DD:
            snprintf(buf,32,"%02d%s%02d%s%02d%s",year%100,snap[gap][0],month,snap[gap][1],day,snap[gap][2]);break;
        case YYYY_MM_DD:
            snprintf(buf,32,"%04d%s%02d%s%02d%s",year,snap[gap][0],month,snap[gap][1],day,snap[gap][2]);break;
        case HH_MM_SS:
            snprintf(buf,32,"%02d%s%02d%s%02d%s",hour,snap[gap][3],minute,snap[gap][4],second,snap[gap][5]);break;
        case H12_MM_SS:
            am = (hour <= 12)?true:false;
            if(am)
            {
                snprintf(buf,32,"%02d%s%02d%s%02d%s AM",hour,snap[gap][3],minute,snap[gap][4],second,snap[gap][5]);break;
            }
            else
            {
                newHour = hour - 12;
                snprintf(buf,32,"%02d%s%02d%s%02d%s",newHour,snap[gap][3],minute,snap[gap][4],second,snap[gap][5]);break;
            }
        default :
            break;
    }
    str = buf;
    return str;
}

void DateTimeClass::print(Uart &uart)
{
    uart.printf("%04d-%02d-%02d %02d:%02d:%02d week:%d\n",year,month,day,\
        hour,minute,second,day_of_week());
}

bool DateTimeClass::is_leap_year()
{
    if(year % 4 == 0) //必须能被4整除
    {
        if(year % 100 == 0)
        {
            if(year % 400 == 0)return true; //如果以00结尾,还要能被400整除
            else return false;
        }
        else return true;
    }
    else return false;
}

int DateTimeClass::day_of_year()
{
    int days = day;

    for(uint8_t i = 0; i < month - 1; ++i)
    {
        days += days_in_month_table[i];
    }
    if(is_leap_year()  && month > 2)
    {
        days -= 1;
    }
    return days;
}
int DateTimeClass::day_of_week()
{
    int week;
    uint16_t _year = year;
    uint8_t _month = month;
    if(_month == 1 || _month == 2)
    {
        _month += 12;
        _year--;
    }
    week = (day + 2 * _month + 3 * (_month + 1) / 5 + _year + _year / 4 - _year / 100 + _year / 400) % 7 + 1;
    return week;
}
int DateTimeClass::seconde_of_day()
{
    return hour * 3600 + minute * 60 + second;
}
int DateTimeClass::seconde_of_year()
{
    int days = day_of_year() - 1;
    return days * 86400 + seconde_of_day();
}

int DateTimeClass::minute_of_day()
{
    return hour * 60 + minute ;

}
void DateTimeClass::add_years(int value)
{
    year += value;
}
void DateTimeClass::add_months(int value)
{
    month = month + value;
    if(month > 12)
    {
        year += month/12;
        month = month%12;
    }
    else if(month <= 0)
    {
        while(month <= 0)
        {
            month += 12;
            year--;
        }
    }
}

void DateTimeClass::add_days(int value)
{
    day = day + value;
    if(day > days_in_month(year,month))
    {
        while(day > days_in_month(year,month))
        {
            day -= days_in_month(year,month);
            add_months(1);
        }
    }
    else if(day <= 0)
    {
        while(day <= 0)
        {
            add_months(-1);
            day += days_in_month(year,month);
        }
    }

}
void DateTimeClass::add_hours(int value)
{
    int days_to_add = (hour + value) / 24;
    hour = (hour + value) % 24;
    if(hour < 0) {hour += 24;days_to_add -= 1;}
    add_days(days_to_add);
}
void DateTimeClass::add_minutes(int value)
{
    int hours_to_add = (minute + value) / 60;
    minute = (minute + value) % 60;
    if(minute < 0) {minute += 60;hours_to_add -= 1;}
    add_hours(hours_to_add);
}
void DateTimeClass::add_seconds(int value)
{
    int minute_to_add = (second + value) / 60;
    second = (second + value) % 60;
    if(second < 0){ second += 60;minute_to_add -= 1;}
    add_minutes(minute_to_add);
}
    
DateTimeClass DateTimeClass::ToUniversalTime()
{
    DateTimeClass dt = *this;
    dt.kind = Utc;
    dt.add_hours(-UtcOffset);
    return dt;
}
double DateTimeClass::ToTimeStamp()
{
    DateTimeClass dtStart("1970-1-1 0:0:0",Utc);
    DateTimeClass dt = this->ToUniversalTime();
    TimeSpan ts = dt - dtStart;
    return ts.total_seconds;
}



TimeSpan DateTimeClass::operator-(DateTimeClass& b)
{
    TimeSpan ts;
    int remainderSec ;
    
    if(this->year == b.year)
    {
        int remainderSec = this->seconde_of_year() - b.seconde_of_year();
        ts.days = remainderSec / 86400;remainderSec  %= 86400; 
        ts.hours = remainderSec / 3600;remainderSec  %= 3600; 
        ts.minutes = remainderSec / 60;remainderSec  %= 60; 
        ts.seconds = remainderSec;
    }else
    {
        int yearMin,yearMax;
        int flag = 0;
        if(this->year < b.year)
        {
            yearMin = this->year;
            yearMax = b.year;
            ts.days = days_in_year(this->year) - this->day_of_year();
            ts.days += b.day_of_year();
            flag = 1;
        }
        else
        {
            yearMax = this->year;
            yearMin = b.year;
            ts.days = days_in_year(b.year) - b.day_of_year();
            ts.days += this->day_of_year();

        }
        for(int i = yearMin + 1 ; i < yearMax ; i++)
        {
            if(IsLeapYear(i))
                ts.days += 366;
            else 
                ts.days += 365;
        }
        
        if(flag)
            ts.days = -ts.days;
        
        int secs1 = this->seconde_of_day();
        int secs2 = b.seconde_of_day();
        remainderSec = secs1 - secs2;
        if(ts.days < 0 && remainderSec > 0)
        {
            ts.days += 1;
            remainderSec -= SECONDS_PER_DAY;
        }
        if(ts.days > 0 && remainderSec < 0)
        {
            ts.days -= 1;
            remainderSec += SECONDS_PER_DAY;
        }
        ts.hours = remainderSec / 3600; remainderSec  %= 3600; 
        ts.minutes = remainderSec / 60;remainderSec  %= 60; 
        ts.seconds = remainderSec;
    }
    
    ts.total_days = ts.days + (ts.hours*3600 + ts.minutes*60 + ts.seconds)/(double)SECONDS_PER_DAY;
    ts.total_hours = (ts.days * 24) + ts.hours + (ts.minutes*60 + ts.seconds)/(double)SECONDS_PER_HOUR;
    ts.total_minutes = (double)ts.days * MINUTE_PER_DAY + ts.hours * MINUTE_PER_HOUR + ts.minutes + ts.seconds/(double)SECONDS_PER_MINUTE;
    ts.total_seconds = (double)ts.days * SECONDS_PER_DAY + ts.hours * SECONDS_PER_HOUR + ts.minutes*SECONDS_PER_MINUTE + ts.seconds;
    return ts;
};

DateTimeClass  DateTimeClass::operator+(TimeSpan& b)
{
    DateTimeClass dt = *this;
    dt.add_days(b.days);
    dt.add_hours(b.hours);
    dt.add_minutes(b.minutes);
    dt.add_seconds(b.seconds);
    return dt;
}
