#include "mem_test.h"


typedef struct _BLOCK_LINK
{
	struct _BLOCK_LINK *nextFreeBlock;	/*<< The next free block in the list. */
	size_t blockSize;						/*<< The size of the free block. */
} _BlockLink_t;



static size_t memMinimumEverFreeBytesRemaining ;
static size_t memFreeBytesRemaining;
static size_t memBlockAllocatedBit = 0;


#define BYTE_ALIGNMENT          (8)                 //�ֽڶ���



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

#define ALIGN(size, align)                  ((align + size - 1) & (~ (align - 1)))
#define MEM_ALIGN(size)                     ((BYTE_ALIGNMENT + size - 1) & (~ (BYTE_ALIGNMENT - 1)))
#define MEM_ALIGN_DOWN(size)                ((size) & ~((BYTE_ALIGNMENT) - 1))

static const size_t memHeapStructSize	= ( sizeof( _BlockLink_t ) + ( ( size_t ) ( BYTE_ALIGNMENT - 1 ) ) ) & ~( ( size_t ) BYTE_ALIGNMENT_MASK );


extern int Image$$RW_IRAM1$$ZI$$Limit;
void rt_system_heap_init(void *begin_addr, void *end_addr)
{
}



 //   rt_system_heap_init((void*)&Image$$RW_IRAM1$$ZI$$Limit, (void*)123456);



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

static size_t mem_size_aligned;



/******************************************************************************
 * FUNCTION: memHeapInit ( )
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
void memHeapInit(void *begin_addr, void *end_addr)
{
    static _BlockLink_t memStart;
    static _BlockLink_t *pxMemEnd;
    _BlockLink_t *pxFirstFreeBlock;
    uint8_t *pucAlignedHeap;
    size_t uxAddress;
    size_t xTotalHeapSize = MEM_ALIGN(((uint32_t)end_addr - (uint32_t)begin_addr));

	/* Ensure the heap starts on a correctly aligned boundary. */
	/*��֤�ڴ���׵�ַ8�ֽڶ���*/
	uxAddress = ( size_t ) (begin_addr);

	if( ( uxAddress & BYTE_ALIGNMENT_MASK ) != 0 )
	{
		uxAddress += ( BYTE_ALIGNMENT - 1 );
		uxAddress &= ~( ( size_t ) BYTE_ALIGNMENT_MASK );
		xTotalHeapSize -= uxAddress - ( size_t ) (begin_addr);
	}
    mem_size_aligned = xTotalHeapSize - 2 * memHeapStructSize;

	pucAlignedHeap = ( uint8_t * ) uxAddress;

	/* xStart is used to hold a pointer to the first item in the list of free
	blocks.  The void cast is used to prevent compiler warnings. */
	memStart.nextFreeBlock = ( void * ) pucAlignedHeap;             //ָ���һ�������ڴ��
	memStart.blockSize = ( size_t ) 0;

	/* pxEnd is used to mark the end of the list of free blocks and is inserted
	at the end of the heap space. */
	/*ָ�����ڴ�ѵ����һ��BlockLink_t��С��λ���ϡ�Ҳ����˵���ڴ�����Ŀռ��Ǵ洢��һ��BlockLink_t������ָʾ���п�������ս�*/
	uxAddress = ( ( size_t ) pucAlignedHeap ) + xTotalHeapSize;
	uxAddress -= memHeapStructSize;
	uxAddress &= ~( ( size_t ) BYTE_ALIGNMENT_MASK );
    
    
	pxMemEnd = (void *) uxAddress;
	pxMemEnd->blockSize = 0;
	pxMemEnd->nextFreeBlock = NULL;

	/* To start with there is a single free block that is sized to take up the
	entire heap space, minus the space taken by pxEnd. */
	/*��ʼֻ��һ�������ڴ�飬��СΪ���п����ڴ�ش�С*/
	pxFirstFreeBlock = ( void * ) pucAlignedHeap;
	pxFirstFreeBlock->blockSize = uxAddress - ( size_t ) pxFirstFreeBlock;
	pxFirstFreeBlock->nextFreeBlock = pxMemEnd;

	/* Only one block exists - and it covers the entire usable heap space. */
	memMinimumEverFreeBytesRemaining = pxFirstFreeBlock->blockSize;
	memFreeBytesRemaining = pxFirstFreeBlock->blockSize;

	/* Work out the position of the top bit in a size_t variable. */
	/*����BlockLink_t��xBlockSize�����һλ����ʶĳ���ڴ���Ƿ��ڿ���״̬*/
	memBlockAllocatedBit = ( ( size_t ) 1 ) << ( ( sizeof( size_t ) * heapBITS_PER_BYTE ) - 1 );
}


