#include "ebox_mem.h"
#include "ebox_printf.h"
#include "mcu_config.h"

static uint32_t ram_addr_begin,ram_addr_end;


#define BYTE_ALIGNMENT          (8)                 //字节对齐

#define MEM_ALIGN(size)         ((BYTE_ALIGNMENT + size - 1) & (~ (BYTE_ALIGNMENT - 1)))
#define MEM_ALIGN_DOWN(size)    ((size) & ~((BYTE_ALIGNMENT) - 1))
#define SIZEOF_STRUCT_MEM	    MEM_ALIGN(sizeof(eboxBlockLink_t))

/* Block sizes must not get too small. */
#define _MINIMUM_BLOCK_SIZE	( ( size_t ) ( SIZEOF_STRUCT_MEM << 1 ) )



static eboxBlockLink_t heap[3]={0,0,0};//记录了内存的起始位置
static eboxBlockLink_t *end_block[3]={0,0,0};//指向内存结尾最后的SIZEOF_STRUCT_MEM个字节
static size_t mem_size_aligned;//保存了内存总量有多大


static size_t MinimumEverFreeBytesRemaining ; //表示未分配内存堆空间历史最小值。只有记录未分配内存堆的最小值，才能知道最坏情况下内存堆的使用情况。
static size_t FreeBytesRemaining; //表示当前未分配的内存堆大小
static size_t BlockAllocatedBit = 0; //这个变量在第一次调用内存申请函数时被初始化,最高位被置1.根据CPU字长来设置。


static void insert_block_into_freeList( eboxBlockLink_t *pxBlockToInsert);

void ebox_heap_init(void *begin_addr, void *end_addr)
{
    eboxBlockLink_t *first_block;
    uint32_t begin_align = MEM_ALIGN((uint32_t)begin_addr);
    uint32_t end_align = MEM_ALIGN_DOWN((uint32_t)end_addr - 8);

    if ((end_align > (2 * SIZEOF_STRUCT_MEM)) &&
        ((end_align - 2 * SIZEOF_STRUCT_MEM) >= begin_align))
    {
        mem_size_aligned = end_align - begin_align - 2 * SIZEOF_STRUCT_MEM;
    }
    else
    {
        return;
    }
    
    //heap[0]的next指针指向，内存起始位置
    heap[0].nextFreeBlock  = (eboxBlockLink_t *)begin_align;
    heap[0].blockSize = 0;
    
    //heap_end指向内存终止位置SIZEOF_STRUCT_MEM个字节之前
    end_block[0] = (eboxBlockLink_t *)end_align;
    end_block[0]->blockSize = 0;
    end_block[0]->nextFreeBlock  = NULL;
    
    //起始位置初始化一个可用block；next指向heap_end
    first_block->nextFreeBlock = end_block[0];
    first_block->blockSize = mem_size_aligned;
    
    MinimumEverFreeBytesRemaining = first_block->blockSize;
	FreeBytesRemaining = first_block->blockSize;

	BlockAllocatedBit = ( ( size_t ) 1 ) << ( ( sizeof( size_t ) * 8 ) - 1 );

    ram_addr_begin = (uint32_t)begin_addr;
    ram_addr_end = (uint32_t)end_addr;
}
void *ebox_malloc( size_t xWantedSize )
{

    eboxBlockLink_t *pxBlock, *pxPreviousBlock, *pxNewBlockLink;
    void *pvReturn = NULL;

    __disable_irq();
    
    if(end_block[0] == NULL)
    {
        ebox_heap_init(MCU_SRAM_BEGIN,(void *)MCU_SRAM_END);
    }
        
        

    if( ( xWantedSize & BlockAllocatedBit ) == 0 )
    {
        
        if( xWantedSize > 0 )
        {
            xWantedSize += SIZEOF_STRUCT_MEM;                 //实际大小=请求大小+1个BlockLink_t的大小
            xWantedSize = MEM_ALIGN(xWantedSize);
        }
        if( ( xWantedSize > 0 ) && ( xWantedSize <= FreeBytesRemaining ) )
        {
            pxPreviousBlock = &heap[0];
            pxBlock = heap[0].nextFreeBlock;
            while( ( pxBlock->blockSize < xWantedSize ) && ( pxBlock->nextFreeBlock != NULL ) )
            {
                pxPreviousBlock = pxBlock;
                pxBlock = pxBlock->nextFreeBlock;
            }
            if( (size_t)(&pxBlock) != mem_size_aligned )
            {
                pvReturn = ( void * ) ( ( ( uint8_t * ) pxPreviousBlock->nextFreeBlock ) + SIZEOF_STRUCT_MEM );
                pxPreviousBlock->nextFreeBlock = pxBlock->nextFreeBlock;

            }
            if( ( pxBlock->blockSize - xWantedSize ) > _MINIMUM_BLOCK_SIZE )
            {
                pxNewBlockLink = ( void * ) ( ( ( uint8_t * ) pxBlock ) + xWantedSize );
                /* Calculate the sizes of two blocks split from the
                single block. */
                pxNewBlockLink->blockSize = pxBlock->blockSize - xWantedSize;
                pxBlock->blockSize = xWantedSize;
                insert_block_into_freeList(  pxNewBlockLink  );

            }
            FreeBytesRemaining -= pxBlock->blockSize;
            if( FreeBytesRemaining < MinimumEverFreeBytesRemaining )
            {
                MinimumEverFreeBytesRemaining = FreeBytesRemaining;
            }

            /* The block is being returned - it is allocated and owned
            by the application and has no "next" block. */
            pxBlock->blockSize |= BlockAllocatedBit;
            pxBlock->nextFreeBlock = NULL;

        }
    __enable_irq();
    }
    if(pvReturn == NULL)
    {
        ebox_printf("bad mem malloc!!!\r\n");
    }
    
	return pvReturn;
}
void ebox_free( void *pv )
{
    uint8_t *puc = ( uint8_t * ) pv;
    eboxBlockLink_t *pxLink;

    
	if( pv != NULL )
	{
		/* The memory being freed will have an BlockLink_t structure immediately
		before it. */
		puc -= SIZEOF_STRUCT_MEM;

		/* This casting is to keep the compiler from issuing warnings. */
		pxLink = ( void * ) puc;

		/* Check the block is actually allocated. */

		if( ( pxLink->blockSize & BlockAllocatedBit ) != 0 )
		{
			if( pxLink->nextFreeBlock == NULL )
			{
				/* The block is being returned to the heap - it is no longer
				allocated. */
				pxLink->blockSize &= ~BlockAllocatedBit;
				{
					/* Add this block to the list of free blocks. */
					FreeBytesRemaining += pxLink->blockSize;
					//traceFREE( pv, pxLink->blockSize );
					insert_block_into_freeList( ( ( eboxBlockLink_t * ) pxLink ) );
				}
			}
			else
			{
			}
		}
		else
		{
		}
	}

}
size_t ebox_get_free(void)
{
    return FreeBytesRemaining;
}


