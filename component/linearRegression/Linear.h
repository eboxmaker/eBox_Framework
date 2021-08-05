#ifndef __LINEAR_H
#define __LINEAR_H

#include "ebox_mem.h"
class Linear
{
public:
    Linear();
    int linear_regression(double *data, int rows, double *a, double *b, double *SquarePoor);
    void add_row(double _x,double _y);
    bool exe();
void display();
private:
    double rate;
    double offset;
    double *x;
    double *y;
    int rows;

    double SquarePoor[4];

    double ESS; //(Explained Sum of Squares:�ع�ƽ����
    double variance; //���� s= ��(x-u)^2/N;

    double SSR;//residual sum of squares:�в�ƽ����,Ҳ��ʣ��ƽ����

    double SSD;//���ƽ���ͣ�Sum of Squares of Deviations���Ǹ�����ƽ����֮���ƽ�����ܺ͡�
    double RMSE;//Root mean squared error:��׼��Ҳ����Ϊ��׼ƫ���׼��(Standard Deviation)����������ƫ��ƽ�����ľ��루������ƽ������
              //�������ƽ����ƽ����ķ������æұ�ʾ
    double F;//F����ֵ
    double R;//�����ϵ�����ع鷽�����ʣ�෽��




    double SST;//��ƽ���ͣ�total sum of squares��

};
//x[0]:1.000 	 y[0]:3.000 
//x[1]:2.000 	 y[1]:5.000 
//x[2]:3.000 	 y[2]:0.000 
//x[3]:4.000 	 y[3]:200.000 
//y=58.600*x+-94.500
//�ع鷽��ʽ:    Y = -94.50000 + 58.60000*X
//�ع������Լ���: 
//�ع�ƽ���ͣ�  17169.8000  �ع鷽�  17169.8000 
//ʣ��ƽ���ͣ�  12048.2000  ʣ�෽�   6024.1000 
//���ƽ���ͣ�  29218.0000  ��׼��     77.6151 
//F   ��  �飺      2.8502  ���ϵ����      0.7666 
//ʣ�����: 
//      �۲�ֵ      ����ֵ      ʣ��ֵ    ʣ��ƽ�� 
//        3.00      -35.90       38.90     1513.21 
//        5.00       22.70      -17.70      313.29 
//        0.00       81.30      -81.30     6609.69 
//      200.00      139.90       60.10     3612.01 

//ʣ���׼��ͳ��ѧ����ΪRMSE��Root mean squared error��,�ֳ�Ϊ����������׼���ع�ϵͳ����ϱ�׼��
//ʣ���׼��SE��Ҳ�ƾ�����
#endif
