#ifndef __DATETIME_H
#define __DATETIME_H

#include "ebox_type.h"
#include "wstring.h"
#include "ebox_uart.h"
#include "TimeSpan.h"

#define LOCAL_UTC_OFFSET 8
class DateTimeClass 
{
public:
    typedef enum {
        YYYY_MM_DD_HH_MM_SS,
        YYYY_MM_DD_H12_MM_SS,
        YY_MM_DD_HH_MM_SS,
        YY_MM_DD_H12_MM_SS,
        YY_MM_DD,
        YYYY_MM_DD,
        HH_MM_SS,
        H12_MM_SS,
    }TimeFormat_t;
    typedef enum {
        Sep1,//--- ::
        Sep2,// //::
        SepC,//ºº×Ö
    }TimeSeparatorFormat_t;
    
    
public:
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
    int milliSecond;
    int err;


    DateTimeClass(int utc_offset = LOCAL_UTC_OFFSET);
    DateTimeClass(String str,int utc_offset = LOCAL_UTC_OFFSET);
    DateTimeClass(String date,String time,int utc_offset = LOCAL_UTC_OFFSET);

    bool parse(String &str);
    bool isLeapYear();


    int dayOfYear();
    int dayOfWeek();
    int secondeOfDay();
    int secondeOfYear();
    int minuteOfDay();
    void addMonths(int value);
    void addYears(int value);
    void addDays(int value);
    void addHours(int value);
    void addMinutes(int value);
    void addSeconds(int value);
    
    String toString(TimeFormat_t format = YYYY_MM_DD_HH_MM_SS,TimeSeparatorFormat_t gap = Sep1);
    
    DateTimeClass getUniversalTime();
    double getTimeStamp();
    TimeSpan  operator-(DateTimeClass& b);
    DateTimeClass  operator+(TimeSpan& b);
    DateTimeClass  operator-(TimeSpan& b);
    bool operator<( DateTimeClass &right);
    bool operator>( DateTimeClass &right) { return right < *this; }
    bool operator<=( DateTimeClass &right) { return !(*this > right); }
    bool operator>=( DateTimeClass &right)  { return !(*this < right); }
    bool operator==( DateTimeClass &right) ;

    void print(Uart &uart);
    static bool limitCheck(DateTimeClass &dt);

private:
    int utcOffset;
    void add_one_day();

};






#endif

