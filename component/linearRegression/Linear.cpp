
/**
  ******************************************************************************
  * @file    linear.cpp
  * @author  shentq
  * @version V2.0
  * @date    2021/08/7
  * @brief   ebox application example .
  ******************************************************************************
  * @attention
  *
  * No part of this software may be used for any commercial activities by any form
  * or means, without the prior written consent of shentq. This specification is
  * preliminary and is subject to change at any time without notice. shentq assumes
  * no responsibility for any errors contained herein.
  * <h2><center>&copy; Copyright 2015 shentq. All Rights Reserved.</center></h2>
  ******************************************************************************
  */
#include "linear.h"
#include "ebox_core.h"


#if EBOX_DEBUG
#define  LINEAR_DEBUG false
#define  LINEAR_ERROR true
#endif


#if EBOX_DEBUG

#if LINEAR_DEBUG
#define  LinearDebug(...) DBG("[Linear dbg]:"),DBG(__VA_ARGS__)
#else
#define  LinearDebug(...)
#endif

#if LINEAR_ERROR
#define LinearErr(...) ebox_printf("[Linear err]:%d:", __LINE__),ebox_printf( __VA_ARGS__)
#else
#define LinearErr(fmt, ...)
#endif
#endif

Linear::Linear()
{
    rows = 0;
    x = (double*)ebox_malloc(sizeof(*x));
    y = (double*)ebox_malloc(sizeof(*y));
}
Linear::~Linear()
{
    clear();
}
void Linear::add_row(double _x,double _y)
{
    x = (double*)ebox_realloc(x,sizeof(_x)*(rows+1));
    y = (double*)ebox_realloc(y,sizeof(_y)*(rows+1));
    x[rows] = _x;
    y[rows] = _y;
    
    rows++;
    LinearDebug("------Ԫ��(%d)---------\n",rows);
    for(int i = 0; i < rows;i++)
    {
        LinearDebug("x[%d]:%0.3f \t y[%d]:%0.3f \n",i,x[i],i,y[i]);
    }
}

bool Linear::exe()
{
    double xa = 0.0;
    double ya = 0.0;  
    double Lxx = 0.0;
    double Lxy = 0.0;
    if ( rows < 1)
        return false;
    for (int i = 0; i < rows; i++)
    {
        xa += x[i];
        ya += y[i];
    }
    xa /= rows;                                     // Xƽ��ֵ
    ya /= rows;                                     // Yƽ��ֵ
    for (int i = 0; i < rows; i++)
    {
        Lxx += ((x[i] - xa) * (x[i] - xa));             // Lxx = Sum((X - Xa)ƽ��)
        Lxy += ((x[i] - xa) * (y[i] - ya));       // Lxy = Sum((X - Xa)(Y - Ya))
    }
    
    rate = Lxy / Lxx;                                // k = Lxy / Lxx
    offset = ya - rate * xa;                              // b = Ya - k*Xa
    LinearDebug("y=%0.3f*x+%0.3f\n",rate,offset);
    // �������
    SquarePoor[0] = SquarePoor[1] = 0.0;
    for (int i = 0; i < rows; i++)
    {
        Lxy = offset + rate * x[i];//����ֵ
        SquarePoor[0] += ((Lxy - ya) * (Lxy - ya)); // U(�ع�ƽ����)
        SquarePoor[1] += ((y[i] - Lxy) * (y[i] - Lxy)); // Q(ʣ��ƽ����)
    }
    RSS = SquarePoor[0];//����ֵ����ʵƽ��ֵ�Ĳ��ƽ����
    ESS = SquarePoor[1];//����ֵ����ʵֵ�Ĳ��ƽ����
    TSS = RSS + ESS;
    

    
    SquarePoor[2] = SquarePoor[0];                  // �ع鷽��
    SquarePoor[3] = SquarePoor[1] / (rows - 2);     // ʣ�෽��
    
    RMSE2 = SquarePoor[3];
    RMSE = sqrt(SquarePoor[3]);
    R = sqrt(SquarePoor[0] / (SquarePoor[0] + SquarePoor[1]));
    F = SquarePoor[2] /SquarePoor[3];
    
    display();
    return true;
}
void Linear::clear()
{
    rows = 0;
    ebox_free(x);
    ebox_free(y);
}
void Linear::display()
{

    double v;
    int i;
    LinearDebug("�ع鷽��ʽ:    Y = %.5lf", offset);
    LinearDebug(" + %.5lf*X\n", rate);
    LinearDebug("�ع������Լ���: \n");
    LinearDebug("�ع�ƽ���ͣ�%12.4lf  �ع鷽�%12.4lf \n", RSS, RSS);
    LinearDebug("ʣ��ƽ���ͣ�%12.4lf  ʣ�෽�%12.4lf \n", ESS, RMSE2);
    LinearDebug("���ƽ���ͣ�%12.4lf  ��׼��%12.4lf \n", TSS, RMSE);
    LinearDebug("F   ��  �飺%12.4lf  ���ϵ����%12.4lf \n", F,R);
    LinearDebug("ʣ�����: \n");
    LinearDebug("      �۲�ֵ      ����ֵ      ʣ��ֵ    ʣ��ƽ�� \n");
    for (i = 0; i < rows; i ++)
    {
        v = offset;
        v += x[i] * rate;
        LinearDebug("%12.2lf%12.2lf%12.2lf%12.2lf \n", y[i], v, y[i] - v, (y[i] - v) * (y[i] - v));
    }
}
