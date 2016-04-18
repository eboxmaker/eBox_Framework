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
#include "heap.h"

//�ڴ�ش�С
size_t configTOTAL_HEAP_SIZE[] = {
#if (EN_SRAM_IN == 1)
    configTOTAL_HEAP_SIZE1,     //�ڲ��ڴ�ش�С
#endif
#if (EN_SRAM_EX == 1)
    configTOTAL_HEAP_SIZE2,     //�ⲿ�ڴ�ش�С
#endif
#if (EN_SRAM_CCM == 1)
    configTOTAL_HEAP_SIZE3,     //CCM �ڴ�ش�С(�˲��� SRAM ���� CPU ���Է���!!!)
#endif
};

/* Block sizes must not get too small. */
#define heapMINIMUM_BLOCK_SIZE	( ( size_t ) ( xHeapStructSize << 1 ) )

/* Assumes 8bit bytes! */
#define heapBITS_PER_BYTE		( ( size_t ) 8 )

#if portBYTE_ALIGNMENT == 32
	#define portBYTE_ALIGNMENT_MASK ( 0x0000001f );
#endif
#if portBYTE_ALIGNMENT == 16
	#define portBYTE_ALIGNMENT_MASK ( 0x0000000f )
#endif
#if portBYTE_ALIGNMENT == 8
	#define portBYTE_ALIGNMENT_MASK ( 0x00000007 )
#endif

/* Allocate the memory for the heap. */
#if( configAPPLICATION_ALLOCATED_HEAP == 0 )
	/* The application writer has already defined the array used for the RTOS
	heap - probably so it can be placed in a special segment or address. */
	#if (EN_SRAM_IN == 1)
	    __align(8) static uint8_t ucMemSRAMIn[ configTOTAL_HEAP_SIZE1 ];
	#endif
	#if (EN_SRAM_EX == 1)
        __align(8) static uint8_t ucMemSRAMEx[ configTOTAL_HEAP_SIZE2 ]  __attribute__((at(0X68000000)));
    #endif
    #if (EN_SRAM_CCM == 1)
        __align(8) static uint8_t ucMemSRAMCCM[ configTOTAL_HEAP_SIZE3 ] __attribute__((at(0X10000000)));
    #endif
#endif /* configAPPLICATION_ALLOCATED_HEAP */

uint8_t *ucHeap[] = {
#if (EN_SRAM_IN == 1)
    ucMemSRAMIn,     //�ڲ��ڴ�ص�ַ
#endif
#if (EN_SRAM_EX == 1)
    ucMemSRAMEx,     //�ⲿ�ڴ�ص�ַ
#endif
#if (EN_SRAM_CCM == 1)
    ucMemSRAMCCM,   //CCM �ڴ�ص�ַ(�˲��� SRAM ���� CPU ���Է���!!!)
#endif
};
/*-----------------------------------------------------------*/

/* The size of the structure placed at the beginning of each allocated memory
block must by correctly byte aligned. */
//��xHeapStructSize�Ĵ�С�ֽڶ���
static const size_t xHeapStructSize	= ( sizeof( BlockLink_t ) + ( ( size_t ) ( portBYTE_ALIGNMENT - 1 ) ) ) & ~( ( size_t ) portBYTE_ALIGNMENT_MASK );

/* Create a couple of list links to mark the start and end of the list. */
static BlockLink_t xStart[COUT_OF_MEM];
static BlockLink_t *pxEnd[COUT_OF_MEM] = {
#if COUT_OF_MEM > 0
    NULL,
#endif
#if COUT_OF_MEM > 1
    NULL,
#endif
#if COUT_OF_MEM > 2
    NULL,
#endif
};

/* Keeps track of the number of free bytes remaining, but says nothing about
fragmentation. */
static size_t xFreeBytesRemaining[COUT_OF_MEM] = {
#if COUT_OF_MEM > 0
    0U,
#endif
#if COUT_OF_MEM > 1
    0U,
#endif
#if COUT_OF_MEM > 2
    0U,
#endif
};
static size_t xMinimumEverFreeBytesRemaining[COUT_OF_MEM] = {
#if COUT_OF_MEM > 0
    0U,
#endif
#if COUT_OF_MEM > 1
    0U,
#endif
#if COUT_OF_MEM > 2
    0U,
#endif
};

