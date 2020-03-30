
#include "ebox_core.h"
#include "mcu_config.h"
#include "ebox_mem.h"



static uint32_t heap_addr_begin, heap_addr_end;


#define BYTE_ALIGNMENT          (4)                 //字节对齐

#define MEM_ALIGN(size)         ((BYTE_ALIGNMENT + size - 1) & (~ (BYTE_ALIGNMENT - 1)))
#define MEM_ALIGN_DOWN(size)    ((size) & ~((BYTE_ALIGNMENT) - 1))
#define SIZEOF_STRUCT_MEM	    MEM_ALIGN(sizeof(eboxBlockLink_t))

/* Block sizes must not get too small. */
#define _MINIMUM_BLOCK_SIZE	( ( size_t ) ( SIZEOF_STRUCT_MEM << 1 ) )



static eboxBlockLink_t heap[3] = {0, 0, 0}; //记录了内存的起始位置
static eboxBlockLink_t *end_block[3] = {0, 0, 0}; //指向内存结尾最后的SIZEOF_STRUCT_MEM个字节
static size_t mem_size_aligned;//保存了内存总量有多大


static size_t MinimumEverFreeBytesRemaining ; //表示未分配内存堆空间历史最小值。只有记录未分配内存堆的最小值，才能知道最坏情况下内存堆的使用情况。
static size_t FreeBytesRemaining; //表示当前未分配的内存堆大小
static size_t BlockAllocatedBit = 0; //这个变量在第一次调用内存申请函数时被初始化,最高位被置1.根据CPU字长来设置。


static void insert_block_into_freeList( eboxBlockLink_t *pxBlockToInsert);

void ebox_heap_init(void *begin_addr, void *end_addr)
{
    eboxBlockLink_t *first_block;
    uint32_t begin_align = MEM_ALIGN((uint32_t)begin_addr);
    uint32_t end_align = MEM_ALIGN_DOWN((uint32_t)end_addr - SIZEOF_STRUCT_MEM);//预留最后一个结构体的空间。将会被初始化正链表的最后一个节点

    if ((end_align > (2 * SIZEOF_STRUCT_MEM)) &&
            ((end_align - 2 * SIZEOF_STRUCT_MEM) >= begin_align))
    {
        mem_size_aligned = end_align - begin_align - 2 * SIZEOF_STRUCT_MEM;//去掉起始和结束的两个结构体区域，剩余就是最大内存
    }
    else
    {
        return;
    }

    //heap[0]的next指针指向，内存起始位置
    heap[0].nextFreeBlock  = (eboxBlockLink_t *)begin_align;
    heap[0].blockSize = 0;//heap为链表入口。没有可用内存

    //heap_end指向内存终止位置SIZEOF_STRUCT_MEM个字节之前
    end_block[0] = (eboxBlockLink_t *)end_align;
    end_block[0]->blockSize = 0;//最后一个节点。后面没有内存。
    end_block[0]->nextFreeBlock  = NULL;//最后一个节点。后面没有内存。

    //起始位置初始化一个可用block；next指向heap_end
    first_block = (eboxBlockLink_t *)begin_align;
    first_block->nextFreeBlock = end_block[0];
    first_block->blockSize = mem_size_aligned;

    MinimumEverFreeBytesRemaining = first_block->blockSize;//更新历史最小分配内存。用来评估内存分配的碎片化程度，即使该被分配的内存被释放，此最小值也会保持不变
    FreeBytesRemaining = first_block->blockSize;//更新可用内存

    BlockAllocatedBit = ( ( size_t ) 1 ) << ( ( sizeof( size_t ) * 8 ) - 1 );//计算CPU位数，并将最高位设置为1.保存于BlockAllocatedBit

    heap_addr_begin = (uint32_t)begin_align;
    heap_addr_end = (uint32_t)end_align;

    //初始化完成后的结构为heap->first_block->end_block;
}


