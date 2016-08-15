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
#include "heap.h"

//内存池大小
size_t cfgTOTAL_HEAP_SIZE[] = {
#if (EN_SRAM_IN == 1)
    cfgTOTAL_HEAP_SIZE1,     //内部内存池大小
#endif
#if (EN_SRAM_EX == 1)
    cfgTOTAL_HEAP_SIZE2,     //外部内存池大小
#endif
#if (EN_SRAM_CCM == 1)
    cfgTOTAL_HEAP_SIZE3,     //CCM 内存池大小(此部分 SRAM 仅仅 CPU 可以访问!!!)
#endif
};

/* Block sizes must not get too small. */
#define heapMINIMUM_BLOCK_SIZE	( ( size_t ) ( memHeapStructSize << 1 ) )

/* Assumes 8bit bytes! */
#define heapBITS_PER_BYTE		( ( size_t ) 8 )

#if BYTE_ALIGNMENT == 32
	#define BYTE_ALIGNMENT_MASK ( 0x0000001f );
#endif
#if BYTE_ALIGNMENT == 16
	#define BYTE_ALIGNMENT_MASK ( 0x0000000f )
#endif
#if BYTE_ALIGNMENT == 8
	#define BYTE_ALIGNMENT_MASK ( 0x00000007 )
#endif

/* Allocate the memory for the heap. */
#if( cfgAPPLICATION_ALLOCATED_HEAP == 0 )
	/* The application writer has already defined the array used for the RTOS
	heap - probably so it can be placed in a special segment or address. */
	#if (EN_SRAM_IN == 1)
	    __align(8) static uint8_t memSRAMIn[ cfgTOTAL_HEAP_SIZE1 ];
	#endif
	#if (EN_SRAM_EX == 1)
        __align(8) static uint8_t memSRAMEx[ cfgTOTAL_HEAP_SIZE2 ]  __attribute__((at(0X68000000)));
    #endif
    #if (EN_SRAM_CCM == 1)
        __align(8) static uint8_t memSRAMCCM[ cfgTOTAL_HEAP_SIZE3 ] __attribute__((at(0X10000000)));
    #endif
#endif /* cfgAPPLICATION_ALLOCATED_HEAP */

uint8_t *memHeap[] = {
#if (EN_SRAM_IN == 1)
    memSRAMIn,     //内部内存池地址
#endif
#if (EN_SRAM_EX == 1)
    memSRAMEx,     //外部内存池地址
#endif
#if (EN_SRAM_CCM == 1)
    memSRAMCCM,   //CCM 内存池地址(此部分 SRAM 仅仅 CPU 可以访问!!!)
#endif
};
/*-----------------------------------------------------------*/

/* The size of the structure placed at the beginning of each allocated memory
block must by correctly byte aligned. */
//让xHeapStructSize的大小字节对齐
static const size_t memHeapStructSize	= ( sizeof( BlockLink_t ) + ( ( size_t ) ( BYTE_ALIGNMENT - 1 ) ) ) & ~( ( size_t ) BYTE_ALIGNMENT_MASK );

