
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
            本数据链表采用尾插法创建链表，数据起始位置为0。结束位置为size() - 1；
            head()，tail()中的数据为有效数据；
            Node.data为指向任意缓冲区数据的指针。用户根据自己的需求进行类型转换
            可以是任意数据类型（可以使结构体，缓冲区，变量等）.
*/

int List::insert_head(const void *data)
{
    Node *node_new;
    if((node_new = (Node *)ebox_malloc(sizeof(Node))) == NULL )
        return -1;
    
    node_new->data = (Node *)data;
    node_new->next = NULL;
    
    if(_size == 0)
    {
        _head = node_new;    
    }
    else
    {
        node_new->next = _head;
        _head = node_new;

    }
    _size++;
    return 0;
}

int List::insert_tail(const void *data)
{
    Node *node_prev,*node_new;
    if((node_new = (Node *)ebox_malloc(sizeof(Node))) == NULL )
        return -1;
    
    node_new->data = (Node *)data;
    node_new->next = NULL;
    
    if(_size == 0)
    {
        _head = node_new;    
    }
    else
    {
        node_prev = _head;
        
        while(NULL != node_prev->next)
        {

            node_prev = node_prev->next;
        }
        node_prev->next = node_new;
    }
    _size++;
    return 0;
}

int List::insert(int at,const void *data)
{
    Node *node_prev,*node_at,*node_new;
    int pos_at;
    int found = 0;
    if(is_empty() || at < 0 ) return -1;
    if((node_new = (Node *)ebox_malloc(sizeof(Node))) == NULL )
        return -1;
    
    node_new->data = (Node *)data;
    node_new->next = NULL;

    if(at == 0)
    {
        insert_head(data);   return 0; 
    }
    else 
    {
        node_prev = _head;
        node_at = node_prev->next;
        pos_at = 0;
        
        while(NULL != node_at)
        {
            if(pos_at == at - 1)
            {
                found = 1;
                break;        
            }
            node_prev = node_at;
            node_at = node_at->next;
            pos_at++;    
        }
        
        if(found)
        {
            node_new->next = node_at;
            node_prev->next = node_new;
        }
        else
        {
            node_prev->next = node_new;
        }
        _size++;
    }
    return 0;
}

int  List::remove(int at)
{
    Node *node_prev,*node_at,*e;
    int pos_at;
    int found = 0;
    if(is_empty() || at < 0 ) return -1;
    
    node_prev = _head;
    node_at = node_prev->next;
    pos_at = 0;
    
    if(at == 0)//删除链表头部
    {
        node_prev = node_prev->next;
        ebox_free(_head);
        if(_size == 1)
        {
            _head = NULL;
        }
        else
        {
            _head = node_prev;
        }
    }
    else//删除其他位置
    {
        //查找位置
        while(NULL != node_at)
        {
            if(pos_at == at - 1)
            {
                found = 1;
                break;        
            }
            node_prev = node_at;
            node_at = node_at->next;
            pos_at++;    
        }
        
        if(found)
        {
            node_prev->next = node_at->next;
            ebox_free(node_at);        
        }
        else
        {
            return -1;
        }

    }
    _size--;
    
    return 0;
}

void* List::data(int at)
{
    if(is_empty())return NULL;
    if(at > _size - 1) return NULL;//判断读取位置是否超出list的长度
    Node *p = _head;
    for(int i = 0; i < at; i++)
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
    Node *node_tail = _head;
    while(NULL != node_tail->next)
    {
        node_tail = node_tail->next;
    }
    return node_tail;
}

int List::is_empty()
{
    return !_size;
}    
int  List::clear()
{
    Node *p = _head;
    while(NULL != p)
    {
        _head = p->next;
        ebox_free(p);
        p = _head;
        _size--;
    }
    return 0;
}
int  List::modify_node(int at,void *data)
{
    Node *p = _head;
    for(int i = 0; i < at; i++)
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