void *ebox_malloc( size_t xWantedSize )
{
    eboxBlockLink_t *pxBlock, *pxPreviousBlock, *pxNewBlockLink;
    void *pvReturn = NULL;

    uint32_t temp = __get_PRIMASK();//保存之前中断设置
    __disable_irq();

    if(end_block[0] == NULL)//如果在调用之前没有初始化内存，则初始化
    {
        ebox_heap_init((void *)MCU_HEAP_BEGIN, (void *)MCU_HEAP_END);
    }



    if( ( xWantedSize & BlockAllocatedBit ) == 0 )//检查内存单元是否为超过系统管理大小
    {

        if( xWantedSize > 0 )
        {
            xWantedSize += SIZEOF_STRUCT_MEM;                 //实际大小=请求大小+1个BlockLink_t的大小
            xWantedSize = MEM_ALIGN(xWantedSize);
        }
        if( ( xWantedSize > 0 ) && ( xWantedSize <= FreeBytesRemaining ) )//检查申请大小是否小于当前空闲总和
        {
            pxPreviousBlock = &heap[0];
            pxBlock = heap[0].nextFreeBlock;
            while( ( pxBlock->blockSize < xWantedSize ) && ( pxBlock->nextFreeBlock != NULL ) )//寻找第一个大于申请空间的空闲区域
            {
                pxPreviousBlock = pxBlock;
                pxBlock = pxBlock->nextFreeBlock;
            }
            if( (size_t)(pxBlock) != heap_addr_end )//如果所指向的内存区域不超过内存结尾，则有效
            {
                pvReturn = ( eboxBlockLink_t *) ( ( ( uint8_t * ) pxPreviousBlock->nextFreeBlock ) + SIZEOF_STRUCT_MEM ); //返回地址需要增加一个结构体空间
                pxPreviousBlock->nextFreeBlock = pxBlock->nextFreeBlock;//将其前一个空闲块的下一块指向其下一块空闲区域，跳过了当前被申请走的区块

            }
            if( ( pxBlock->blockSize - xWantedSize ) > _MINIMUM_BLOCK_SIZE )//如果当前区块还有剩余内存，则需要在剩余空间上创建新的空闲区块
            {
                pxNewBlockLink = ( eboxBlockLink_t * ) ( ( ( uint8_t * ) pxBlock ) + xWantedSize );//设置新空闲区的结构体存储的地址
                /* Calculate the sizes of two blocks split from the
                single block. */
                pxNewBlockLink->blockSize = pxBlock->blockSize - xWantedSize;//设置空闲区的大小
                pxBlock->blockSize = xWantedSize;//设置当前已申请内存的大小。
                insert_block_into_freeList(  pxNewBlockLink  );//将新的区块插入到空闲链表中

            }
            FreeBytesRemaining -= pxBlock->blockSize;//从空闲内存中减去相应的大小
            if( FreeBytesRemaining < MinimumEverFreeBytesRemaining )//更新历史最小分配内存。用来评估内存分配的碎片化程度，即使该被分配的内存被释放，此最小值也会保持不变
            {
                MinimumEverFreeBytesRemaining = FreeBytesRemaining;
            }

            /* The block is being returned - it is allocated and owned
            by the application and has no "next" block. */
            pxBlock->blockSize |= BlockAllocatedBit;//设置最高位为1，使内存单元标记为有效的
            pxBlock->nextFreeBlock = NULL;//将已分配区块的指向设置为0

        }
        __set_PRIMASK(temp);

    }
    if(pvReturn == NULL)
    {

    }

    return pvReturn;
}


