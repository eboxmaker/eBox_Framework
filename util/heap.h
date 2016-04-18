/******************************************************************************
 * fMalloc.c - Input the information of file:
 * 
 * Copyright (c) 2014-2099  KO
 * 
 * DESCRIPTION: - ����FreeRTOS��heap_4.c�����ڴ����
 *     Input the description of file:
 * Author: ������
 * date:2015/12/22
 * ת����ַ��http://www.amobbs.com/forum.php?mod=viewthread&tid=5640384&highlight=%E5%86%85%E5%AD%98%E7%AE%A1%E7%90%86
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

/*�ڴ��������*/
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

#define COUT_OF_MEM                 (1)                 //�ж��ٿ��ڴ���д1��2��3

//�ڴ�����
typedef enum
{
    SRAM_NONE   = (uint8_t)0,
#if (EN_SRAM_IN == 1)
    SRAM_IN,                                             //�ڲ��ڴ��
#endif

#if (EN_SRAM_EX == 1)
    SRAM_EX,                                            //�ⲿ�ڴ��
#endif

#if (EN_SRAM_CCM == 1)
    SRAM_CCM,                                            //CCM �ڴ��(�˲��� SRAM ���� CPU ���Է���!!!)
#endif
}E_MemType;

#ifndef OS_ENTER_CRITICAL
    #define OS_ENTER_CRITICAL()                         //��д����ϵͳ��ش���
#endif

#ifndef OS_EXIT_CRITICAL
    #define OS_EXIT_CRITICAL()                            //��д����ϵͳ��ش���
#endif

#ifndef configUSE_MALLOC_FAILED_HOOK
	#define configUSE_MALLOC_FAILED_HOOK 0              //�ڴ����ʧ�ܹ��Ӻ���
#endif

#ifndef configAPPLICATION_ALLOCATED_HEAP
	#define configAPPLICATION_ALLOCATED_HEAP 0          //�ڴ�����ⲿ�ļ����廹����fMalloc.c�ж���      
#endif

#define configTOTAL_HEAP_SIZE1		( ( size_t ) ( 10 * 1024 ) )//�ڲ��ڴ�ش�С
#define configTOTAL_HEAP_SIZE2		( ( size_t ) ( 500 * 1024 ) )//�ⲿ�ڴ�ش�С
#define configTOTAL_HEAP_SIZE3		( ( size_t ) ( 60 * 1024 ) )//CCM�ڴ�ش�С

#define portBYTE_ALIGNMENT          (8)                 //�ֽڶ���

/* Allocate the memory for the heap. */
#if( configAPPLICATION_ALLOCATED_HEAP == 1 )
	/* The application writer has already defined the array used for the RTOS
	heap - probably so it can be placed in a special segment or address. */
	extern uint8_t ucHeap[ configTOTAL_HEAP_SIZE ];
#endif /* configAPPLICATION_ALLOCATED_HEAP */

#ifndef mtCOVERAGE_TEST_MARKER
	#define mtCOVERAGE_TEST_MARKER()
#endif

#define configASSERT( x )       assert_param(x)      //����

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


//�û��ӿ�
//E_MemType-1����������
#define mem_malloc(xWantedSize,eMemType)    pvPortMalloc(xWantedSize,eMemType-1)       
#define mem_free(pv,eMemType)               do{vPortFree(pv,eMemType-1);pv = NULL;}while(0)
#define mem_get_free_size(eMemType)           xPortGetFreeHeapSize(eMemType-1)


#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif /* __F_MALLOC_H */


