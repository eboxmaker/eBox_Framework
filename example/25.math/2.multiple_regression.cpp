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
double data[15][5] = {
//   X1   X2    X3   X4    Y
  { 316, 1536, 874, 981, 3894 },
  { 385, 1771, 777, 1386, 4628 },
  { 299, 1565, 678, 1672, 4569 },
  { 326, 1970, 785, 1864, 5340 },
  { 441, 1890, 785, 2143, 5449 },
  { 460, 2050, 709, 2176, 5599 },
  { 470, 1873, 673, 1769, 5010 },
  { 504, 1955, 793, 2207, 5694 },
  { 348, 2016, 968, 2251, 5792 },
  { 400, 2199, 944, 2390, 6126 },
  { 496, 1328, 749, 2287, 5025 },
  { 497, 1920, 952, 2388, 5924 },
  { 533, 1400, 1452, 2093, 5657 },
  { 506, 1612, 1587, 2083, 6019 },
  { 458, 1613, 1485, 2390, 6141 },
};



void display(double *dat, double *Answer, double *SquarePoor, int rows, int cols)
{
    double v, *p;
    int i, j;
    uart1.printf("回归方程式:    Y = %.5lf", Answer[0]);
    for (i = 1; i < cols; i ++)
        uart1.printf(" + %.5lf*X%d", Answer[i], i);
    uart1.printf("\r\n");
    uart1.printf("回归显著性检验: \r\n");
    uart1.printf("回归平方和：%12.4lf  回归方差：%12.4lf \r\n", SquarePoor[0], SquarePoor[2]);
    uart1.printf("剩余平方和：%12.4lf  剩余方差：%12.4lf \r\n", SquarePoor[1], SquarePoor[3]);
    uart1.printf("离差平方和：%12.4lf  标准误差：%12.4lf \r\n", SquarePoor[0] + SquarePoor[1], sqrt(SquarePoor[3]));
    uart1.printf("F   检  验：%12.4lf  相关系数：%12.4lf \r\n", SquarePoor[2] / SquarePoor[3],
           sqrt(SquarePoor[0] / (SquarePoor[0] + SquarePoor[1])));
    uart1.printf("剩余分析: \r\n");
    uart1.printf("      观察值       估计值        剩余值      剩余平方 \r\n");
    for (i = 0, p = dat; i < rows; i ++, p ++)
    {
        v = Answer[0];
        for (j = 1; j < cols; j ++, p ++)
            v += *p * Answer[j];
        uart1.printf("%12.2lf%12.2lf%12.2lf%12.2lf \r\n", *p, v, *p - v, (*p - v) * (*p - v));
    }
    system("pause");
}


void setup()
{
    double Answer[5], SquarePoor[4];
    ebox_init();
    uart1.begin(115200);
    uart1.printf("\r\nuart1 115200 ok!\r\n");

    if (multiple_regression((double*)data, 15, 5, Answer, SquarePoor) == 0)
        display((double*)data, Answer, SquarePoor, 15, 5);
}


int main(void)
{
    setup();

    while(1)
    {

    }
}