/* Gets set to the top bit of an size_t type.  When this bit in the xBlockSize
member of an BlockLink_t structure is set then the block belongs to the
application.  When the bit is free the block is still part of the free heap
space. */
static size_t xBlockAllocatedBit = 0;

/*-----------------------------------------------------------*/
/*
 * Inserts a block of memory that is being freed into the correct position in
 * the list of free memory blocks.  The block being freed will be merged with
 * the block in front it and/or the block behind it if the memory blocks are
 * adjacent to each other.
 */
static void prvInsertBlockIntoFreeList( BlockLink_t *pxBlockToInsert ,uint8_t eMemType);

/*
 * Called automatically to setup the required heap structures the first time
 * pvPortMalloc() is called.
 */
static void prvHeapInit( uint8_t eMemType );

void *pvPortMalloc( size_t xWantedSize ,uint8_t eMemType )
{
    BlockLink_t *pxBlock, *pxPreviousBlock, *pxNewBlockLink;
    void *pvReturn = NULL;

    if(eMemType >= COUT_OF_MEM)
    {
        return NULL;
    }
    
	OS_ENTER_CRITICAL();
	{
		/* If this is the first call to malloc then the heap will require
		initialisation to setup the list of free blocks. */
		/*����ʱ��û�г�ʼ�����ȳ�ʼ��*/
		if( pxEnd[eMemType] == NULL )
		{
			prvHeapInit(eMemType);
		}
		else
		{
			mtCOVERAGE_TEST_MARKER();
		}

		/* Check the requested block size is not so large that the top bit is
		set.  The top bit of the block size member of the BlockLink_t structure
		is used to determine who owns the block - the application or the
		kernel, so it must be free. */
		/*����BlockLink_t��xBlockSize�����һλ����ʶĳ���ڴ���Ƿ��ڿ���״̬,
		ÿһ�������ȥ���ڴ���С���������ˡ����磬���õ���STM32F103��size_t��
		����Ϊunsigned int���͵ģ�32λ����֧�ֵ�4G���ڴ�ռ䡣�������1λ����ָ
		ʾ�ռ�״̬�Ļ����Ǿ�ֻ��31λȥ��ʶ�ڴ���ַ����ֻ֧�ֵ�2G���ڴ�ռ�*/
		if( ( xWantedSize & xBlockAllocatedBit ) == 0 )
		{
			/* The wanted size is increased so it can contain a BlockLink_t
			structure in addition to the requested amount of bytes. */
			if( xWantedSize > 0 )
			{
				xWantedSize += xHeapStructSize;                 //ʵ�ʴ�С=�����С+1��BlockLink_t�Ĵ�С

				/* Ensure that blocks are always aligned to the required number
				of bytes. */
				/*��֤����ĵĴ�С�ֽڶ���*/
				if( ( xWantedSize & portBYTE_ALIGNMENT_MASK ) != 0x00 )
				{
					/* Byte alignment required. */
					xWantedSize += ( portBYTE_ALIGNMENT - ( xWantedSize & portBYTE_ALIGNMENT_MASK ) );
					configASSERT( ( xWantedSize & portBYTE_ALIGNMENT_MASK ) == 0 );
				}
				else
				{
					mtCOVERAGE_TEST_MARKER();
				}
			}
			else
			{
				mtCOVERAGE_TEST_MARKER();
			}

            /*�����СС��ʵ�ʿ��ã����Է���*/
			if( ( xWantedSize > 0 ) && ( xWantedSize <= xFreeBytesRemaining[eMemType] ) )
			{
				/* Traverse the list from the start	(lowest address) block until
				one	of adequate size is found. */
				/*Ѱ��һ����������С���ڴ��*/
				pxPreviousBlock = &(xStart[eMemType]);
				pxBlock = xStart[eMemType].pxNextFreeBlock;
				while( ( pxBlock->xBlockSize < xWantedSize ) && ( pxBlock->pxNextFreeBlock != NULL ) )
				{
					pxPreviousBlock = pxBlock;
					pxBlock = pxBlock->pxNextFreeBlock;
				}

				/* If the end marker was reached then a block of adequate size
				was	not found. */
				/*�ҵ������С���ڴ����*/
				if( pxBlock != pxEnd[eMemType] )
				{
					/* Return the memory space pointed to - jumping over the
					BlockLink_t structure at its start. */
					pvReturn = ( void * ) ( ( ( uint8_t * ) pxPreviousBlock->pxNextFreeBlock ) + xHeapStructSize );

					/* This block is being returned for use so must be taken out
					of the list of free blocks. */
					pxPreviousBlock->pxNextFreeBlock = pxBlock->pxNextFreeBlock;

					/* If the block is larger than required it can be split into
					two. */
					/*��������ȥ�Ŀ��п������Ŀռ��ܶ࣬�򽫸ÿ��п���зָ��ʣ��Ĳ���
					������ӵ������С�*/
					if( ( pxBlock->xBlockSize - xWantedSize ) > heapMINIMUM_BLOCK_SIZE )
					{
						/* This block is to be split into two.  Create a new
						block following the number of bytes requested. The void
						cast is used to prevent byte alignment warnings from the
						compiler. */
						pxNewBlockLink = ( void * ) ( ( ( uint8_t * ) pxBlock ) + xWantedSize );
						configASSERT( ( ( ( size_t ) pxNewBlockLink ) & portBYTE_ALIGNMENT_MASK ) == 0 );

						/* Calculate the sizes of two blocks split from the
						single block. */
						pxNewBlockLink->xBlockSize = pxBlock->xBlockSize - xWantedSize;
						pxBlock->xBlockSize = xWantedSize;

						/* Insert the new block into the list of free blocks. */
						prvInsertBlockIntoFreeList( ( pxNewBlockLink ),eMemType );
					}
					else
					{
						mtCOVERAGE_TEST_MARKER();
					}

					xFreeBytesRemaining[eMemType] -= pxBlock->xBlockSize;

					if( xFreeBytesRemaining[eMemType] < xMinimumEverFreeBytesRemaining[eMemType] )
					{
						xMinimumEverFreeBytesRemaining[eMemType] = xFreeBytesRemaining[eMemType];
					}
					else
					{
						mtCOVERAGE_TEST_MARKER();
					}

					/* The block is being returned - it is allocated and owned
					by the application and has no "next" block. */
					pxBlock->xBlockSize |= xBlockAllocatedBit;
					pxBlock->pxNextFreeBlock = NULL;
				}
				else
				{
					mtCOVERAGE_TEST_MARKER();
				}
			}
			else
			{
				mtCOVERAGE_TEST_MARKER();
			}
		}
		else
		{
			mtCOVERAGE_TEST_MARKER();
		}

		//traceMALLOC( pvReturn, xWantedSize );
	}
	OS_EXIT_CRITICAL();

    /*�ڴ����ʧ��ʱ���õĹ��Ӻ���*/
	#if( configUSE_MALLOC_FAILED_HOOK == 1 )
	{
		if( pvReturn == NULL )
		{
			extern void vApplicationMallocFailedHook( void );
			vApplicationMallocFailedHook();
		}
		else
		{
			mtCOVERAGE_TEST_MARKER();
		}
	}
	#endif

	configASSERT( ( ( ( uint32_t ) pvReturn ) & portBYTE_ALIGNMENT_MASK ) == 0 );
	return pvReturn;
}
/*-----------------------------------------------------------*/

