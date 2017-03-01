/**
  ******************************************************************************
  * @file   : *.cpp
  * @author : shentq
  * @version: V1.2
  * @date   : 2016/08/14

  * @brief   ebox application example .
  *
  * Copyright 2016 shentq. All Rights Reserved.         
  ******************************************************************************
 */
 
 
#include "ebox.h"
#include "bsp.h"
#include "list.h"

List l;
Node x;
int table[10]={1,2};
void setup()
{
    ebox_init();
    PB7.mode(OUTPUT_PP);
    uart1.begin(115200);
    x.data = table;
}
    int *p;
    Node *node;
int main(void)
{
    setup();
    uart1.printf("%d\n",node);
     node = l.head;
   for(int i = 0; i < 10; i++)
        table[i] = i;
    node = l.head;
    for(int i = 0; i < 10; i++)
    {
        l.insert(node,&table[i]);
        uart1.printf("l.head = 0x%x\n",l.head);
        uart1.printf("l.head->next = 0x%x\n",l.head->next);
        node = l.next(node);
        uart1.printf("l.head = 0x%x\n",l.head);
        uart1.printf("node = 0x%x\n",node);
        uart1.printf("l.size = %d\n",l.list_size);
    }
    
    node = l.head;
    while(1)
    {
        p = (int *)l.data(node);
        uart1.printf("table[0] = %d\n",*p);
        node = l.next(node);
        if(node == NULL)
            while(1);
    }

}


