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
    uart1.printf("test1\n",node);
    uart1.printf("test2\n",node);
    for(int i = 0; i < 10; i++)
        table[i] = i;
    for(int i = 0; i < 10; i++)
    {
        l.insert_tail(&table[i]);
    }
//    l.remove(9);
//    l.insert(2,&table[3]);
//    l.insert(2,&table[5]);
//    l.modify_node(7,&table[2]);
    l.swap(5,6);
//    l.clear();

    while(1)
    {
        if((p = (int *)l.data(count)) != NULL)
        {
            uart1.printf("table[%d] = %d\n",count,*p);
            count++;
        }
        else
            uart1.printf("DATA NULL\n",count,*p);

        if(count >= l.size())
        {
            
            if((node = l.head()) != NULL)
            {
                uart1.printf("head.data = %d\n",*((int *)node->data));
            }
            else
                uart1.printf("head NULL\n",count,*p);
            
            if((node = l.tail()) != NULL)
                uart1.printf("tail.data = %d\n",*((int *)node->data));
            else
                uart1.printf("tail NULL\n",count,*p);
                
            while(1);
        }
    }

}