void *_memMalloc( size_t xWantedSize )
{
    static _BlockLink_t *memStart;

    _BlockLink_t *pxBlock, *pxPreviousBlock, *pxNewBlockLink;
    void *pvReturn = NULL;


    {
		/* If this is the first call to malloc then the heap will require
		initialisation to setup the list of free blocks. */
		/*����ʱ��û�г�ʼ�����ȳ�ʼ��*/

//        memHeapInit();


		/* Check the requested block size is not so large that the top bit is
		set.  The top bit of the block size member of the BlockLink_t structure
		is used to determine who owns the block - the application or the
		kernel, so it must be free. */
		/*����BlockLink_t��xBlockSize�����һλ����ʶĳ���ڴ���Ƿ��ڿ���״̬,
		ÿһ�������ȥ���ڴ���С���������ˡ����磬���õ���STM32F103��size_t��
		����Ϊunsigned int���͵ģ�32λ����֧�ֵ�4G���ڴ�ռ䡣�������1λ����ָ
		ʾ�ռ�״̬�Ļ����Ǿ�ֻ��31λȥ��ʶ�ڴ���ַ����ֻ֧�ֵ�2G���ڴ�ռ�*/
		if( ( xWantedSize & memBlockAllocatedBit ) == 0 )
		{
			/* The wanted size is increased so it can contain a BlockLink_t
			structure in addition to the requested amount of bytes. */
			if( xWantedSize > 0 )
			{
				xWantedSize += memHeapStructSize;                 //ʵ�ʴ�С=�����С+1��BlockLink_t�Ĵ�С

				/* Ensure that blocks are always aligned to the required number
				of bytes. */
				/*��֤����ĵĴ�С�ֽڶ���*/
				if( ( xWantedSize & BYTE_ALIGNMENT_MASK ) != 0x00 )
				{
					/* Byte alignment required. */
					xWantedSize += ( BYTE_ALIGNMENT - ( xWantedSize & BYTE_ALIGNMENT_MASK ) );
				}
				else
				{
				}
			}
			else
			{
			}

            /*�����СС��ʵ�ʿ��ã����Է���*/
			if( ( xWantedSize > 0 ) && ( xWantedSize <= memFreeBytesRemaining ) )
			{
				/* Traverse the list from the start	(lowest address) block until
				one	of adequate size is found. */
				/*Ѱ��һ����������С���ڴ��*/
				pxPreviousBlock = (memStart);
				pxBlock = memStart->nextFreeBlock;
				while( ( pxBlock->blockSize < xWantedSize ) && ( pxBlock->nextFreeBlock != NULL ) )
				{
					pxPreviousBlock = pxBlock;
					pxBlock = pxBlock->nextFreeBlock;
				}

				/* If the end marker was reached then a block of adequate size
				was	not found. */
				/*�ҵ������С���ڴ����*/
				if( (size_t)(&pxBlock) != mem_size_aligned )
				{
					/* Return the memory space pointed to - jumping over the
					BlockLink_t structure at its start. */
					pvReturn = ( void * ) ( ( ( uint8_t * ) pxPreviousBlock->nextFreeBlock ) + memHeapStructSize );

					/* This block is being returned for use so must be taken out
					of the list of free blocks. */
					pxPreviousBlock->nextFreeBlock = pxBlock->nextFreeBlock;

					/* If the block is larger than required it can be split into
					two. */
					/*��������ȥ�Ŀ��п������Ŀռ��ܶ࣬�򽫸ÿ��п���зָ��ʣ��Ĳ���
					������ӵ������С�*/
					if( ( pxBlock->blockSize - xWantedSize ) > heapMINIMUM_BLOCK_SIZE )
					{
						/* This block is to be split into two.  Create a new
						block following the number of bytes requested. The void
						cast is used to prevent byte alignment warnings from the
						compiler. */
						pxNewBlockLink = ( void * ) ( ( ( uint8_t * ) pxBlock ) + xWantedSize );

						/* Calculate the sizes of two blocks split from the
						single block. */
						pxNewBlockLink->blockSize = pxBlock->blockSize - xWantedSize;
						pxBlock->blockSize = xWantedSize;

						/* Insert the new block into the list of free blocks. */
						//memInsertBlockIntoFreeList( ( pxNewBlockLink ),memType );
					}
					else
					{
					}

					memFreeBytesRemaining -= pxBlock->blockSize;

					if( memFreeBytesRemaining < memMinimumEverFreeBytesRemaining )
					{
						memMinimumEverFreeBytesRemaining = memFreeBytesRemaining;
					}
					else
					{
					}

					/* The block is being returned - it is allocated and owned
					by the application and has no "next" block. */
					pxBlock->blockSize |= memBlockAllocatedBit;
					pxBlock->nextFreeBlock = NULL;
				}
				else
				{
				}
			}
			else
			{
			}
		}
		else
		{
		}

		//traceMALLOC( pvReturn, xWantedSize );
	}

    /*�ڴ����ʧ��ʱ���õĹ��Ӻ���*/
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
	return pvReturn;
}