void vPortFree( void *pv ,uint8_t eMemType)
{
    uint8_t *puc = ( uint8_t * ) pv;
    BlockLink_t *pxLink;

    if(eMemType >= COUT_OF_MEM)
    {
        return;
    }
    
	if( pv != NULL )
	{
		/* The memory being freed will have an BlockLink_t structure immediately
		before it. */
		puc -= xHeapStructSize;

		/* This casting is to keep the compiler from issuing warnings. */
		pxLink = ( void * ) puc;

		/* Check the block is actually allocated. */
		configASSERT( ( pxLink->xBlockSize & xBlockAllocatedBit ) != 0 );
		configASSERT( pxLink->pxNextFreeBlock == NULL );

		if( ( pxLink->xBlockSize & xBlockAllocatedBit ) != 0 )
		{
			if( pxLink->pxNextFreeBlock == NULL )
			{
				/* The block is being returned to the heap - it is no longer
				allocated. */
				pxLink->xBlockSize &= ~xBlockAllocatedBit;

				OS_ENTER_CRITICAL();
				{
					/* Add this block to the list of free blocks. */
					xFreeBytesRemaining[eMemType] += pxLink->xBlockSize;
					//traceFREE( pv, pxLink->xBlockSize );
					prvInsertBlockIntoFreeList( ( ( BlockLink_t * ) pxLink ),eMemType );
				}
				OS_EXIT_CRITICAL();
			}
			else
			{
				mtCOVERAGE_TEST_MARKER();
			}
		}
		else
		{
			mtCOVERAGE_TEST_MARKER();
		}
	}
}
/*-----------------------------------------------------------*/

