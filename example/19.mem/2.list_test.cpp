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

int table[10]={1,2};
List l;
Node *node;
int *p;
int count = 0;


void setup()
{
    ebox_init();
    uart1.begin(115200);
}
int main(void)
{
    setup();
    uart1.printf("%d\n",node);
    for(int i = 0; i < 10; i++)
        table[i] = i;
    for(int i = 0; i < 10; i++)
    {
        l.insert(i,&table[i]);
    }
    l.remove(5);
    l.insert(6,&table[5]);

    while(1)
    {
        p = (int *)l.data(count);
        uart1.printf("table[%d] = %d\n",count,*p);
        count++;
        if(count == l.size())
        {
            node = l.head();
            uart1.printf("head.data = %d\n",*((int *)node->data));
            node = l.tail();
            uart1.printf("tail.data = %d\n",*((int *)node->data));
            while(1);
        }
    }

}


