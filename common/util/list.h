#ifndef __LIST_H
#define __LIST_H
#include "ebox.h"

typedef struct _node
{
 void           *data;
 struct _node   *next;
}Node;

class List
{
public:
    Node *head;
    Node *tail;
    int list_size;

    List(){head = NULL;tail = NULL;}
    int  insert(Node *node,const void *data);
    int  del(Node *node);
    Node *next(Node *node);
    void *data(Node *node);
    Node *output();
    Node *Gethead(){return head;}
};
#endif