void ebox_free( void *pv )
{
    uint8_t *puc = ( uint8_t * ) pv;
    eboxBlockLink_t *pxLink;

    uint32_t temp = __get_PRIMASK();//保存之前中断设置
    __disable_irq();
    if( pv != NULL )
    {
        /* The memory being freed will have an BlockLink_t structure immediately
        before it. */
        puc -= SIZEOF_STRUCT_MEM;

        /* This casting is to keep the compiler from issuing warnings. */
        pxLink = ( eboxBlockLink_t * ) puc;

        /* Check the block is actually allocated. */

        if( ( pxLink->blockSize & BlockAllocatedBit ) != 0 )//如果最高位为1，说明是有效的内存单元
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
    __set_PRIMASK(temp);

}

void *ebox_realloc(void *ptr, size_t size)
{
    if(ptr == NULL)
    {
         ptr = ebox_malloc(size);//distribution new mem
        return ptr;
    }
    else
    {
        void *temp = ptr;//save ptr to temp
        if(size < ebox_get_sizeof_ptr(ptr))
            return ptr;
        ptr = ebox_malloc(size);//distribution new mem
        if(ptr != NULL)
        {
            ebox_memcpy(ptr, temp, ebox_get_sizeof_ptr(temp));
            ebox_free(temp);//free temp
        }
        else
            return NULL;
    }
    return ptr;
}

size_t ebox_get_sizeof_ptr(void *ptr)
{
    eboxBlockLink_t *temp = (eboxBlockLink_t *)((uint32_t)ptr - SIZEOF_STRUCT_MEM);//get ptr`s mem maneger block
    if((temp->blockSize & BlockAllocatedBit) != 0) //如果最高位为1，说明是有效的内存单元
        return ((temp->blockSize & (~BlockAllocatedBit)) - 8);
    else
        return 0;
}

size_t ebox_get_free(void)
{
    return FreeBytesRemaining;
}

void *malloc(size_t size)
{
    return ebox_malloc(size);
}

void free(void *ptr)
{
    ebox_free(ptr);
}

void *realloc(void *ptr, size_t size)
{
    return ebox_realloc(ptr, size);
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


//获取heap使用了多少字节
float ebox_mem_usage(void)
{
    return (100 - ebox_get_free() * 100.0 / (heap_addr_end - heap_addr_begin));
}


//获取heap使用率
float ebox_mem_used(void)
{
    return (MCU_HEAP_SIZE - ebox_get_free());
}


//获取heap起始地址
size_t ebox_get_heap_start_addr(void)
{
    return (size_t)MEM_ALIGN((uint32_t)heap_addr_begin);
}

//获取heap结束地址
size_t ebox_get_heap_end_addr(void)
{
    return (size_t)MEM_ALIGN((uint32_t)heap_addr_end);

}


/**
*@brief    打印内存分割情况
           free block 即是内存可用区域，block数量越多说明内存碎片化越是严重
           第一个block是链表头部的变量，地址为heap[x];可用内存为0;
           最后一个变量是*end_block[x],指向的地址为内存尾部减去一个管理单元占用量
           0X20004FF8 = MCU_HEAP_END - SIZEOF_STRUCT_MEM;
           中间的是可用内存块，由于多次分配后，一部分被释放，一部分未被释放就
           会导致出现多个不连续的内存区域，也就是内存碎片化;如果碎片化太严重，
           可能会导致系统可用内存虽然很大，但是无法分配一个连续的大区域的内存。
           最理想的状态是可用内存块为1。
*@param    NONE
*@retval   可用内存块数量
*/
uint16_t ebox_free_block_print(void)
{

#if (EBOX_DEBUG && USE_PRINTF)
    eboxBlockLink_t *p;
    int i = 0;
    ebox_printf("\r\n------------------start------------------\r\n");
    ebox_printf(" free block\taddres\t|size\t|\r\n");
    for(p = (eboxBlockLink_t *)( &(heap[0]) ); p != NULL; p = ( p->nextFreeBlock))
    {
        ebox_printf("free block %d: ", i++);
        ebox_printf("|@0X%X\t|%06d\t|\r\n", p, p->blockSize);
    }
    ebox_printf("-------------------end-------------------\r\n");
    return i - 2;
#else
    return 0;
#endif
}


/**
 * memcmp - Compare two areas of memory
 * @param cs: One area of memory
 * @param ct: Another area of memory
 * @param count: The size of the area.
 */
int ebox_memcmp(const void *cs, const void *ct, size_t count)
{
    const unsigned char *su1, *su2;
    int res = 0;

    for( su1 = (unsigned char *)cs, su2 = (unsigned char *)ct; 0 < count; ++su1, ++su2, count--)
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
void *ebox_memcpy(void *dst, const void *src, size_t count)
{

    char *tmp = (char *) dst, *s = (char *) src;

    while (count--)
        *tmp++ = *s++;

    return dst;
}


