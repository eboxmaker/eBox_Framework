#ifndef __DATETIME_H
#define __DATETIME_H

#include "ebox_type.h"
#include "wstring.h"
#include "ebox_uart.h"
#include "TimeSpan.h"

#define UtcOffset 8  //������

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
        SepC,//����
    }TimeSeparatorFormat_t;
    
    //ָ�� System.DateTime �����Ǳ�ʾ����ʱ�䡢Э��ͨ��ʱ�� (UTC)�����ǼȲ�ָ��Ϊ����ʱ�䣬Ҳ��ָ��Ϊ UTC��
    typedef enum 
    {        
        Unspecified = 0,//��ʾ��ʱ���δָ��Ϊ����ʱ�䣬Ҳδָ��ΪЭ��ͨ��ʱ�� (UTC)��
        Utc = 1,//     ��ʾ��ʱ��Ϊ UTC��
        Local = 2//     ��ʾ��ʱ��Ϊ����ʱ�䡣
    }DateTimeKind_t;
    
public:
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
    int milli_second;


    DateTimeClass();
    DateTimeClass(String str,DateTimeKind_t _kind = Local);

    static DateTimeClass parse(String str);

    bool is_leap_year();


    int day_of_year();
    int day_of_week();
    int seconde_of_day();
    int seconde_of_year();
    int minute_of_day();
    void add_months(int value);
    void add_years(int value);
    void add_days(int value);
    void add_hours(int value);
    void add_minutes(int value);
    void add_seconds(int value);
    
    String toString(TimeFormat_t format = YYYY_MM_DD_HH_MM_SS,TimeSeparatorFormat_t gap = Sep1);
    
    DateTimeClass ToUniversalTime();
    double ToTimeStamp();
    TimeSpan  operator-(DateTimeClass& b);
    DateTimeClass  operator+(TimeSpan& b);
    void print(Uart &uart);

private:
    DateTimeKind_t kind;
    void add_one_day();
    
};


#endif

