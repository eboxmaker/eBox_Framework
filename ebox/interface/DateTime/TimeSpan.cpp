#include "TimeSpan.h"


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