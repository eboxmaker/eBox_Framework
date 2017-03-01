
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
#include "list.h"
/**
 *@brief    List��������
            �������������β�巨��������ʼλ��Ϊ0������λ��Ϊsize() - 1��
            head()��tail()�е�����Ϊ��Ч���ݣ�
            Node.dataΪָ�����⻺�������ݵ�ָ�롣�û������Լ��������������ת��
            �����������������ͣ�����ʹ�ṹ�壬�������������ȣ�.
*/


int List::insert(int x,const void *data)
{
    Node *new_node,*p,*e;
    if(x > _size ) return -1;//�жϲ���λ���Ƿ񳬳�list�ĳ���
    if((new_node = (Node *)ebox_malloc(sizeof(Node))) == NULL )
        return -1;
    new_node->data = (Node *)data;
    new_node->next = NULL;
    p = _head;
    if(x == 0)//���뵽����ͷ��
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
        for(int i = 1; i < x; i++)//Ѱ��Ҫ�����λ��
            p = p->next;//pΪ������ǰһ��λ��
        if(x < _size )//����������м�λ��
        {
            e = p->next;//eΪ�����ĺ�һ��λ��
            p->next = new_node;//����һ��λ��ָ���µ�λ��
            new_node->next= e;//���µ�λ�õ���һ��λ��ָ��e
        }
        else//���������ĩβ
        {
            p->next = new_node;//����һ��λ��ָ���µ�λ��
            new_node->next= NULL;//���µ�λ�õ���һ��λ��ָ��NULL
        }
    }
    _size++;
    return 0;
}
int  List::remove(int x)
{
    Node *new_node,*p,*s,*e;
    if(x >= _size ) return -1;//�ж�ɾ��λ���Ƿ񳬳�list�ĳ���
    p = _head;
    if(x == 0)//ɾ������ͷ��
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
        for(int i = 1; i < x; i++)//Ѱ��Ҫɾ����λ�õ�ǰһ��Node
        {
            p = p->next;//pΪɾ�����ǰһ��λ��
        }
        s = p->next;//sΪɾ�����λ��
        
        if(x < (_size - 1))//ɾ�������м�λ��
        {
            e = p->next->next;//eΪɾ����ĺ�һ��λ��
            ebox_free(s);
            p->next = e;
        }
        else//ɾ����ĩβ
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
    if(x > _size - 1) return NULL;//�ж϶�ȡλ���Ƿ񳬳�list�ĳ���
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

