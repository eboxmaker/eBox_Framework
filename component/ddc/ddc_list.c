#include "ddc_list.h"
#include "ebox_cfun.h"

DdcNode_t *list_creat_node(uint8_t *p)
{
    DdcNode_t *newNode;
    newNode = (DdcNode_t *)ebox_malloc(sizeof(DdcNode_t));
    if (newNode == NULL)
    {
        //printf("out of memory!\n");
        return NULL;
    }
    else
    {
        //        ebox_printf("[list]creat list\t get free:%d\r\n",ebox_get_free());
        newNode->p = p;
        newNode->time = 0;
        newNode->next = NULL;
        return newNode;
    }
}

void list_insert(DdcNode_t *head, uint8_t *p)
{
    DdcNode_t *new_node = list_creat_node(p);
    DdcNode_t *pcur = head;

    while(pcur->next != NULL)
        pcur = pcur->next;
    pcur->next = new_node;


}

void list_delete_by_val(DdcNode_t *head, uint16_t id)
{
    Data16_t temp;
    //find target node and its precursor
    DdcNode_t *cur = head->next;
    DdcNode_t *pre = head;
    temp.value = id;

    if (head->next == NULL)
    {
        //ebox_printf("empty list!\n");
        return;
    }

    while(cur)
    {
        if ((cur->p[2] == temp.byte[0]) && (cur->p[3] == temp.byte[1]))
        {
            //delete target node
            pre->next = cur->next;
            //            ebox_printf("[list...]id=%d delete success\t get free:%d\r\n",temp.value,ebox_get_free());
            ebox_free(cur->p);
            //            ebox_printf("free frame buffer ok!get free = %d\r\n",ebox_get_free());
            ebox_free(cur);
            //            ebox_printf("free list buffer ok!get free = %d\r\n",ebox_get_free());
            break;
        }
        else
        {
            cur = cur->next;
            pre = pre->next;
        }
    }


}
uint16_t list_get_size(DdcNode_t *head)
{
    DdcNode_t *temp;
    unsigned int i;
    for ( temp = head; temp != NULL; temp = head->next)
    {
        i++;
    }
    return i;
}
void list_free(DdcNode_t *head)
{
    DdcNode_t *temp;
    for ( temp = head; temp != NULL; temp = head->next)
    {
        head = head->next;
        ebox_free(temp->p);
        ebox_free(temp);
    }
}
