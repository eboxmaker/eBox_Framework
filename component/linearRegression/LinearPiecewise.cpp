/**
  ******************************************************************************
  * @file    linearPiecewise.cpp
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

#include "linearPiecewise.h"
#include "ebox_core.h"
#include "sort.h"

#if EBOX_DEBUG
#define  LINEAR_DEBUG true
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

LinearPiecewise::LinearPiecewise()
{
    rows = 0;
}
LinearPiecewise::~LinearPiecewise()
{
    clear();
}
bool LinearPiecewise::add_row(double x,double y)
{
    for(int i = 0; i < rows;i++)
    {
        if(x == pList.get(i).x)
        {
            LinearErr("item is exist\n");
            return false;
        }
    }
    Pair p;
    p.x = x;
    p.y = y;
    pList.add(p);
    LinearDebug("add:x = %0.3f\t y = %0.3f\n",p.x,p.y);
    rows++;
    return true;
}

LinearPiecewise::Group LinearPiecewise::calculate(double x0,double y0,double x1,double y1)
{
    LinearPiecewise::Group item;
    double xa = 0.0;
    double ya = 0.0;  
    double Lxx = 0.0;
    double Lxy = 0.0;

    xa = (x0 + x1)/2;
    ya = (y0 + y1)/2;
    
    Lxx += ((x0 - xa) * (x0 - xa));             // Lxx = Sum((X - Xa)平方)
    Lxx += ((x1 - xa) * (x1 - xa));             // Lxx = Sum((X - Xa)平方)

    Lxy += ((x0 - xa) * (y0 - ya));       // Lxy = Sum((X - Xa)(Y - Ya))
    Lxy += ((x1 - xa) * (y1 - ya));       // Lxy = Sum((X - Xa)(Y - Ya))
    item.rate = Lxy / Lxx;                                // k = Lxy / Lxx
    item.offset = ya - item.rate * xa;                              // b = Ya - k*Xa
    item.xStart = min2v(x0,x1);
    item.xEnd = max2v(x0,x1);
    LinearDebug("线性方程：y=%0.3f*x+%0.3f\n",item.rate,item.offset);
    return item;
}

bool LinearPiecewise::exe()
{
    if ( rows < 2)
        return false;

    sort();
    for (int i = 0; i < rows - 1; i++)
    {
        Group item = calculate(pList[i].x,pList[i].y,pList[i+1].x,pList[i+1].y);
        itemList.add(item);
    }
    for(int i = 0; i < rows - 1; i++)
    {
        LinearDebug("区间信息：xs = %0.3f,xe=%0.3f,k=%0.3f,b=%0.3f\n",itemList[i].xStart,itemList[i].xEnd,itemList[i].rate,itemList[i].offset);
    }

    return true;
}
double LinearPiecewise::get(double x)
{
    double value;
    int room = -1;
    if(x < itemList[0].xStart)
    {
        room = 0;
        value = (itemList[0].rate * x + itemList[0].offset);
    }
    else if(x > itemList[rows - 2].xEnd)
    {
        room = rows - 2;
        value = (itemList[rows - 2].rate * x + itemList[rows - 2].offset);
    }
    else
    {
        for(int i = 0; i < rows - 1; i++)
        {
            if(x >= itemList[i].xStart && x <= itemList[i].xEnd)
            {
                room = i;
                value = itemList[i].rate * x + itemList[i].offset;
                break;
            }
        }
    }
    LinearDebug("区间:%d\t rate=%0.3f\t offset=%0.3f\t%0.3f<=x=%0.1f<=%0.3f\ty=%0.3f\n",\
                room,itemList[room].rate,itemList[room].offset
                ,itemList[room].xStart,x,itemList[room].xEnd,value);

    return value;

}

void LinearPiecewise::clear()
{
    pList.clear();
    itemList.clear();
    rows = 0;
}

void LinearPiecewise::sort()
{
    pList.sort(LinearPiecewise::cmp);
    LinearDebug("------排序元素(%d)---------\n",rows);
    for(int i = 0; i < rows;i++)
    {
        LinearDebug("x[%d]:%0.3f \t y[%d]:%0.3f \n",i,pList[i].x,i,pList[i].y);
    }
    

}

