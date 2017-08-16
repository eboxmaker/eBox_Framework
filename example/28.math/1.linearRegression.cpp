/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox


#include "ebox.h"
#include "math.h"
#include "../core/math/LinearRegression.h"
double data1[12][2] = {
//    X      Y
    {187.1, 25.4},
    {179.5, 22.8},
    {157.0, 20.6},
    {197.0, 21.8},
    {239.4, 32.4},
    {217.8, 24.4},
    {227.1, 29.3},
    {233.4, 27.9},
    {242.0, 27.8},
    {251.9, 34.2},
    {230.0, 29.2},
    {271.8, 30.0}
};
double answer[2];
double SquarePoor[4];


void display(double *dat, double *Answer, double *SquarePoor, int rows, int cols)
{
    double v, *p;
    int i, j;
    uart1.printf("�ع鷽��ʽ:    Y = %.5lf", Answer[0]);
    for (i = 1; i < cols; i ++)
        uart1.printf(" + %.5lf*X%d\r\n", Answer[i], i);
    uart1.printf("�ع������Լ���: \r\n");
    uart1.printf("�ع�ƽ���ͣ�%12.4lf  �ع鷽�%12.4lf \r\n", SquarePoor[0], SquarePoor[2]);
    uart1.printf("ʣ��ƽ���ͣ�%12.4lf  ʣ�෽�%12.4lf \r\n", SquarePoor[1], SquarePoor[3]);
    uart1.printf("���ƽ���ͣ�%12.4lf  ��׼��%12.4lf \r\n", SquarePoor[0] + SquarePoor[1], sqrt(SquarePoor[3]));
    uart1.printf("F   ��  �飺%12.4lf  ���ϵ����%12.4lf \r\n", SquarePoor[2] /SquarePoor[3],
           sqrt(SquarePoor[0] / (SquarePoor[0] + SquarePoor[1])));
    uart1.printf("ʣ�����: \r\n");
    uart1.printf("      �۲�ֵ      ����ֵ      ʣ��ֵ    ʣ��ƽ�� \r\n");
    for (i = 0, p = dat; i < rows; i ++, p ++)
    {
        v = Answer[0];
        for (j = 1; j < cols; j ++, p ++)
            v += *p * Answer[j];
        uart1.printf("%12.2lf%12.2lf%12.2lf%12.2lf \r\n", *p, v, *p - v, (*p - v) * (*p - v));
    }
}


void setup()
{
    ebox_init();
    uart1.begin(115200);
    uart1.printf("\r\nuart1 115200 ok!");

    
    linear_regression((double*)data1,12,&answer[0],&answer[1],&SquarePoor[0]);
    display((double*)data1,answer,&SquarePoor[0],12,2);
}

int main(void)
{
    setup();

    while(1)
    {

    }
}