size_t xPortGetFreeHeapSize( uint8_t eMemType )
{
    if(eMemType >= COUT_OF_MEM)
    {
        return 0;
    }
    else
    {
	    return xFreeBytesRemaining[eMemType];
	}
}
/*-----------------------------------------------------------*/

size_t xPortGetMinimumEverFreeHeapSize( uint8_t eMemType )
{
    if(eMemType >= COUT_OF_MEM)
    {
        return 0;
    }
    {
	    return xMinimumEverFreeBytesRemaining[eMemType];
	}
}
/*-----------------------------------------------------------*/

void vPortInitialiseBlocks( void )
{
	/* This just exists to keep the linker quiet. */
}
/*-----------------------------------------------------------*/

/******************************************************************************
 * FUNCTION: prvHeapInit ( )
 * DESCRIPTION: �ڴ�س�ʼ��
 *    Input the description of function: 
 * Input Parameters: 
 * Output Parameters: 
 * Returns Value: 
 * 
 * Author: FuDongQiang @ 2015/12/22
 * 
 * modification history
 *   ...
 ******************************************************************************/
static void prvHeapInit(uint8_t eMemType)
{
    BlockLink_t *pxFirstFreeBlock;
    uint8_t *pucAlignedHeap;
    size_t uxAddress;
    size_t xTotalHeapSize = configTOTAL_HEAP_SIZE[eMemType];

	/* Ensure the heap starts on a correctly aligned boundary. */
	/*��֤�ڴ���׵�ַ8�ֽڶ���*/
	uxAddress = ( size_t ) (ucHeap[eMemType]);

	if( ( uxAddress & portBYTE_ALIGNMENT_MASK ) != 0 )
	{
		uxAddress += ( portBYTE_ALIGNMENT - 1 );
		uxAddress &= ~( ( size_t ) portBYTE_ALIGNMENT_MASK );
		xTotalHeapSize -= uxAddress - ( size_t ) (ucHeap[eMemType]);
	}

	pucAlignedHeap = ( uint8_t * ) uxAddress;

	/* xStart is used to hold a pointer to the first item in the list of free
	blocks.  The void cast is used to prevent compiler warnings. */
	xStart[eMemType].pxNextFreeBlock = ( void * ) pucAlignedHeap;             //ָ���һ�������ڴ��
	xStart[eMemType].xBlockSize = ( size_t ) 0;

	/* pxEnd is used to mark the end of the list of free blocks and is inserted
	at the end of the heap space. */
	/*ָ�����ڴ�ѵ����һ��BlockLink_t��С��λ���ϡ�Ҳ����˵���ڴ�����Ŀռ��Ǵ洢��һ��BlockLink_t������ָʾ���п�������ս�*/
	uxAddress = ( ( size_t ) pucAlignedHeap ) + xTotalHeapSize;
	uxAddress -= xHeapStructSize;
	uxAddress &= ~( ( size_t ) portBYTE_ALIGNMENT_MASK );
	pxEnd[eMemType] = ( void * ) uxAddress;
	pxEnd[eMemType]->xBlockSize = 0;
	pxEnd[eMemType]->pxNextFreeBlock = NULL;

	/* To start with there is a single free block that is sized to take up the
	entire heap space, minus the space taken by pxEnd. */
	/*��ʼֻ��һ�������ڴ�飬��СΪ���п����ڴ�ش�С*/
	pxFirstFreeBlock = ( void * ) pucAlignedHeap;
	pxFirstFreeBlock->xBlockSize = uxAddress - ( size_t ) pxFirstFreeBlock;
	pxFirstFreeBlock->pxNextFreeBlock = pxEnd[eMemType];

	/* Only one block exists - and it covers the entire usable heap space. */
	xMinimumEverFreeBytesRemaining[eMemType] = pxFirstFreeBlock->xBlockSize;
	xFreeBytesRemaining[eMemType] = pxFirstFreeBlock->xBlockSize;

	/* Work out the position of the top bit in a size_t variable. */
	/*����BlockLink_t��xBlockSize�����һλ����ʶĳ���ڴ���Ƿ��ڿ���״̬*/
	xBlockAllocatedBit = ( ( size_t ) 1 ) << ( ( sizeof( size_t ) * heapBITS_PER_BYTE ) - 1 );
}
/*-----------------------------------------------------------*/

