#ifndef __TIMESPAN_H
#define __TIMESPAN_H

#include "ebox_uart.h"

class TimeSpan
{
public :
        //
        // 摘要:
        //     将 System.TimeSpan 结构的新实例初始化为指定的刻度数。
        //
        // 参数:
        //   ticks:
        //     以 100 毫微秒为单位表示的时间段。
        TimeSpan(){};
        TimeSpan(long ticks);
        //
        // 摘要:
        //     将 System.TimeSpan 结构的新实例初始化为指定的小时数、分钟数和秒数。
        //
        // 参数:
        //   hours:
        //     小时数。
        //
        //   minutes:
        //     分钟数。
        //
        //   seconds:
        //     秒数。
        //
        // 异常:
        //   T:System.ArgumentOutOfRangeException:
        //     参数指定一个小于 System.TimeSpan.MinValue 或大于 System.TimeSpan.MaxValue 的 System.TimeSpan
        //     值。
        TimeSpan(int hours, int minutes, int seconds);
        //
        // 摘要:
        //     将 System.TimeSpan 结构的新实例初始化为指定的天数、小时数、分钟数和秒数。
        //
        // 参数:
        //   days:
        //     天数。
        //
        //   hours:
        //     小时数。
        //
        //   minutes:
        //     分钟数。
        //
        //   seconds:
        //     秒数。
        //
        // 异常:
        //   T:System.ArgumentOutOfRangeException:
        //     参数指定一个小于 System.TimeSpan.MinValue 或大于 System.TimeSpan.MaxValue 的 System.TimeSpan
        //     值。
        TimeSpan(int days, int hours, int minutes, int seconds);
        //
        // 摘要:
        //     将 System.TimeSpan 结构的新实例初始化为指定的天数、小时数、分钟数、秒数和毫秒数。
        //
        // 参数:
        //   days:
        //     天数。
        //
        //   hours:
        //     小时数。
        //
        //   minutes:
        //     分钟数。
        //
        //   seconds:
        //     秒数。
        //
        //   milliseconds:
        //     毫秒数。
        //
        // 异常:
        //   T:System.ArgumentOutOfRangeException:
        //     参数指定一个小于 System.TimeSpan.MinValue 或大于 System.TimeSpan.MaxValue 的 System.TimeSpan
        //     值。
        TimeSpan(int days, int hours, int minutes, int seconds, int milliseconds);

public :
    int seconds;
    int minutes;
    int hours;
    int days;

    double total_seconds;
    double total_minutes;
    double total_hours;
    double total_days;

    void print(Uart& uart);


};  

#endif