static void insert_block_into_freeList( eboxBlockLink_t *pxBlockToInsert)
{
    eboxBlockLink_t *pxIterator;
    uint8_t *puc;
    
  	/* Iterate through the list until a block is found that has a higher address
	than the block being inserted. */
	for( pxIterator = &(heap[0]); pxIterator->nextFreeBlock < pxBlockToInsert; pxIterator = pxIterator->nextFreeBlock )
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
		if( pxIterator->nextFreeBlock != end_block[0] )
		{
			/* Form one big block from the two blocks. */
			pxBlockToInsert->blockSize += pxIterator->nextFreeBlock->blockSize;
			pxBlockToInsert->nextFreeBlock = pxIterator->nextFreeBlock->nextFreeBlock;
		}
		else
		{
			pxBlockToInsert->nextFreeBlock = end_block[0];
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
	}

    
}
float ebox_mem_usage()
{
    return (100 - ebox_get_free() * 100.0 / (ram_addr_end - ram_addr_begin));
}
size_t ebox_get_sram_start_addr(void)
{
    return (size_t)MEM_ALIGN((uint32_t)ram_addr_begin);
}

size_t ebox_get_sram_end_addr(void)
{
    return (size_t)MEM_ALIGN((uint32_t)ram_addr_end);

}

uint16_t ebox_free_block_print(void)
{
    eboxBlockLink_t *p;
    int i = 0;
    ebox_printf("----start----\r\n");
    for(p = (eboxBlockLink_t *)( &(heap[0]) ); p != NULL; p = ( p->nextFreeBlock))
    {
        ebox_printf("free block %d: ",i++);
        ebox_printf("0X%X\t|%x\t|\r\n",p,p->blockSize);
    }
    ebox_printf("----end-----\r\n");
    return 0;
}


/**
 * memcmp - Compare two areas of memory
 * @param cs: One area of memory
 * @param ct: Another area of memory
 * @param count: The size of the area.
 */
int ebox_memcmp(const void * cs,const void * ct, size_t count)
{
	const unsigned char *su1, *su2;
	int res = 0;

	for( su1 = cs, su2 = ct; 0 < count; ++su1, ++su2, count--)
		if ((res = *su1 - *su2) != 0)
			break;
	return res;
}

/**
 * This function will copy memory content from source address to destination
 * address.
 *
 * @param dst the address of destination memory
 * @param src  the address of source memory
 * @param count the copied length
 *
 * @return the address of destination memory
 *
 */
void *ebox_memcpy(void * dst, const void *src, size_t count)
{

	char *tmp = (char *) dst, *s = (char *) src;

	while (count--)
		*tmp++ = *s++;
	
	return dst;
}
