#ifndef __DATETIME_H
#define __DATETIME_H

#include "ebox_type.h"
#include "wstring.h"
#include "ebox_uart.h"
#include "TimeSpan.h"

#define UtcOffset 8  //东八区

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
        SepC,//汉字
    }TimeSeparatorFormat_t;
    
    //指定 System.DateTime 对象是表示本地时间、协调通用时间 (UTC)，还是既不指定为本地时间，也不指定为 UTC。
    typedef enum 
    {        
        Unspecified = 0,//表示的时间既未指定为本地时间，也未指定为协调通用时间 (UTC)。
        Utc = 1,//     表示的时间为 UTC。
        Local = 2//     表示的时间为本地时间。
    }DateTimeKind_t;
    
public:
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
    int milliSecond;
    int err;


    DateTimeClass();
    DateTimeClass(String str,DateTimeKind_t _kind = Local);

    static DateTimeClass parse(String &str);
    static bool limitCheck(DateTimeClass &dt);
    static DateTimeClass now();

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
    
    DateTimeClass toUniversalTime();
    double toTimeStamp();
    TimeSpan  operator-(DateTimeClass& b);
    DateTimeClass  operator+(TimeSpan& b);
    void print(Uart &uart);

private:
    DateTimeKind_t kind;
    void add_one_day();

};


#endif

