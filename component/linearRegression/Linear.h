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

    //(residual sum of squares:残差平方和,也称剩余平方和)是实际值与估计值之差的平方的总和，也就是误差项平方的总和，利用剩余平方和可以很好地表示剩余的总和。
    double RSS;// RSS = sum(((y[i] - Lxy) * (y[i] - Lxy))) 

    //(Explained Sum of Squares:回归平方和，sum(power(y - average(Y),2)）  是估计值y-真实平均值Y的差的平方和
    double ESS; // ESS = sum((Lxy - ya) * (Lxy - ya))

    //(Total Sum of Squares:总离差平方和，或者总平方和）  是因变量回归值-因变量平均值的离差平方和
    double TSS; //TSS = 剩余平方和 + 回归平方和
    
    double RMSE2;//剩余方差：是剩余平方和的平均值

    //Root mean squared error:剩余标准差也被称为标准偏差，标准差(Standard Deviation)描述各数据偏离平均数的距离（离均差）的平均数，
    double RMSE;//它是剩余方差的方根
              

    double F;//F检验值
    double R;//相关性系数：回归方差除以剩余方差



};

/*


------元素(4)---------
x[0]:1.000 	 y[0]:3.000 
x[1]:2.000 	 y[1]:5.000 
x[2]:3.000 	 y[2]:6.400 
x[3]:4.000 	 y[3]:9.500 
y=2.090*x+0.750
回归方程式:    Y = 0.75000 + 2.09000*X
回归显著性检验: 
回归平方和：     21.8405  回归方差：     21.8405 
剩余平方和：      0.5670  剩余方差：      0.2835 
离差平方和：     22.4075  标准误差：      0.5324 
F   检  验：     77.0388  相关系数：      0.9873 
剩余分析: 
      观察值      估计值      剩余值    剩余平方 
        3.00        2.84        0.16        0.03 
        5.00        4.93        0.07        0.00 
        6.40        7.02       -0.62        0.38 
        9.50        9.11        0.39        0.15 
*/

//剩余标准差统计学符号为RMSE（Root mean squared error）,又称为均方根误差、标准误差、回归系统的拟合标准差
//剩余标准差SE，也称均方差
/*
剩余标准差SE，也称均方差，统计学概念，在线性回归分析中，真实值和估计值之间的差称为残差（或者剩余量），所有预测值的残差平方和（或者剩余平方和），剩余标准差就是剩余平方和的开平方。用来表示估计值的精度。
RMSE=power(MSE,0.5)=POWER(SSE/n)


离差平方和（Sum of Squares of Deviations,简称TSS）是各项与平均项之差的平方的总和。定义是设x是一个随机变量，令η=x-Ex, 则 称 η为x的离差，它反映了x与其数学期望Ex的偏离程度。
*/
#endif
