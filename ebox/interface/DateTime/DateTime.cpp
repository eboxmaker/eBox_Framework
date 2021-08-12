#include "DateTime.h"
#include "ebox_core.h"
const uint8_t days_in_month_table[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};


#define SECONDS_PER_MINUTE  60
#define SECONDS_PER_HOUR    3600
#define SECONDS_PER_DAY     86400
#define SECONDS_PER_YEAR    (365*86400)
#define SECONDS_PER_LEAP_YEAR (366*86400)

#define MINUTES_PER_HOUR (60)
#define MINUTES_PER_DAY  (60*24)

#define HOURS_PER_DAY  (24)

bool is_leap_year(int year)
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
    if(month == 2 && is_leap_year(year))
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
    return is_leap_year(year)?366:365;
}

int seconds_in_year(int year)
{
    return is_leap_year(year)?SECONDS_PER_YEAR:SECONDS_PER_LEAP_YEAR;
}
int find_uint_from_string(String &str,int seek,uint32_t *value)
{
    char buf[64];
    uint32_t val = 0;
    if(str.length() > 64) return -1;
    memcpy(buf,str.c_str(),64);
    int index = seek;
    while(1)
    {
        if(isDigit(buf[index])){break;}//如果是数字则进入下一循环
        else{//否则加一
            index++;
            if(index == 64) return -1;
        }
    }
    while(1)
    {
        if(isDigit(buf[index])){
            val = val * 10 + buf[index++] - '0';
        }
        else{
            *value = val;
            return index;
        }
    }
//    return -1;
}

DateTimeClass::DateTimeClass(int utc_offset)
{
    utcOffset = utc_offset%24;
    err = 0;
}

DateTimeClass::DateTimeClass(String str,int utc_offset)
{
    utcOffset = utc_offset%24;
    err = 0;
    if(!parse(str))
    {
        err = -1;
    }

}
/*
    DateTimeClass dt((__DATE__), (__TIME__),8);
    @param date string, e.g. ("Apr 16 2020").
    @param time string, e.g. ("18:34:56").
*/
DateTimeClass::DateTimeClass(String date,String time,int utc_offset)
{
    utcOffset = utc_offset%24;
    err = 0;

    char buff[11];
    memcpy(buff, date.c_str(), 11);
    year = atoi(buff + 7);
    // Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec
    switch (buff[0]) {
        case 'J':
            month = (buff[1] == 'a') ? 1 : ((buff[2] == 'n') ? 6 : 7);
            break;
        case 'F':
            month = 2;
            break;
        case 'A':
            month = buff[2] == 'r' ? 4 : 8;
            break;
        case 'M':
            month = buff[2] == 'r' ? 3 : 5;
            break;
        case 'S':
            month = 9;
            break;
        case 'O':
            month = 10;
            break;
        case 'N':
            month = 11;
            break;
        case 'D':
            month = 12;
            break;
        default:
          err = -1;
          break;
    }
    day = atoi(buff + 4);
    memset(buff,0,11);
    memcpy(buff, time.c_str(), 8);
    hour = atoi(buff);
    minute = atoi(buff + 3);
    second = atoi(buff + 6);

}



bool DateTimeClass::parse(String &str)
{
    uint32_t value[10];
    int seek = 0;
    int k = 0;
    while(seek >= 0)
    {
        seek = find_uint_from_string(str,seek,&value[k++]);
        if(k == 6) break;
    }
    if(k != 6)
    {
        return false;
    }
    year = value[0];
    month = value[1];
    day = value[2];
    hour = value[3];
    minute = value[4];
    second = value[5];
    DateTimeClass::limitCheck(*this);
    return true;
}
bool DateTimeClass::limitCheck(DateTimeClass &dt)
{
    bool ret;
    if(dt.second < 0)       {dt.second = 0;ret = false;}
    else if(dt.second > 59) {dt.second = 59;ret = false;}
    if(dt.minute < 0)       {dt.minute = 0;ret = false;}
    else if(dt.minute > 59) {dt.minute = 59;ret = false;}
    if(dt.hour < 0)         {dt.hour = 0;ret = false;}
    else if(dt.hour > 23)   {dt.hour = 23;ret = false;}

    if(dt.year < 0)         {dt.year = 0;ret = false;}
    else if(dt.year > 3000) {dt.year = 3000;ret = false;}
    if(dt.month < 1)        {dt.month = 1;ret = false;}
    else if(dt.month > 12)  {dt.month = 12;ret = false;}
    
    if(dt.day < 1)          {dt.day = 1;ret = false;}
    else if(dt.day > days_in_month(dt.year,dt.month)) {dt.day = days_in_month(dt.year,dt.month);ret = false;}
    if(ret == false) dt.err = -1;
    return ret;
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
        hour,minute,second,dayOfWeek());
}

