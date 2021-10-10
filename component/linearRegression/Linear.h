/**
  ******************************************************************************
  * @file    linear.h
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

#ifndef __LINEAR_H
#define __LINEAR_H

#include "ebox_mem.h"
class Linear
{
public:
    Linear();
    ~Linear();
    void add_row(double _x,double _y);
    bool exe();
    void clear();
    void display();
public:
    double rate;
    double offset;
private:

    double *x;
    double *y;
    int rows;

    double SquarePoor[4];

    //TSS = ESS + RSS

    //(residual sum of squares:�в�ƽ����,Ҳ��ʣ��ƽ����)��ʵ��ֵ�����ֵ֮���ƽ�����ܺͣ�Ҳ���������ƽ�����ܺͣ�����ʣ��ƽ���Ϳ��Ժܺõر�ʾʣ����ܺ͡�
    double RSS;// RSS = sum(((y[i] - Lxy) * (y[i] - Lxy))) 

    //(Explained Sum of Squares:�ع�ƽ���ͣ�sum(power(y - average(Y),2)��  �ǹ���ֵy-��ʵƽ��ֵY�Ĳ��ƽ����
    double ESS; // ESS = sum((Lxy - ya) * (Lxy - ya))

    //(Total Sum of Squares:�����ƽ���ͣ�������ƽ���ͣ�  ��������ع�ֵ-�����ƽ��ֵ�����ƽ����
    double TSS; //TSS = ʣ��ƽ���� + �ع�ƽ����
    
    double RMSE2;//ʣ�෽���ʣ��ƽ���͵�ƽ��ֵ

    //Root mean squared error:ʣ���׼��Ҳ����Ϊ��׼ƫ���׼��(Standard Deviation)����������ƫ��ƽ�����ľ��루������ƽ������
    double RMSE;//����ʣ�෽��ķ���
              

    double F;//F����ֵ
    double R;//�����ϵ�����ع鷽�����ʣ�෽��



};

/*


------Ԫ��(4)---------
x[0]:1.000 	 y[0]:3.000 
x[1]:2.000 	 y[1]:5.000 
x[2]:3.000 	 y[2]:6.400 
x[3]:4.000 	 y[3]:9.500 
y=2.090*x+0.750
�ع鷽��ʽ:    Y = 0.75000 + 2.09000*X
�ع������Լ���: 
�ع�ƽ���ͣ�     21.8405  �ع鷽�     21.8405 
ʣ��ƽ���ͣ�      0.5670  ʣ�෽�      0.2835 
���ƽ���ͣ�     22.4075  ��׼��      0.5324 
F   ��  �飺     77.0388  ���ϵ����      0.9873 
ʣ�����: 
      �۲�ֵ      ����ֵ      ʣ��ֵ    ʣ��ƽ�� 
        3.00        2.84        0.16        0.03 
        5.00        4.93        0.07        0.00 
        6.40        7.02       -0.62        0.38 
        9.50        9.11        0.39        0.15 
*/

//ʣ���׼��ͳ��ѧ����ΪRMSE��Root mean squared error��,�ֳ�Ϊ����������׼���ع�ϵͳ����ϱ�׼��
//ʣ���׼��SE��Ҳ�ƾ�����
/*
ʣ���׼��SE��Ҳ�ƾ����ͳ��ѧ��������Իع�����У���ʵֵ�͹���ֵ֮��Ĳ��Ϊ�в����ʣ������������Ԥ��ֵ�Ĳв�ƽ���ͣ�����ʣ��ƽ���ͣ���ʣ���׼�����ʣ��ƽ���͵Ŀ�ƽ����������ʾ����ֵ�ľ��ȡ�
RMSE=power(MSE,0.5)=POWER(SSE/n)


���ƽ���ͣ�Sum of Squares of Deviations,���TSS���Ǹ�����ƽ����֮���ƽ�����ܺ͡���������x��һ��������������=x-Ex, �� �� ��Ϊx��������ӳ��x������ѧ����Ex��ƫ��̶ȡ�
*/
#endif
