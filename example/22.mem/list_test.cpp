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
#include "bsp_ebox.h"
#include "list_c.h"

typedef struct student {
  char first_name[10];
  char last_name[10];
  unsigned int age;
  struct list_head node_student;
} student_t;



struct  intList {
    struct list_head list;
    int val;
};



void setup()
{
    int ret;
    ebox_init();
    uart1.begin(115200);
    print_log();
    PB8.mode(OUTPUT_PP);
    
    
    student_t *stu = NULL;
    
}
int main(void)
{
    int index = 0;
    int free;
    int i = 0;
    setup();
    uint32_t last = millis();
    
    struct list_head *pos = NULL;
    struct list_head *tmp = NULL;
    struct intList *pnode;
    
//    LIST_HEAD(mylist);
    struct list_head mylist;
    INIT_LIST_HEAD(&mylist);
    
    if(list_empty(&mylist))
    {
        uart1.printf("链表为空\n");
    }
    free = ebox_get_free();
    uart1.printf("free:%d\n",free); 
    for(int i = 0; i < 10; i++)
    {
        struct intList *ptr = (struct intList *)ebox_malloc(sizeof(intList));
        ptr->val = i;
        
        //插入链表尾部
        list_add_tail(&(ptr->list),&mylist);
     
        //链表遍历
        list_for_each(pos, &mylist) {
            pnode = list_entry(pos, struct intList, list);
            uart1.printf("%d ", pnode->val);
        }
        uart1.printf("\n");  
    }
    uart1.flush();
    free = ebox_get_free();
    uart1.printf("free:%d\n",free);    
    
    //查找链表中的元素
    uart1.printf("\n查找第一个val==2的链表位置\n");
    int location = 0;
    list_for_each(pos, &mylist) {
        pnode = list_entry(pos, struct intList, list);
        location++;
        if(pnode->val == 2)
        {
            uart1.printf("location=%d\n",location);
        }
        
    }
    uart1.printf("mylist len:%d\n",list_len(&mylist));  

    

    //删除链表中的元素
    uart1.printf("\n删除偶数位置的元素\n");
    index= 0 ;
    list_for_each(pos, &mylist) {
        pnode = list_entry(pos, struct intList, list);
        index++;
        if(index % 2 == 0)
        {
            tmp = pos;
            pos = pos->prev;
            list_del(tmp);
            ebox_free(pnode);
        }
        
    }
    //新的链表打印
    list_for_each(pos, &mylist) {
        pnode = list_entry(pos, struct intList, list);
        uart1.printf("%d ", pnode->val);
    }
    uart1.printf("\n");  
    uart1.printf("删除成功\n");
    uart1.flush();
    free = ebox_get_free();
    uart1.printf("free:%d\n",free);  
    
    
    uart1.printf("\n插入链表\n");
    struct intList addition;
    addition.val = 1;
    struct list_head *pre,*next;
    int state = 0;
    list_for_each(pos, &mylist) {
        pnode = list_entry(pos, struct intList, list);
        if(pnode->val == 0)
        {
            pre = pos;
            state = 1;
        }
        if(pnode->val == 2)
        {
            next = pos;
            state = 2;
        }
        if(state == 2)
        {
            __list_add(&addition.list,pre,next);
        }
    }
    //新的链表打印
    list_for_each(pos, &mylist) {
        pnode = list_entry(pos, struct intList, list);
        uart1.printf("%d ", pnode->val);
    }
    uart1.printf("\n");  

    
    uart1.printf("\n安全遍历链表\n");
    index= 0 ;
    list_for_each_safe(pos, tmp,&mylist)
    {
        index++;
        if(index == 2)
            list_del(pos);
    }
    //新的链表打印
    list_for_each(pos, &mylist) {
        pnode = list_entry(pos, struct intList, list);
        uart1.printf("%d ", pnode->val);
    }
    uart1.printf("\n");  
    
    
    uart1.printf("\n新建链表\n");
    //创建新的链表
    LIST_HEAD(list2);
    for(int i = 0; i < 10; i++)
    {
        struct intList *ptr = (struct intList *)ebox_malloc(sizeof(intList));
        ptr->val = 10+i;
        //插入链表尾部
        list_add_tail(&(ptr->list),&list2);
    }
    //新的链表打印
    list_for_each(pos, &list2) {
        pnode = list_entry(pos, struct intList, list);
        uart1.printf("%d ", pnode->val);
    }
    uart1.printf("\n");
    
    uart1.printf("\n拼接链表\n");
    list_splice(&list2,&mylist);
        //新的链表打印
    list_for_each(pos, &mylist) {
        pnode = list_entry(pos, struct intList, list);
        uart1.printf("%d ", pnode->val);
    }
    uart1.printf("\n");
    
    while(1)
    {
        delay_ms(1000);
    }

}


