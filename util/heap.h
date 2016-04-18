/******************************************************************************
 * fMalloc.c - Input the information of file:
 * 
 * Copyright (c) 2014-2099  KO
 * 
 * DESCRIPTION: - 参照FreeRTOS的heap_4.c做的内存分配
 *     Input the description of file:
 * Author: 负西弱
 * date:2015/12/22
 * 转载网址：http://www.amobbs.com/forum.php?mod=viewthread&tid=5640384&highlight=%E5%86%85%E5%AD%98%E7%AE%A1%E7%90%86
 * 
 * modification history
 *   ...
 ******************************************************************************/
#ifndef __F_MALLOC_H
#define __F_MALLOC_H    

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus
    

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include <stdlib.h>

/*内存分配配置*/
#define EN_SRAM_IN                  (1)
#define EN_SRAM_EX                  (0)
#define EN_SRAM_CCM                 (0)

#ifndef EN_SRAM_IN
    #define EN_SRAM_IN              (1)
#endif
#ifndef EN_SRAM_EX
    #define EN_SRAM_EX              (1)
#endif
#ifndef EN_SRAM_CCM
    #define EN_SRAM_CCM             (1)
#endif

#define COUT_OF_MEM                 (1)                 //有多少块内存填写1、2、3

//内存种类
typedef enum
{
    SRAM_NONE   = (uint8_t)0,
#if (EN_SRAM_IN == 1)
    SRAM_IN,                                             //内部内存池
#endif

#if (EN_SRAM_EX == 1)
    SRAM_EX,                                            //外部内存池
#endif

#if (EN_SRAM_CCM == 1)
    SRAM_CCM,                                            //CCM 内存池(此部分 SRAM 仅仅 CPU 可以访问!!!)
#endif
}E_MemType;

#ifndef OS_ENTER_CRITICAL
    #define OS_ENTER_CRITICAL()                         //填写操作系统相关代码
#endif

#ifndef OS_EXIT_CRITICAL
    #define OS_EXIT_CRITICAL()                            //填写操作系统相关代码
#endif

#ifndef configUSE_MALLOC_FAILED_HOOK
	#define configUSE_MALLOC_FAILED_HOOK 0              //内存分配失败钩子函数
#endif

#ifndef configAPPLICATION_ALLOCATED_HEAP
	#define configAPPLICATION_ALLOCATED_HEAP 0          //内存池在外部文件定义还是在fMalloc.c中定义      
#endif

#define configTOTAL_HEAP_SIZE1		( ( size_t ) ( 10 * 1024 ) )//内部内存池大小
#define configTOTAL_HEAP_SIZE2		( ( size_t ) ( 500 * 1024 ) )//外部内存池大小
#define configTOTAL_HEAP_SIZE3		( ( size_t ) ( 60 * 1024 ) )//CCM内存池大小

#define portBYTE_ALIGNMENT          (8)                 //字节对齐

/* Allocate the memory for the heap. */
#if( configAPPLICATION_ALLOCATED_HEAP == 1 )
	/* The application writer has already defined the array used for the RTOS
	heap - probably so it can be placed in a special segment or address. */
	extern uint8_t ucHeap[ configTOTAL_HEAP_SIZE ];
#endif /* configAPPLICATION_ALLOCATED_HEAP */

#ifndef mtCOVERAGE_TEST_MARKER
	#define mtCOVERAGE_TEST_MARKER()
#endif

#define configASSERT( x )       assert_param(x)      //断言

/* Define the linked list structure.  This is used to link free blocks in order
of their memory address. */
typedef struct A_BLOCK_LINK
{
	struct A_BLOCK_LINK *pxNextFreeBlock;	/*<< The next free block in the list. */
	size_t xBlockSize;						/*<< The size of the free block. */
} BlockLink_t;

extern size_t configTOTAL_HEAP_SIZE[];

void *pvPortMalloc( size_t xWantedSize,uint8_t eMemType );
void vPortFree( void *pv,uint8_t eMemType );
size_t xPortGetFreeHeapSize( uint8_t eMemType );


//用户接口
//E_MemType-1才是数组编号
#define mem_malloc(xWantedSize,eMemType)    pvPortMalloc(xWantedSize,eMemType-1)       
#define mem_free(pv,eMemType)               do{vPortFree(pv,eMemType-1);pv = NULL;}while(0)
#define mem_get_free_size(eMemType)           xPortGetFreeHeapSize(eMemType-1)


#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif /* __F_MALLOC_H */


