#include "template.h"
int  _Init(void)
{
    int status;
    //添加用户的lcd驱动代码
    return status;
}
void _On()
{
    //添加用户的lcd驱动代码

}
void _Off()
{
    //添加用户的lcd驱动代码
}
void _SetPixelIndex(u16 x,u16 y,int PixelIndex);
void _DrawHLine(int x0, int y,  int x1);
void _DrawVLine(int x, int y0,  int y1);
void _FillRect(int x0, int y0,  int x1, int y1);