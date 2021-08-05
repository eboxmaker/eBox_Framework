#include "linear.h"
#include "ebox.h"

Linear::Linear()
{
    rows = 0;
    x = (double*)ebox_malloc(sizeof(*x));
    y = (double*)ebox_malloc(sizeof(*y));
}
void Linear::add_row(double _x,double _y)
{
    x = (double*)ebox_realloc(x,sizeof(_x)*(rows+1));
    y = (double*)ebox_realloc(y,sizeof(_y)*(rows+1));
    x[rows] = _x;
    y[rows] = _y;
    
    rows++;
    uart1.printf("------元素(%d)---------\n",rows);
    for(int i = 0; i < rows;i++)
    {
        uart1.printf("x[%d]:%0.3f \t y[%d]:%0.3f \n",i,x[i],i,y[i]);
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
    xa /= rows;                                     // X平均值
    ya /= rows;                                     // Y平均值
    for (int i = 0; i < rows; i++)
    {
        Lxx += ((x[i] - xa) * (x[i] - xa));             // Lxx = Sum((X - Xa)平方)
        Lxy += ((x[i] - xa) * (y[i] - ya));       // Lxy = Sum((X - Xa)(Y - Ya))
    }
    
    rate = Lxy / Lxx;                                // k = Lxy / Lxx
    offset = ya - rate * xa;                              // b = Ya - k*Xa
    uart1.printf("y=%0.3f*x+%0.3f\n",rate,offset);
    // 方差分析
    SquarePoor[0] = SquarePoor[1] = 0.0;
    for (int i = 0; i < rows; i++)
    {
        Lxy = offset + rate * x[i];
        SquarePoor[0] += ((Lxy - ya) * (Lxy - ya)); // U(回归平方和)
        SquarePoor[1] += ((y[i] - Lxy) * (y[i] - Lxy)); // Q(剩余平方和)
    }
    SquarePoor[2] = SquarePoor[0];                  // 回归方差
    SquarePoor[3] = SquarePoor[1] / (rows - 2);     // 剩余方差
    display();
}
void Linear::display()
{

    double v;
    int i, j;
    uart1.printf("回归方程式:    Y = %.5lf", offset);
    uart1.printf(" + %.5lf*X\r\n", rate);
    uart1.printf("回归显著性检验: \r\n");
    uart1.printf("回归平方和：%12.4lf  回归方差：%12.4lf \r\n", SquarePoor[0], SquarePoor[2]);
    uart1.printf("剩余平方和：%12.4lf  剩余方差：%12.4lf \r\n", SquarePoor[1], SquarePoor[3]);
    uart1.printf("离差平方和：%12.4lf  标准误差：%12.4lf \r\n", SquarePoor[0] + SquarePoor[1], sqrt(SquarePoor[3]));
    uart1.printf("F   检  验：%12.4lf  相关系数：%12.4lf \r\n", SquarePoor[2] /SquarePoor[3],
           sqrt(SquarePoor[0] / (SquarePoor[0] + SquarePoor[1])));
    uart1.printf("剩余分析: \r\n");
    uart1.printf("      观察值      估计值      剩余值    剩余平方 \r\n");
    for (i = 0; i < rows; i ++)
    {
        v = offset;
        v += x[i] * rate;
        uart1.printf("%12.2lf%12.2lf%12.2lf%12.2lf \r\n", y[i], v, y[i] - v, (y[i] - v) * (y[i] - v));
    }
}
