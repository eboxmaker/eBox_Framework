#ifndef __TIMESPAN_H
#define __TIMESPAN_H

#include "ebox_uart.h"

class TimeSpan
{
public :
        //
        // ժҪ:
        //     �� System.TimeSpan �ṹ����ʵ����ʼ��Ϊָ���Ŀ̶�����
        //
        // ����:
        //   ticks:
        //     �� 100 ��΢��Ϊ��λ��ʾ��ʱ��Ρ�
        TimeSpan(){};
        TimeSpan(long ticks);
        //
        // ժҪ:
        //     �� System.TimeSpan �ṹ����ʵ����ʼ��Ϊָ����Сʱ������������������
        //
        // ����:
        //   hours:
        //     Сʱ����
        //
        //   minutes:
        //     ��������
        //
        //   seconds:
        //     ������
        //
        // �쳣:
        //   T:System.ArgumentOutOfRangeException:
        //     ����ָ��һ��С�� System.TimeSpan.MinValue ����� System.TimeSpan.MaxValue �� System.TimeSpan
        //     ֵ��
        TimeSpan(int hours, int minutes, int seconds);
        //
        // ժҪ:
        //     �� System.TimeSpan �ṹ����ʵ����ʼ��Ϊָ����������Сʱ������������������
        //
        // ����:
        //   days:
        //     ������
        //
        //   hours:
        //     Сʱ����
        //
        //   minutes:
        //     ��������
        //
        //   seconds:
        //     ������
        //
        // �쳣:
        //   T:System.ArgumentOutOfRangeException:
        //     ����ָ��һ��С�� System.TimeSpan.MinValue ����� System.TimeSpan.MaxValue �� System.TimeSpan
        //     ֵ��
        TimeSpan(int days, int hours, int minutes, int seconds);
        //
        // ժҪ:
        //     �� System.TimeSpan �ṹ����ʵ����ʼ��Ϊָ����������Сʱ�����������������ͺ�������
        //
        // ����:
        //   days:
        //     ������
        //
        //   hours:
        //     Сʱ����
        //
        //   minutes:
        //     ��������
        //
        //   seconds:
        //     ������
        //
        //   milliseconds:
        //     ��������
        //
        // �쳣:
        //   T:System.ArgumentOutOfRangeException:
        //     ����ָ��һ��С�� System.TimeSpan.MinValue ����� System.TimeSpan.MaxValue �� System.TimeSpan
        //     ֵ��
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
