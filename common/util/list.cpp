#include "list.h"





int List::insert(Node *node,const void *data)
{
    Node *new_node;
    if((new_node = (Node *)ebox_malloc(sizeof(Node))) == NULL )
        return -1;
    new_node->data = (Node *)data;
    new_node->next = NULL;
    if(node == NULL)//插入到链表头部
    {
        if(list_size == 0)
            tail = new_node;
        new_node->next = tail;
        head = new_node;
    }
    else
    {
        if(node->next == NULL)
            tail = new_node;
        new_node->next = node->next;
        node->next = new_node;
    }
    list_size++;
    return 0;
}
Node* List::next(Node *node)
{
    return node->next;
}
void* List::data(Node *node)
{
    return node->data;
}

Node* List::output()
{
    

}