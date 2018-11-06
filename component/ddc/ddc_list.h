#ifndef __DDC_LIST_H
#define __DDC_LIST_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ebox_type.h"


typedef struct DdcNode
{
    uint8_t         time;
    uint8_t         *p;
    struct DdcNode  *next;
} DdcNode_t;

// create a new node with a value
DdcNode_t  *list_creat_node(uint8_t *p);
void        list_insert(DdcNode_t *head, uint8_t *p);
void        list_delete_by_val(DdcNode_t *head, uint16_t id);
uint16_t    list_get_size(DdcNode_t *head);
void        list_free(DdcNode_t *head);


#ifdef __cplusplus
}
#endif

#endif
