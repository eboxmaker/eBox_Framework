
/**
  ******************************************************************************
  * @file    list.h
  * @author  shentq
  * @version V1.0
  * @date    2017/03/02
  * @brief   Listµ•œÚ¡¥±Ì
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
#ifndef __LIST_H
#define __LIST_H
#include "ebox_type.h"
#include "ebox_core.h"
#include "ebox_mem.h"

typedef struct _node
{
    void           *data;
    struct _node   *next;
} Node;

class List
{
    private:
        Node *_head;
        Node *_tail;
        int  _size;
    public:

        List()
        {
            _head = NULL;
            _size = 0;
        }
        ~List()
        {
    //        ebox_printf("list0:%d\r\n",ebox_get_free());
            clear();
    //        ebox_printf("list1:%d\r\n",ebox_get_free());

        }
        int  insert(int at, void *data);
        int  insert_head(void *data);
        int  insert_tail(void *data);
        int  remove(int x);
        void *data(int x);
        Node *head();
        Node *tail();
        int  is_empty();
        int  clear();
        int  modify_node(int x, void *data);
        int  swap(int x, int y);
        int  size()
        {
            return _size;
        }
};
#endif