bool DateTimeClass::isLeapYear()
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

int DateTimeClass::dayOfYear()
{
    int days = day;

    for(uint8_t i = 0; i < month - 1; ++i)
    {
        days += days_in_month_table[i];
    }
    if(isLeapYear()  && month > 2)
    {
        days -= 1;
    }
    return days;
}
int DateTimeClass::dayOfWeek()
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
int DateTimeClass::secondeOfDay()
{
    return hour * 3600 + minute * 60 + second;
}
int DateTimeClass::secondeOfYear()
{
    int days = dayOfYear() - 1;
    return days * 86400 + secondeOfDay();
}

int DateTimeClass::minuteOfDay()
{
    return hour * 60 + minute ;

}
void DateTimeClass::addYears(int value)
{
    year += value;
}
void DateTimeClass::addMonths(int value)
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

void DateTimeClass::addDays(int value)
{
    day = day + value;
    if(day > days_in_month(year,month))
    {
        while(day > days_in_month(year,month))
        {
            day -= days_in_month(year,month);
            addMonths(1);
        }
    }
    else if(day <= 0)
    {
        while(day <= 0)
        {
            addMonths(-1);
            day += days_in_month(year,month);
        }
    }

}
void DateTimeClass::addHours(int value)
{
    int days_to_add = (hour + value) / 24;
    hour = (hour + value) % 24;
    if(hour < 0) {hour += 24;days_to_add -= 1;}
    addDays(days_to_add);
}
void DateTimeClass::addMinutes(int value)
{
    int hours_to_add = (minute + value) / 60;
    minute = (minute + value) % 60;
    if(minute < 0) {minute += 60;hours_to_add -= 1;}
    addHours(hours_to_add);
}
void DateTimeClass::addSeconds(int value)
{
    int minute_to_add = (second + value) / 60;
    second = (second + value) % 60;
    if(second < 0){ second += 60;minute_to_add -= 1;}
    addMinutes(minute_to_add);
}
    
DateTimeClass DateTimeClass::getUniversalTime()
{
    DateTimeClass dt = *this;
    dt.utcOffset -= utcOffset;
    dt.addHours(-utcOffset);
    return dt;
}
double DateTimeClass::getTimeStamp()
{
    DateTimeClass dtStart("1970-1-1 0:0:0",(int)0);
    DateTimeClass dt = this->getUniversalTime();
    TimeSpan ts = dt - dtStart;
    return ts.total_seconds;
}



TimeSpan DateTimeClass::operator-(DateTimeClass& b)
{
    TimeSpan ts;
    
    DateTimeClass dtThis = getUniversalTime();
    DateTimeClass dtRight =  b.getUniversalTime();
    int remainderSec ;
    
    if(dtThis.year == dtRight.year)
    {
        int remainderSec = dtThis.secondeOfYear() - dtRight.secondeOfYear();
        ts.days = remainderSec / 86400;remainderSec  %= 86400; 
        ts.hours = remainderSec / 3600;remainderSec  %= 3600; 
        ts.minutes = remainderSec / 60;remainderSec  %= 60; 
        ts.seconds = remainderSec;
    }else
    {
        int yearMin,yearMax;
        int flag = 0;
        if(dtThis.year < dtRight.year)
        {
            yearMin = dtThis.year;
            yearMax = dtRight.year;
            ts.days = days_in_year(dtThis.year) - dtThis.dayOfYear();
            ts.days += dtRight.dayOfYear();
            flag = 1;
        }
        else
        {
            yearMax = dtThis.year;
            yearMin = dtRight.year;
            ts.days = days_in_year(dtRight.year) - dtRight.dayOfYear();
            ts.days += dtThis.dayOfYear();

        }
        for(int i = yearMin + 1 ; i < yearMax ; i++)
        {
            if(is_leap_year(i))
                ts.days += 366;
            else 
                ts.days += 365;
        }
        
        if(flag)
            ts.days = -ts.days;
        
        int secs1 = dtThis.secondeOfDay();
        int secs2 = dtRight.secondeOfDay();
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
        ts.minutes = remainderSec % 60; 
        ts.seconds = remainderSec;
    }
    
    ts.total_seconds = (double)ts.days * SECONDS_PER_DAY + (double)ts.hours * SECONDS_PER_HOUR + (double)ts.minutes*SECONDS_PER_MINUTE + (double)ts.seconds;
    ts.total_minutes = ts.seconds/SECONDS_PER_MINUTE;
    ts.total_hours   =  ts.seconds/SECONDS_PER_HOUR;
    ts.total_days    = ts.seconds/SECONDS_PER_DAY;
    
    return ts;
};

