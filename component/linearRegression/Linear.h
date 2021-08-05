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

    double ESS; //(Explained Sum of Squares:回归平方和
    double variance; //方差 s= ∑(x-u)^2/N;

    double SSR;//residual sum of squares:残差平方和,也称剩余平方和

    double SSD;//离差平方和（Sum of Squares of Deviations）是各项与平均项之差的平方的总和。
    double RMSE;//Root mean squared error:标准差也被称为标准偏差，标准差(Standard Deviation)描述各数据偏离平均数的距离（离均差）的平均数，
              //它是离差平方和平均后的方根，用σ表示
    double F;//F检验值
    double R;//相关性系数：回归方差除以剩余方差




    double SST;//总平方和（total sum of squares）

};
//x[0]:1.000 	 y[0]:3.000 
//x[1]:2.000 	 y[1]:5.000 
//x[2]:3.000 	 y[2]:0.000 
//x[3]:4.000 	 y[3]:200.000 
//y=58.600*x+-94.500
//回归方程式:    Y = -94.50000 + 58.60000*X
//回归显著性检验: 
//回归平方和：  17169.8000  回归方差：  17169.8000 
//剩余平方和：  12048.2000  剩余方差：   6024.1000 
//离差平方和：  29218.0000  标准误差：     77.6151 
//F   检  验：      2.8502  相关系数：      0.7666 
//剩余分析: 
//      观察值      估计值      剩余值    剩余平方 
//        3.00      -35.90       38.90     1513.21 
//        5.00       22.70      -17.70      313.29 
//        0.00       81.30      -81.30     6609.69 
//      200.00      139.90       60.10     3612.01 

//剩余标准差统计学符号为RMSE（Root mean squared error）,又称为均方根误差、标准误差、回归系统的拟合标准差
//剩余标准差SE，也称均方差
#endif
