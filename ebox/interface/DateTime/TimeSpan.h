#ifndef __TIMESPAN_H
#define __TIMESPAN_H

#include "ebox_type.h"
class TimeSpan
{
public :
        //
        // 摘要:
        //     将 TimeSpan 结构的新实例初始化为指定的刻度数。
        // 参数:
        //   ticks:以秒为单位表示的时间段。
        TimeSpan(){};
        TimeSpan(int64_t seconds);
        //
        // 摘要:
        //     将 TimeSpan 结构的新实例初始化为指定的小时数、分钟数和秒数。。
        TimeSpan(int hours, int minutes, int seconds);
        //
        // 摘要:
        //     将 TimeSpan 结构的新实例初始化为指定的天数、小时数、分钟数和秒数。
        TimeSpan(int days, int hours, int minutes, int seconds);

public :
    int seconds;
    int minutes;
    int hours;
    int days;
    double total_seconds;
    double total_minutes;
    double total_hours;
    double total_days;
//    void print(Uart& uart);

};  
#endif
