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
#ifndef __E_HEAP_H
#define __E_HEAP_H    

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
    #define EN_SRAM_EX              (0)
#endif
#ifndef EN_SRAM_CCM
    #define EN_SRAM_CCM             (0)
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
}MemType_t;

#ifndef OS_ENTER_CRITICAL
    #define OS_ENTER_CRITICAL()                         //��д����ϵͳ��ش���
#endif

#ifndef OS_EXIT_CRITICAL
    #define OS_EXIT_CRITICAL()                            //��д����ϵͳ��ش���
#endif

#ifndef cfgUSE_MALLOC_FAILED_HOOK
	#define cfgUSE_MALLOC_FAILED_HOOK 0              //�ڴ����ʧ�ܹ��Ӻ���
#endif

#ifndef cfgAPPLICATION_ALLOCATED_HEAP
	#define cfgAPPLICATION_ALLOCATED_HEAP 0          //�ڴ�����ⲿ�ļ����廹����fMalloc.c�ж���      
#endif

#define cfgTOTAL_HEAP_SIZE1		( ( size_t ) ( 5000 ) )//�ڲ��ڴ�ش�С
#define cfgTOTAL_HEAP_SIZE2		( ( size_t ) ( 500 * 1024 ) )//�ⲿ�ڴ�ش�С
#define cfgTOTAL_HEAP_SIZE3		( ( size_t ) ( 60 * 1024 ) )//CCM�ڴ�ش�С

#define BYTE_ALIGNMENT          (8)                 //�ֽڶ���

/* Allocate the memory for the heap. */
#if( cfgAPPLICATION_ALLOCATED_HEAP == 1 )
	/* The application writer has already defined the array used for the RTOS
	heap - probably so it can be placed in a special segment or address. */
	extern uint8_t memHeap[ cfgTOTAL_HEAP_SIZE ];
#endif /* cfgAPPLICATION_ALLOCATED_HEAP */

#ifndef COVERAGE_TEST_MARKER
	#define COVERAGE_TEST_MARKER()
#endif

#define cfgASSERT( x )       assert_param(x)      //����

/* Define the linked list structure.  This is used to link free blocks in order
of their memory address. */
typedef struct BLOCK_LINK
{
	struct BLOCK_LINK *nextFreeBlock;	/*<< The next free block in the list. */
	size_t blockSize;						/*<< The size of the free block. */
} BlockLink_t;

extern size_t cfgTOTAL_HEAP_SIZE[];

void *memMalloc( size_t xWantedSize,uint8_t memType );
void memFree( void *pv,uint8_t memType );
size_t memGetFreeHeapSize( uint8_t memType );


//�û��ӿ�
//E_MemType-1����������
#define mem_malloc(xWantedSize,memType)    memMalloc(xWantedSize,memType-1)       
#define mem_free(pv,memType)               do{memFree(pv,memType-1);pv = NULL;}while(0)
#define mem_get_free_size(memType)           memGetFreeHeapSize(memType-1)


#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif /* __F_MALLOC_H */


