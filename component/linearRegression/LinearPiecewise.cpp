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

LinearPiecewise::LinearPiecewise()
{
    rows = 0;
}
LinearPiecewise::~LinearPiecewise()
{
    clear();
}
bool LinearPiecewise::add_row(double _x,double _y)
{
    for(int i = 0; i < rows;i++)
    {
        if(_x == pair[i].x)
        {
            LinearErr("item is exist\n");
            return false;
        }
    }
    
    pair = (Pair*)ebox_realloc(pair,sizeof(*pair)*(rows+1));
    pair[rows].x = _x;
    pair[rows].y = _y;
    LinearDebug("Ԫ�أ�x:%0.3f \t y:%0.3f \n",pair[rows].x,pair[rows].y);
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
    
    Lxx += ((x0 - xa) * (x0 - xa));             // Lxx = Sum((X - Xa)ƽ��)
    Lxx += ((x1 - xa) * (x1 - xa));             // Lxx = Sum((X - Xa)ƽ��)

    Lxy += ((x0 - xa) * (y0 - ya));       // Lxy = Sum((X - Xa)(Y - Ya))
    Lxy += ((x1 - xa) * (y1 - ya));       // Lxy = Sum((X - Xa)(Y - Ya))
    item.rate = Lxy / Lxx;                                // k = Lxy / Lxx
    item.offset = ya - item.rate * xa;                              // b = Ya - k*Xa
    item.xStart = min2v(x0,x1);
    item.xEnd = max2v(x0,x1);
    LinearDebug("���Է��̣�y=%0.3f*x+%0.3f\n",item.rate,item.offset);
    return item;
}

bool LinearPiecewise::exe()
{
    if ( rows < 2)
        return false;

    itemListPtr = (Group*)ebox_malloc(sizeof(Group) * (rows - 1));
    sort();
    for (int i = 0; i < rows - 1; i++)
    {
        Group item = calculate(pair[i].x,pair[i].y,pair[i+1].x,pair[i+1].y);
        itemListPtr[i] = item;
    }
    for(int i = 0; i < rows - 1; i++)
    {
        LinearDebug("������Ϣ��xs = %0.3f,xe=%0.3f,k=%0.3f,b=%0.3f\n",itemListPtr[i].xStart,itemListPtr[i].xEnd,itemListPtr[i].rate,itemListPtr[i].offset);
    }

    return true;
}
double LinearPiecewise::get(double x)
{
    double value;
    int room = -1;
    if(x < itemListPtr[0].xStart)
    {
        room = 0;
        value = (itemListPtr[0].rate * x + itemListPtr[0].offset);
    }
    else if(x > itemListPtr[rows - 2].xEnd)
    {
        room = rows - 2;
        value = (itemListPtr[rows - 2].rate * x + itemListPtr[rows - 2].offset);
    }
    else
    {
        for(int i = 0; i < rows - 1; i++)
        {
            if(x >= itemListPtr[i].xStart && x <= itemListPtr[i].xEnd)
            {
                room = i;
                value = itemListPtr[i].rate * x + itemListPtr[i].offset;
                break;
            }
        }
    }
    LinearDebug("����:%d\t rate=%0.3f\t offset=%0.3f\t%0.3f<=x=%0.1f<=%0.3f\ty=%0.3f\n",\
                room,itemListPtr[room].rate,itemListPtr[room].offset
                ,itemListPtr[room].xStart,x,itemListPtr[room].xEnd,value);

    return value;

}

void LinearPiecewise::clear()
{
    ebox_free(pair);
    ebox_free(itemListPtr);
    rows = 0;
}

void LinearPiecewise::sort()
{
    Pair* tempPair = (Pair*)ebox_malloc(sizeof(Pair) * (rows));
    double *buf = (double*)ebox_malloc(sizeof(double) * rows);
    
    LinearDebug("------Ԫ��(%d)---------\n",rows);
    for(int i = 0; i < rows;i++)
    {
        LinearDebug("x[%d]:%0.3f \t y[%d]:%0.3f \n",i,pair[i].x,i,pair[i].y);
    }
    
    
    for(int i = 0; i < rows; i++)
    {
        buf[i] = pair[i].x;
    }
    quick_sort(buf,0,rows - 1);
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < rows; j++)
        {
            if(buf[i] == pair[j].x)
            {
                tempPair[i] = pair[j];
            }
        }
    }
    for(int i = 0; i < rows;i++)
    {
        pair[i] = tempPair[i];
    }
    ebox_free(buf);
    ebox_free(tempPair);
    LinearDebug("------����Ԫ��(%d)---------\n",rows);
    for(int i = 0; i < rows;i++)
    {
        LinearDebug("x[%d]:%0.3f \t y[%d]:%0.3f \n",i,pair[i].x,i,pair[i].y);
    }
    

}