static void prvInsertBlockIntoFreeList( BlockLink_t *pxBlockToInsert ,uint8_t eMemType)
{
    BlockLink_t *pxIterator;
    uint8_t *puc;

	/* Iterate through the list until a block is found that has a higher address
	than the block being inserted. */
	for( pxIterator = &(xStart[eMemType]); pxIterator->pxNextFreeBlock < pxBlockToInsert; pxIterator = pxIterator->pxNextFreeBlock )
	{
		/* Nothing to do here, just iterate to the right position. */
	}

	/* Do the block being inserted, and the block it is being inserted after
	make a contiguous block of memory? */
	/*�����Ž�pxIterator��pxBlockToInsert���кϲ������Ժϲ��ı�׼ΪpxIterator��
	�׵�ַ����pxIterator�Ŀ��С֮�����pxBlockToInsert���׵�ַ����Ⱦ�˵������
	�������ڵ�.������ܺϲ�����ʲô�¶�������*/
	puc = ( uint8_t * ) pxIterator;
	if( ( puc + pxIterator->xBlockSize ) == ( uint8_t * ) pxBlockToInsert )
	{
		pxIterator->xBlockSize += pxBlockToInsert->xBlockSize;
		pxBlockToInsert = pxIterator;
	}
	else
	{
		mtCOVERAGE_TEST_MARKER();
	}

	/* Do the block being inserted, and the block it is being inserted before
	make a contiguous block of memory? */
	/*�����Ž�pxBlockToInsert��pxIteratorָ�����һ�����п���кϲ����ɺϲ��ı�
	׼�͸ո�˵��һ����ֻ�������pxBlockToInsert���׵�ַ����pxBlockToInsert�Ŀ��
	С��pxIteratorָ�����һ�����ַ�Ƚϡ��ܺϲ�����õģ����ܺϲ�����Ҫ�޸�
	pxBlockToInsert��Nextָ�룬ָ��pxIterator����һ�����п�*/
	puc = ( uint8_t * ) pxBlockToInsert;
	if( ( puc + pxBlockToInsert->xBlockSize ) == ( uint8_t * ) pxIterator->pxNextFreeBlock )
	{
		if( pxIterator->pxNextFreeBlock != pxEnd[eMemType] )
		{
			/* Form one big block from the two blocks. */
			pxBlockToInsert->xBlockSize += pxIterator->pxNextFreeBlock->xBlockSize;
			pxBlockToInsert->pxNextFreeBlock = pxIterator->pxNextFreeBlock->pxNextFreeBlock;
		}
		else
		{
			pxBlockToInsert->pxNextFreeBlock = pxEnd[eMemType];
		}
	}
	else
	{
		pxBlockToInsert->pxNextFreeBlock = pxIterator->pxNextFreeBlock;
	}

	/* If the block being inserted plugged a gab, so was merged with the block
	before and the block after, then it's pxNextFreeBlock pointer will have
	already been set, and should not be set here as that would make it point
	to itself. */
	/*���Ҫ��pxBlockToInsertû�к�pxIterator�ϲ�����Ҫ�޸�pxIterator��Next
	ָ�룬���������������������*/
	if( pxIterator != pxBlockToInsert )
	{
		pxIterator->pxNextFreeBlock = pxBlockToInsert;
	}
	else
	{
		mtCOVERAGE_TEST_MARKER();
	}
}


