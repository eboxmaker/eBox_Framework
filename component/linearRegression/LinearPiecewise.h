/**
  ******************************************************************************
  * @file    linearPiecewise.h
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


#ifndef __LINEARPiecewise_H
#define __LINEARPiecewise_H

#include "ebox_mem.h"
#include "LinkedList/LinkedList.h"


/*
    1,��������
    2,���N������
    3,����exe��������
    4,�û����ɵ���get�õ����
    5,����У׼���ȵ���clear,�ظ�234
*/
class LinearPiecewise
{
public:
    typedef struct {
        double xStart;
        double xEnd;
        double rate;
        double offset;
    }Group;
    typedef struct{
        double x;
        double y;
    }Pair;
public:
    LinearPiecewise();
    ~LinearPiecewise();
    bool add_row(double x,double y);
    bool exe();
    double get(double x);
    void clear();
    
private:
    Group calculate(double x0,double y0,double x1,double y1);
    void sort();
    static int cmp(LinearPiecewise::Pair &a, LinearPiecewise::Pair &b) {
        return a.x > b.x;
    }
    int rows;
    LinkedList<Pair> pList;
    LinkedList<Group> itemList;
};



#endif
