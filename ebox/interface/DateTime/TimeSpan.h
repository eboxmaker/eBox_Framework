#ifndef __TIMESPAN_H
#define __TIMESPAN_H

#include "ebox_type.h"
class TimeSpan
{
public :
        //
        // ժҪ:
        //     �� TimeSpan �ṹ����ʵ����ʼ��Ϊָ���Ŀ̶�����
        // ����:
        //   ticks:����Ϊ��λ��ʾ��ʱ��Ρ�
        TimeSpan(){};
        TimeSpan(int64_t seconds);
        //
        // ժҪ:
        //     �� TimeSpan �ṹ����ʵ����ʼ��Ϊָ����Сʱ��������������������
        TimeSpan(int hours, int minutes, int seconds);
        //
        // ժҪ:
        //     �� TimeSpan �ṹ����ʵ����ʼ��Ϊָ����������Сʱ������������������
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
