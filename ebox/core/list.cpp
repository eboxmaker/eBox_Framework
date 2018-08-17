
/**
  ******************************************************************************
  * @file    list.cpp
  * @author  shentq
  * @version V1.0
  * @date    2017/03/02
  * @brief   List��������
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
#include "ebox_mem.h"
#include "list.h"
/**
 *@brief    List��������
            �������������β�巨��������������ʼλ��Ϊ0������λ��Ϊsize() - 1��
            head()��tail()�е�����Ϊ��Ч���ݣ�
            Node.dataΪָ�����⻺�������ݵ�ָ�롣�û������Լ��������������ת��
            �����������������ͣ�����ʹ�ṹ�壬�������������ȣ�.
*/

int List::insert_head(void *data)
{
    Node *node_new;
    if((node_new = (Node *)ebox_malloc(sizeof(Node))) == NULL )
        return -1;
    
    node_new->data = data;
    node_new->next = _head;
    _head = node_new;
    
    if(_size == 0)
    {
        _tail = node_new;
    }
    _size++;
    return 0;
}

int List::insert_tail(void *data)
{
    Node *node_new;
    if((node_new = (Node *)ebox_malloc(sizeof(Node))) == NULL )
        return -1;
    
    node_new->data = (Node *)data;
    node_new->next = NULL;
    
    if(_size == 0)
    {
        _head = _tail = node_new;    
    }
    else
    {
        _tail->next = node_new;
        _tail = node_new;
    }
    _size++;
    return 0;
}

int List::insert(int at,void *data)
{
    Node *node_prev,*node_at,*node_new;
    int pos_at;
    int found = 0;
    if(is_empty() || at < 0 ) return -1;
    //1�������ڴ棬�������
    if((node_new = (Node *)ebox_malloc(sizeof(Node))) == NULL )
        return -1;
    
    node_new->data = data;
    node_new->next = NULL;

    //2������λ��
    if(at == 0)//�����0λ�ã���ʾ��ͷ����
    {
        //3����������
        insert_head(data);   return 0; 
    }
    else //����λ��
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
        //3����������
        
        if(found)//�м�λ��
        {
            node_new->next = node_at;
            node_prev->next = node_new;
        }
        else//β��λ��
        {
            node_prev->next = node_new;
            _tail = node_new;
        }
        _size++;
    }
    return 0;
}

int  List::remove(int at)
{
    Node *node_prev,*node_at;
    int pos_at;
    int found = 0;
    if(is_empty() || at < 0 ) return -1;
    
    node_prev = _head;
    node_at = node_prev->next;
    pos_at = 0;
    
    if(at == 0)//ɾ������ͷ��
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
    else//ɾ������λ��
    {
        //����λ��
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
    if(at > _size - 1) return NULL;//�ж϶�ȡλ���Ƿ񳬳�list�ĳ���
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
//    if(is_empty())return NULL;
//    Node *node_tail = _head;
//    while(NULL != node_tail->next)
//    {
//        node_tail = node_tail->next;
//    }
    return _tail;
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