/* Create a couple of list links to mark the start and end of the list. */
static BlockLink_t memStart[COUT_OF_MEM];
static BlockLink_t *pxMemEnd[COUT_OF_MEM] = {
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
static size_t memFreeBytesRemaining[COUT_OF_MEM] = {
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
static size_t memMinimumEverFreeBytesRemaining[COUT_OF_MEM] = {
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

/* Gets set to the top bit of an size_t type.  When this bit in the blockSize
member of an BlockLink_t structure is set then the block belongs to the
application.  When the bit is free the block is still part of the free heap
space. */
static size_t memBlockAllocatedBit = 0;

/*-----------------------------------------------------------*/
/*
 * Inserts a block of memory that is being freed into the correct position in
 * the list of free memory blocks.  The block being freed will be merged with
 * the block in front it and/or the block behind it if the memory blocks are
 * adjacent to each other.
 */
static void memInsertBlockIntoFreeList( BlockLink_t *pxBlockToInsert ,uint8_t memType);

/*
 * Called automatically to setup the required heap structures the first time
 * pvPortMalloc() is called.
 */
static void memHeapInit( uint8_t memType );

void *memMalloc( size_t xWantedSize ,uint8_t memType )
{
    BlockLink_t *pxBlock, *pxPreviousBlock, *pxNewBlockLink;
    void *pvReturn = NULL;

    if(memType >= COUT_OF_MEM)
    {
        return NULL;
    }
    
	OS_ENTER_CRITICAL();
	{
		/* If this is the first call to malloc then the heap will require
		initialisation to setup the list of free blocks. */
		/*分配时还没有初始化，先初始化*/
		if( pxMemEnd[memType] == NULL )
		{
			memHeapInit(memType);
		}
		else
		{
			COVERAGE_TEST_MARKER();
		}

		/* Check the requested block size is not so large that the top bit is
		set.  The top bit of the block size member of the BlockLink_t structure
		is used to determine who owns the block - the application or the
		kernel, so it must be free. */
		/*用了BlockLink_t中xBlockSize的最高一位来标识某个内存块是否处于空闲状态,
		每一个分配出去的内存块大小就有限制了。例如，我用的是STM32F103，size_t是
		定义为unsigned int类型的，32位，可支持到4G的内存空间。但是最高1位用来指
		示空间状态的话，那就只有31位去标识内存块地址，即只支持到2G的内存空间*/
		if( ( xWantedSize & memBlockAllocatedBit ) == 0 )
		{
			/* The wanted size is increased so it can contain a BlockLink_t
			structure in addition to the requested amount of bytes. */
			if( xWantedSize > 0 )
			{
				xWantedSize += memHeapStructSize;                 //实际大小=请求大小+1个BlockLink_t的大小

				/* Ensure that blocks are always aligned to the required number
				of bytes. */
				/*保证分配的的大小字节对齐*/
				if( ( xWantedSize & BYTE_ALIGNMENT_MASK ) != 0x00 )
				{
					/* Byte alignment required. */
					xWantedSize += ( BYTE_ALIGNMENT - ( xWantedSize & BYTE_ALIGNMENT_MASK ) );
					cfgASSERT( ( xWantedSize & portBYTE_ALIGNMENT_MASK ) == 0 );
				}
				else
				{
					COVERAGE_TEST_MARKER();
				}
			}
			else
			{
				COVERAGE_TEST_MARKER();
			}

            /*分配大小小于实际可用，可以分配*/
			if( ( xWantedSize > 0 ) && ( xWantedSize <= memFreeBytesRemaining[memType] ) )
			{
				/* Traverse the list from the start	(lowest address) block until
				one	of adequate size is found. */
				/*寻找一块满足分配大小的内存块*/
				pxPreviousBlock = &(memStart[memType]);
				pxBlock = memStart[memType].nextFreeBlock;
				while( ( pxBlock->blockSize < xWantedSize ) && ( pxBlock->nextFreeBlock != NULL ) )
				{
					pxPreviousBlock = pxBlock;
					pxBlock = pxBlock->nextFreeBlock;
				}

				/* If the end marker was reached then a block of adequate size
				was	not found. */
				/*找到满足大小的内存块了*/
				if( pxBlock != pxMemEnd[memType] )
				{
					/* Return the memory space pointed to - jumping over the
					BlockLink_t structure at its start. */
					pvReturn = ( void * ) ( ( ( uint8_t * ) pxPreviousBlock->nextFreeBlock ) + memHeapStructSize );

					/* This block is being returned for use so must be taken out
					of the list of free blocks. */
					pxPreviousBlock->nextFreeBlock = pxBlock->nextFreeBlock;

					/* If the block is larger than required it can be split into
					two. */
					/*如果分配出去的空闲块比申请的空间大很多，则将该空闲块进行分割，把剩余的部分
					重新添加到链表中。*/
					if( ( pxBlock->blockSize - xWantedSize ) > heapMINIMUM_BLOCK_SIZE )
					{
						/* This block is to be split into two.  Create a new
						block following the number of bytes requested. The void
						cast is used to prevent byte alignment warnings from the
						compiler. */
						pxNewBlockLink = ( void * ) ( ( ( uint8_t * ) pxBlock ) + xWantedSize );
						cfgASSERT( ( ( ( size_t ) pxNewBlockLink ) & portBYTE_ALIGNMENT_MASK ) == 0 );

						/* Calculate the sizes of two blocks split from the
						single block. */
						pxNewBlockLink->blockSize = pxBlock->blockSize - xWantedSize;
						pxBlock->blockSize = xWantedSize;

						/* Insert the new block into the list of free blocks. */
						memInsertBlockIntoFreeList( ( pxNewBlockLink ),memType );
					}
					else
					{
						COVERAGE_TEST_MARKER();
					}

					memFreeBytesRemaining[memType] -= pxBlock->blockSize;

					if( memFreeBytesRemaining[memType] < memMinimumEverFreeBytesRemaining[memType] )
					{
						memMinimumEverFreeBytesRemaining[memType] = memFreeBytesRemaining[memType];
					}
					else
					{
						COVERAGE_TEST_MARKER();
					}

					/* The block is being returned - it is allocated and owned
					by the application and has no "next" block. */
					pxBlock->blockSize |= memBlockAllocatedBit;
					pxBlock->nextFreeBlock = NULL;
				}
				else
				{
					COVERAGE_TEST_MARKER();
				}
			}
			else
			{
				COVERAGE_TEST_MARKER();
			}
		}
		else
		{
			COVERAGE_TEST_MARKER();
		}

		//traceMALLOC( pvReturn, xWantedSize );
	}
	OS_EXIT_CRITICAL();

    /*内存分配失败时调用的钩子函数*/
	#if( cfgUSE_MALLOC_FAILED_HOOK == 1 )
	{
		if( pvReturn == NULL )
		{
			extern void vApplicationMallocFailedHook( void );
			vApplicationMallocFailedHook();
		}
		else
		{
			COVERAGE_TEST_MARKER();
		}
	}
	#endif

	cfgASSERT( ( ( ( uint32_t ) pvReturn ) & portBYTE_ALIGNMENT_MASK ) == 0 );
	return pvReturn;
}
/*-----------------------------------------------------------*/

void memFree( void *pv ,uint8_t memType)
{
    uint8_t *puc = ( uint8_t * ) pv;
    BlockLink_t *pxLink;

    if(memType >= COUT_OF_MEM)
    {
        return;
    }
    
	if( pv != NULL )
	{
		/* The memory being freed will have an BlockLink_t structure immediately
		before it. */
		puc -= memHeapStructSize;

		/* This casting is to keep the compiler from issuing warnings. */
		pxLink = ( void * ) puc;

		/* Check the block is actually allocated. */
		cfgASSERT( ( pxLink->blockSize & memBlockAllocatedBit ) != 0 );
		cfgASSERT( pxLink->nextFreeBlock == NULL );

		if( ( pxLink->blockSize & memBlockAllocatedBit ) != 0 )
		{
			if( pxLink->nextFreeBlock == NULL )
			{
				/* The block is being returned to the heap - it is no longer
				allocated. */
				pxLink->blockSize &= ~memBlockAllocatedBit;

				OS_ENTER_CRITICAL();
				{
					/* Add this block to the list of free blocks. */
					memFreeBytesRemaining[memType] += pxLink->blockSize;
					//traceFREE( pv, pxLink->blockSize );
					memInsertBlockIntoFreeList( ( ( BlockLink_t * ) pxLink ),memType );
				}
				OS_EXIT_CRITICAL();
			}
			else
			{
				COVERAGE_TEST_MARKER();
			}
		}
		else
		{
			COVERAGE_TEST_MARKER();
		}
	}
}
/*-----------------------------------------------------------*/

size_t memGetFreeHeapSize( uint8_t memType )
{
    if(memType >= COUT_OF_MEM)
    {
        return 0;
    }
    else
    {
	    return memFreeBytesRemaining[memType];
	}
}
/*-----------------------------------------------------------*/

size_t memGetMinimumEverFreeHeapSize( uint8_t memType )
{
    if(memType >= COUT_OF_MEM)
    {
        return 0;
    }
    {
	    return memMinimumEverFreeBytesRemaining[memType];
	}
}
/*-----------------------------------------------------------*/

void memPortInitialiseBlocks( void )
{
	/* This just exists to keep the linker quiet. */
}
/*-----------------------------------------------------------*/

/******************************************************************************
 * FUNCTION: memHeapInit ( )
 * DESCRIPTION: 内存池初始化
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
static void memHeapInit(uint8_t memType)
{
    BlockLink_t *pxFirstFreeBlock;
    uint8_t *pucAlignedHeap;
    size_t uxAddress;
    size_t xTotalHeapSize = cfgTOTAL_HEAP_SIZE[memType];

	/* Ensure the heap starts on a correctly aligned boundary. */
	/*保证内存池首地址8字节对齐*/
	uxAddress = ( size_t ) (memHeap[memType]);

	if( ( uxAddress & BYTE_ALIGNMENT_MASK ) != 0 )
	{
		uxAddress += ( BYTE_ALIGNMENT - 1 );
		uxAddress &= ~( ( size_t ) BYTE_ALIGNMENT_MASK );
		xTotalHeapSize -= uxAddress - ( size_t ) (memHeap[memType]);
	}

	pucAlignedHeap = ( uint8_t * ) uxAddress;

	/* xStart is used to hold a pointer to the first item in the list of free
	blocks.  The void cast is used to prevent compiler warnings. */
	memStart[memType].nextFreeBlock = ( void * ) pucAlignedHeap;             //指向第一个可用内存块
	memStart[memType].blockSize = ( size_t ) 0;

	/* pxEnd is used to mark the end of the list of free blocks and is inserted
	at the end of the heap space. */
	/*指向了内存堆的最后一个BlockLink_t大小的位置上。也就是说，内存堆最后的空间是存储着一个BlockLink_t，用来指示空闲块链表的终结*/
	uxAddress = ( ( size_t ) pucAlignedHeap ) + xTotalHeapSize;
	uxAddress -= memHeapStructSize;
	uxAddress &= ~( ( size_t ) BYTE_ALIGNMENT_MASK );
	pxMemEnd[memType] = ( void * ) uxAddress;
	pxMemEnd[memType]->blockSize = 0;
	pxMemEnd[memType]->nextFreeBlock = NULL;

	/* To start with there is a single free block that is sized to take up the
	entire heap space, minus the space taken by pxEnd. */
	/*初始只有一个可用内存块，大小为所有可用内存池大小*/
	pxFirstFreeBlock = ( void * ) pucAlignedHeap;
	pxFirstFreeBlock->blockSize = uxAddress - ( size_t ) pxFirstFreeBlock;
	pxFirstFreeBlock->nextFreeBlock = pxMemEnd[memType];

	/* Only one block exists - and it covers the entire usable heap space. */
	memMinimumEverFreeBytesRemaining[memType] = pxFirstFreeBlock->blockSize;
	memFreeBytesRemaining[memType] = pxFirstFreeBlock->blockSize;

	/* Work out the position of the top bit in a size_t variable. */
	/*用了BlockLink_t中xBlockSize的最高一位来标识某个内存块是否处于空闲状态*/
	memBlockAllocatedBit = ( ( size_t ) 1 ) << ( ( sizeof( size_t ) * heapBITS_PER_BYTE ) - 1 );
}
/*-----------------------------------------------------------*/

static void memInsertBlockIntoFreeList( BlockLink_t *pxBlockToInsert ,uint8_t memType)
{
    BlockLink_t *pxIterator;
    uint8_t *puc;

	/* Iterate through the list until a block is found that has a higher address
	than the block being inserted. */
	for( pxIterator = &(memStart[memType]); pxIterator->nextFreeBlock < pxBlockToInsert; pxIterator = pxIterator->nextFreeBlock )
	{
		/* Nothing to do here, just iterate to the right position. */
	}

	/* Do the block being inserted, and the block it is being inserted after
	make a contiguous block of memory? */
	/*先试着将pxIterator和pxBlockToInsert进行合并，可以合并的标准为pxIterator的
	首地址加上pxIterator的块大小之后等于pxBlockToInsert的首地址。相等就说明两个
	块是相邻的.如果不能合并，就什么事都不做。*/
	puc = ( uint8_t * ) pxIterator;
	if( ( puc + pxIterator->blockSize ) == ( uint8_t * ) pxBlockToInsert )
	{
		pxIterator->blockSize += pxBlockToInsert->blockSize;
		pxBlockToInsert = pxIterator;
	}
	else
	{
		COVERAGE_TEST_MARKER();
	}

	/* Do the block being inserted, and the block it is being inserted before
	make a contiguous block of memory? */
	/*再试着将pxBlockToInsert和pxIterator指向的下一个空闲块进行合并。可合并的标
	准和刚刚说的一样，只是这次用pxBlockToInsert的首地址加上pxBlockToInsert的块大
	小与pxIterator指向的下一个块地址比较。能合并是最好的，不能合并，则要修改
	pxBlockToInsert的Next指针，指向pxIterator的下一个空闲块*/
	puc = ( uint8_t * ) pxBlockToInsert;
	if( ( puc + pxBlockToInsert->blockSize ) == ( uint8_t * ) pxIterator->nextFreeBlock )
	{
		if( pxIterator->nextFreeBlock != pxMemEnd[memType] )
		{
			/* Form one big block from the two blocks. */
			pxBlockToInsert->blockSize += pxIterator->nextFreeBlock->blockSize;
			pxBlockToInsert->nextFreeBlock = pxIterator->nextFreeBlock->nextFreeBlock;
		}
		else
		{
			pxBlockToInsert->nextFreeBlock = pxMemEnd[memType];
		}
	}
	else
	{
		pxBlockToInsert->nextFreeBlock = pxIterator->nextFreeBlock;
	}

	/* If the block being inserted plugged a gab, so was merged with the block
	before and the block after, then it's nextFreeBlock pointer will have
	already been set, and should not be set here as that would make it point
	to itself. */
	/*最后，要是pxBlockToInsert没有和pxIterator合并，则还要修改pxIterator的Next
	指针，这样整条链表才完整无误*/
	if( pxIterator != pxBlockToInsert )
	{
		pxIterator->nextFreeBlock = pxBlockToInsert;
	}
	else
	{
		COVERAGE_TEST_MARKER();
	}
}