DateTimeClass  DateTimeClass::operator+(TimeSpan& b)
{
    DateTimeClass dt = *this;
    dt.addDays(b.days);
    dt.addHours(b.hours);
    dt.addMinutes(b.minutes);
    dt.addSeconds(b.seconds);
    return dt;
}
DateTimeClass  DateTimeClass::operator-(TimeSpan& b)
{
    DateTimeClass dt = *this;
    dt.addDays(-b.days);
    dt.addHours(-b.hours);
    dt.addMinutes(-b.minutes);
    dt.addSeconds(-b.seconds);
    return dt;
}
/**************************************************************************/
/*!
    @author Anton Rieutskyi
    @brief  Test if one DateTime is less (earlier) than another.
    @warning if one or both DateTime objects are invalid, returned value is
        meaningless
    @see use `isValid()` method to check if DateTime object is valid
    @param right Comparison DateTime object
    @return True if the left DateTime is earlier than the right one,
        false otherwise.
*/
/**************************************************************************/
bool DateTimeClass::operator<( DateTimeClass &right)  {
    DateTimeClass dtThis = getUniversalTime();
    DateTimeClass dtRight =  right.getUniversalTime();
    TimeSpan ts = dtThis - dtRight ;
    if(ts.total_seconds < 0)
        return true;
    else
        return false;
}

/**************************************************************************/
/*!
    @author Anton Rieutskyi
    @brief  Test if two DateTime objects are equal.
    @warning if one or both DateTime objects are invalid, returned value is
        meaningless
    @see use `isValid()` method to check if DateTime object is valid
    @param right Comparison DateTime object
    @return True if both DateTime objects are the same, false otherwise.
*/
/**************************************************************************/
bool DateTimeClass::operator==(DateTimeClass &right) {
    DateTimeClass dtThis = getUniversalTime();
    DateTimeClass dtRight =  right.getUniversalTime();
    return (dtRight.year == dtThis.year && dtRight.month == dtThis.month &&
            dtRight.day == dtThis.day && dtRight.hour == dtThis.hour && dtRight.minute == dtThis.minute &&
            dtRight.second == dtThis.second);
}


TimeSpan::TimeSpan(int64_t ticks)
{
    total_seconds   = ticks;
    total_minutes   = total_seconds/(double)SECONDS_PER_MINUTE;
    total_hours     =  total_seconds/(double)SECONDS_PER_HOUR;
    total_days      = total_seconds/(double)SECONDS_PER_DAY;

    seconds = ticks % 60;
    minutes = (ticks  % 3600) / 60; 
    hours   = (ticks  % SECONDS_PER_DAY) / 3600; 
    days    = ticks / SECONDS_PER_DAY;
    

}
TimeSpan::TimeSpan(int _hours, int _minutes, int _seconds)
{
    total_seconds   = (int64_t)_hours * SECONDS_PER_HOUR + (int64_t)_minutes * SECONDS_PER_MINUTE + (int64_t)_seconds;
    total_minutes   = total_seconds/(double)SECONDS_PER_MINUTE;
    total_hours     =  total_seconds/(double)SECONDS_PER_HOUR;
    total_days      = total_seconds/(double)SECONDS_PER_DAY;
    
    seconds = (int64_t) total_seconds % 60;
    minutes = ((int64_t)total_seconds % SECONDS_PER_HOUR) / 60; 
    hours   = ((int64_t)total_seconds % SECONDS_PER_DAY) / 3600; 
    days    = (int64_t) total_seconds / SECONDS_PER_DAY;
    

}

TimeSpan::TimeSpan(int _days, int _hours, int _minutes, int _seconds)
{

    total_seconds   = (int64_t)_days*SECONDS_PER_DAY + (int64_t)_hours * SECONDS_PER_HOUR + (int64_t)_minutes * SECONDS_PER_MINUTE + (int64_t)_seconds;
    total_minutes   = total_seconds/(double)SECONDS_PER_MINUTE;
    total_hours     = total_seconds/(double)SECONDS_PER_HOUR;
    total_days      = total_seconds/(double)SECONDS_PER_DAY;
    
    seconds = (int64_t) total_seconds % 60;
    minutes = ((int64_t)total_seconds % SECONDS_PER_HOUR) / 60; 
    hours   = ((int64_t)total_seconds % SECONDS_PER_DAY) / 3600; 
    days    = (int64_t) total_seconds / SECONDS_PER_DAY;
}


void TimeSpan::print(Uart& uart)
{
    uart.printf("Days:%d\nHours:%d\nMinutes:%d\nSeconds:%d\n",\
                days,hours,minutes,seconds);
    
    uart.printf("TotalDays :%0.5f\n",total_days);
    uart.printf("TotalHours:%0.5f\n",total_hours);
    uart.printf("TotalMinutes:%0.5f\n",total_minutes);
    uart.printf("TotalSecs:%0.5f\n",total_seconds);
    uart.flush();
}

