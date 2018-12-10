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
#include "ebox_mem.h"
/**
	*	1	动态内存碎片化测试例程
    *   2   block数量越多说明内存碎片化越严重
	*/
/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"动态内存碎片化测试例程"
#define EXAMPLE_DATE	"2018-08-20"

uint8_t *ptr;
uint8_t *ptx[100];
void setup()
{
    ebox_init();
    uart1.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);

    uart1.printf("\r\n");
    uart1.printf("----------------mem test--------------------\r\n");
    uart1.printf("mem :0x%x---0x%x(%dK%dbytes)\r\n", ebox_get_heap_start_addr(), ebox_get_heap_end_addr(), (ebox_get_free()) / 1024, (ebox_get_free()) % 1024);
    int     i = 0;

    delay_ms(100);
    uart1.printf("ptr ID:\t\tADDRE\t\t|free \r\n");
    for( i = 0; i < 100; i++)
    {
        ptx[i] = (uint8_t *)ebox_malloc(100);
        if(ptx[i] == NULL)
            uart1.printf("game over\r\n");


        uart1.printf("ptr %03d:\t@0X%X\t|%05d Byte\r\n", i, ptx[i], ebox_get_free());

    }



    uart1.printf("\r\n===============================\r\n");
    uart1.printf("释放一半(只包含偶数)的指针");
    for( i = 0; i < 100;  i +=   2)
    {
        ebox_free(ptx[i]);
    }
    ebox_free_block_print();


    uart1.printf("\r\n===============================\r\n");
    uart1.printf("释放另一半的指针");
    for( i = 1; i < 100;  i +=   2)
    {

        ebox_free(ptx[i]);
    }
    ebox_free_block_print();

}
int main(void)
{
    setup();
    while(1)
    {
        delay_ms(1000);
    }

}


