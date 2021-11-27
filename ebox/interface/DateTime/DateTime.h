#ifndef __DATETIME_H
#define __DATETIME_H

#include "ebox_type.h"
#include "wstring.h"
#include "TimeSpan.h"

#define DATETIME_USE_PRINT true

#if DATETIME_USE_PRINT
    #include "stream.h"
#endif


bool is_leap_year(int year);
int days_in_month(int year,int month);
int days_in_year(int year);
int seconds_in_year(int year);
    

#define LOCAL_UTC_OFFSET 8
class DateTime 
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


    DateTime(int utc_offset = LOCAL_UTC_OFFSET);
    DateTime(uint64_t time_span,int utc_offset = LOCAL_UTC_OFFSET);
    DateTime(String str,int utc_offset = LOCAL_UTC_OFFSET);
    DateTime(String date,String time,int utc_offset = LOCAL_UTC_OFFSET);
    bool parse(uint64_t stamp);
    bool parse(String &str);
    bool parse(String date,String time,int utc_offset = LOCAL_UTC_OFFSET);
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
    void addMilliSeconds(int value);
    
    String toString(TimeFormat_t format = YYYY_MM_DD_HH_MM_SS,TimeSeparatorFormat_t gap = Sep1);
    
    DateTime getUniversalTime();
    double getTimeStamp();
    TimeSpan  operator-(DateTime& b);
    DateTime  operator+(TimeSpan& b);
    DateTime  operator-(TimeSpan& b);
    bool operator<( DateTime &right);
    bool operator>( DateTime &right) { return right < *this; }
    bool operator<=( DateTime &right) { return !(*this > right); }
    bool operator>=( DateTime &right)  { return !(*this < right); }
    bool operator==( DateTime &right) ;

#if DATETIME_USE_PRINT
    void print(Stream &uart);
#endif
    static bool limitCheck(DateTime &dt);

private:
    int utcOffset;
    void add_one_day();

};






#endif

