
/**
  ******************************************************************************
  * @file    list.cpp
  * @author  shentq
  * @version V1.0
  * @date    2017/03/02
  * @brief   List单向链表
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
#include "list.h"
/**
 *@brief    List单向链表
            本数据链表采用尾插法，数据起始位置为0。结束位置为size() - 1；
            head()，tail()中的数据为有效数据；
            Node.data为指向任意缓冲区数据的指针。用户根据自己的需求进行类型转换
            可以是任意数据类型（可以使结构体，缓冲区，变量等）.
*/


int List::insert(int x,const void *data)
{
    Node *new_node,*p,*e;
    if(x > _size ) return -1;//判断插入位置是否超出list的长度
    if((new_node = (Node *)ebox_malloc(sizeof(Node))) == NULL )
        return -1;
    new_node->data = (Node *)data;
    new_node->next = NULL;
    p = _head;
    if(x == 0)//插入到链表头部
    {
        if(_size == 0)
            _head = new_node;
        else
        {
            new_node->next = p;
            _head = new_node;
        }
    }
    else
    {
        for(int i = 1; i < x; i++)//寻找要插入的位置
            p = p->next;//p为插入点的前一个位置
        if(x < _size )//如果是链表中间位置
        {
            e = p->next;//e为插入点的后一个位置
            p->next = new_node;//将上一个位置指向新的位置
            new_node->next= e;//将新的位置的下一个位置指向e
        }
        else//如果是链表末尾
        {
            p->next = new_node;//将上一个位置指向新的位置
            new_node->next= NULL;//将新的位置的下一个位置指向NULL
        }
    }
    _size++;
    return 0;
}
int  List::remove(int x)
{
    Node *new_node,*p,*s,*e;
    if(x >= _size ) return -1;//判断删除位置是否超出list的长度
    p = _head;
    if(x == 0)//删除链表头部
    {
        p = p->next;
        ebox_free(_head);
        if(_size == 1)
        {
            _head = NULL;
        }
        else
        {
            _head = p;
        }
    }
    else
    {
        for(int i = 1; i < x; i++)//寻找要删除的位置的前一个Node
        {
            p = p->next;//p为删除点的前一个位置
        }
        s = p->next;//s为删除点的位置
        
        if(x < (_size - 1))//删除的是中间位置
        {
            e = p->next->next;//e为删除点的后一个位置
            ebox_free(s);
            p->next = e;
        }
        else//删除的末尾
        {
            ebox_free(s);
            p->next = NULL;
        }
    }
    _size--;
    
    return 0;
}

void* List::data(int x)
{
    if(is_empty())return NULL;
    if(x > _size - 1) return NULL;//判断读取位置是否超出list的长度
    Node *p = _head;
    for(int i = 0; i < x; i++)
        p=p->next;
    return p->data;
}

Node* List::head()
{
    if(is_empty())return NULL;
    return _head;
}

Node* List::tail()
{
    if(is_empty())return NULL;
    Node *p = _head;
    for(int i = 1; i < _size; i++)
        p=p->next;
    return p;
}

int List::is_empty()
{
    return !_size;
}    
int  List::clear()
{
    Node *p = _head;
    while(_size > 0 )
    {
        _head = p->next;
        ebox_free(p);
        p = _head;
        _size--;
    }
    return 0;
}
int  List::modify_node(int x,void *data)
{
    Node *p = _head;
    for(int i = 0; i < x; i++)
    {
        p = p->next;
    }
    p->data = data;
    return 0;
}
int  List::swap(int x,int y)
{
    void *px,*py;
    if(is_empty())return NULL;
    px = data(y);
    py = data(x);
    modify_node(x,px);
    modify_node(y,py);
    return 0;
}

