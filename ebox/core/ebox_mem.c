
#include "ebox_core.h"
#include "mcu_config.h"
#include "ebox_mem.h"



static uint32_t heap_addr_begin,heap_addr_end;


#define BYTE_ALIGNMENT          (4)                 //�ֽڶ���

#define MEM_ALIGN(size)         ((BYTE_ALIGNMENT + size - 1) & (~ (BYTE_ALIGNMENT - 1)))
#define MEM_ALIGN_DOWN(size)    ((size) & ~((BYTE_ALIGNMENT) - 1))
#define SIZEOF_STRUCT_MEM	    MEM_ALIGN(sizeof(eboxBlockLink_t))

/* Block sizes must not get too small. */
#define _MINIMUM_BLOCK_SIZE	( ( size_t ) ( SIZEOF_STRUCT_MEM << 1 ) )



static eboxBlockLink_t heap[3]={0,0,0};//��¼���ڴ����ʼλ��
static eboxBlockLink_t *end_block[3]={0,0,0};//ָ���ڴ��β����SIZEOF_STRUCT_MEM���ֽ�
static size_t mem_size_aligned;//�������ڴ������ж��


static size_t MinimumEverFreeBytesRemaining ; //��ʾδ�����ڴ�ѿռ���ʷ��Сֵ��ֻ�м�¼δ�����ڴ�ѵ���Сֵ������֪���������ڴ�ѵ�ʹ�������
static size_t FreeBytesRemaining; //��ʾ��ǰδ������ڴ�Ѵ�С
static size_t BlockAllocatedBit = 0; //��������ڵ�һ�ε����ڴ����뺯��ʱ����ʼ��,���λ����1.����CPU�ֳ������á�


static void insert_block_into_freeList( eboxBlockLink_t *pxBlockToInsert);

void ebox_heap_init(void *begin_addr, void *end_addr)
{
    eboxBlockLink_t *first_block;
    uint32_t begin_align = MEM_ALIGN((uint32_t)begin_addr);
    uint32_t end_align = MEM_ALIGN_DOWN((uint32_t)end_addr - SIZEOF_STRUCT_MEM);//Ԥ�����һ���ṹ��Ŀռ䡣���ᱻ��ʼ������������һ���ڵ�

    if ((end_align > (2 * SIZEOF_STRUCT_MEM)) &&
        ((end_align - 2 * SIZEOF_STRUCT_MEM) >= begin_align))
    {
        mem_size_aligned = end_align - begin_align - 2 * SIZEOF_STRUCT_MEM;//ȥ����ʼ�ͽ����������ṹ������ʣ���������ڴ�
    }
    else
    {
        return;
    }
    
    //heap[0]��nextָ��ָ���ڴ���ʼλ��
    heap[0].nextFreeBlock  = (eboxBlockLink_t *)begin_align;
    heap[0].blockSize = 0;//heapΪ������ڡ�û�п����ڴ�
    
    //heap_endָ���ڴ���ֹλ��SIZEOF_STRUCT_MEM���ֽ�֮ǰ
    end_block[0] = (eboxBlockLink_t *)end_align;
    end_block[0]->blockSize = 0;//���һ���ڵ㡣����û���ڴ档
    end_block[0]->nextFreeBlock  = NULL;//���һ���ڵ㡣����û���ڴ档
    
    //��ʼλ�ó�ʼ��һ������block��nextָ��heap_end
    first_block = (eboxBlockLink_t *)begin_align;
    first_block->nextFreeBlock = end_block[0];
    first_block->blockSize = mem_size_aligned;
    
    MinimumEverFreeBytesRemaining = first_block->blockSize;//������ʷ��С�����ڴ档���������ڴ�������Ƭ���̶ȣ���ʹ�ñ�������ڴ汻�ͷţ�����СֵҲ�ᱣ�ֲ���
	FreeBytesRemaining = first_block->blockSize;//���¿����ڴ�

	BlockAllocatedBit = ( ( size_t ) 1 ) << ( ( sizeof( size_t ) * 8 ) - 1 );//����CPUλ�����������λ����Ϊ1.������BlockAllocatedBit

    heap_addr_begin = (uint32_t)begin_align;
    heap_addr_end = (uint32_t)end_align;
    
    //��ʼ����ɺ�ĽṹΪheap->first_block->end_block;
}


void *ebox_malloc( size_t xWantedSize )
{
    eboxBlockLink_t *pxBlock, *pxPreviousBlock, *pxNewBlockLink;
    void *pvReturn = NULL;

    uint32_t temp = __get_PRIMASK();//����֮ǰ�ж�����
    __disable_irq();
    
    if(end_block[0] == NULL)//����ڵ���֮ǰû�г�ʼ���ڴ棬���ʼ��
    {
        ebox_heap_init((void *)MCU_HEAP_BEGIN,(void *)MCU_HEAP_END);
    }
        
        

    if( ( xWantedSize & BlockAllocatedBit ) == 0 )//����ڴ浥Ԫ�Ƿ�Ϊ����ϵͳ�����С
    {
        
        if( xWantedSize > 0 )
        {
            xWantedSize += SIZEOF_STRUCT_MEM;                 //ʵ�ʴ�С=�����С+1��BlockLink_t�Ĵ�С
            xWantedSize = MEM_ALIGN(xWantedSize);
        }
        if( ( xWantedSize > 0 ) && ( xWantedSize <= FreeBytesRemaining ) )//��������С�Ƿ�С�ڵ�ǰ�����ܺ�
        {
            pxPreviousBlock = &heap[0];
            pxBlock = heap[0].nextFreeBlock;
            while( ( pxBlock->blockSize < xWantedSize ) && ( pxBlock->nextFreeBlock != NULL ) )//Ѱ�ҵ�һ����������ռ�Ŀ�������
            {
                pxPreviousBlock = pxBlock;
                pxBlock = pxBlock->nextFreeBlock;
            }
            if( (size_t)(pxBlock) != heap_addr_end )//�����ָ����ڴ����򲻳����ڴ��β������Ч
            {
                pvReturn = ( void * ) ( ( ( uint8_t * ) pxPreviousBlock->nextFreeBlock ) + SIZEOF_STRUCT_MEM );//���ص�ַ��Ҫ����һ���ṹ��ռ�
                pxPreviousBlock->nextFreeBlock = pxBlock->nextFreeBlock;//����ǰһ�����п����һ��ָ������һ��������������˵�ǰ�������ߵ�����

            }
            if( ( pxBlock->blockSize - xWantedSize ) > _MINIMUM_BLOCK_SIZE )//�����ǰ���黹��ʣ���ڴ棬����Ҫ��ʣ��ռ��ϴ����µĿ�������
            {
                pxNewBlockLink = ( void * ) ( ( ( uint8_t * ) pxBlock ) + xWantedSize );//�����¿������Ľṹ��洢�ĵ�ַ
                /* Calculate the sizes of two blocks split from the
                single block. */
                pxNewBlockLink->blockSize = pxBlock->blockSize - xWantedSize;//���ÿ������Ĵ�С
                pxBlock->blockSize = xWantedSize;//���õ�ǰ�������ڴ�Ĵ�С��
                insert_block_into_freeList(  pxNewBlockLink  );//���µ�������뵽����������

            }
            FreeBytesRemaining -= pxBlock->blockSize;//�ӿ����ڴ��м�ȥ��Ӧ�Ĵ�С
            if( FreeBytesRemaining < MinimumEverFreeBytesRemaining )//������ʷ��С�����ڴ档���������ڴ�������Ƭ���̶ȣ���ʹ�ñ�������ڴ汻�ͷţ�����СֵҲ�ᱣ�ֲ���
            {
                MinimumEverFreeBytesRemaining = FreeBytesRemaining;
            }

            /* The block is being returned - it is allocated and owned
            by the application and has no "next" block. */
            pxBlock->blockSize |= BlockAllocatedBit;//�������λΪ1��ʹ�ڴ浥Ԫ���Ϊ��Ч��
            pxBlock->nextFreeBlock = NULL;//���ѷ��������ָ������Ϊ0

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

    
	if( pv != NULL )
	{
		/* The memory being freed will have an BlockLink_t structure immediately
		before it. */
		puc -= SIZEOF_STRUCT_MEM;

		/* This casting is to keep the compiler from issuing warnings. */
		pxLink = ( void * ) puc;

		/* Check the block is actually allocated. */

		if( ( pxLink->blockSize & BlockAllocatedBit ) != 0 )//������λΪ1��˵������Ч���ڴ浥Ԫ
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

void *ebox_realloc(void *ptr, size_t size)
{
    if(ptr == NULL)
    {
        return ebox_malloc(size);//distribution new mem
    }
    else
    {
        void *temp = ptr;//save ptr to temp
        if(size < ebox_get_sizeof_ptr(ptr))
            return ptr;
        ptr = ebox_malloc(size);//distribution new mem
        if(ptr != NULL)
        {
            ebox_memcpy(ptr,temp,ebox_get_sizeof_ptr(temp));
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
    if((temp->blockSize & BlockAllocatedBit) != 0) //������λΪ1��˵������Ч���ڴ浥Ԫ
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
    return ebox_realloc(ptr,size);
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
	/*�����Ž�pxIterator��pxBlockToInsert���кϲ������Ժϲ��ı�׼ΪpxIterator��
	�׵�ַ����pxIterator�Ŀ��С֮�����pxBlockToInsert���׵�ַ����Ⱦ�˵������
	�������ڵ�.������ܺϲ�����ʲô�¶�������*/
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
	/*�����Ž�pxBlockToInsert��pxIteratorָ�����һ�����п���кϲ����ɺϲ��ı�
	׼�͸ո�˵��һ����ֻ�������pxBlockToInsert���׵�ַ����pxBlockToInsert�Ŀ��
	С��pxIteratorָ�����һ�����ַ�Ƚϡ��ܺϲ�����õģ����ܺϲ�����Ҫ�޸�
	pxBlockToInsert��Nextָ�룬ָ��pxIterator����һ�����п�*/
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
	/*���Ҫ��pxBlockToInsertû�к�pxIterator�ϲ�����Ҫ�޸�pxIterator��Next
	ָ�룬���������������������*/
	if( pxIterator != pxBlockToInsert )
	{
		pxIterator->nextFreeBlock = pxBlockToInsert;
	}
	else
	{
	}
}


//��ȡheapʹ���˶����ֽ�
float ebox_mem_usage(void)
{
    return (100 - ebox_get_free() * 100.0 / (heap_addr_end - heap_addr_begin));
}


//��ȡheapʹ����
float ebox_mem_used(void)
{
    return (MCU_HEAP_SIZE - ebox_get_free());
}


//��ȡheap��ʼ��ַ
size_t ebox_get_heap_start_addr(void)
{
    return (size_t)MEM_ALIGN((uint32_t)heap_addr_begin);
}

//��ȡheap������ַ
size_t ebox_get_heap_end_addr(void)
{
    return (size_t)MEM_ALIGN((uint32_t)heap_addr_end);

}


 /**
 *@brief    ��ӡ�ڴ�ָ����
            free block �����ڴ��������block����Խ��˵���ڴ���Ƭ��Խ������
            ��һ��block������ͷ���ı�������ַΪheap[x];�����ڴ�Ϊ0;
            ���һ��������*end_block[x],ָ��ĵ�ַΪ�ڴ�β����ȥһ������Ԫռ����
            0X20004FF8 = MCU_HEAP_END - SIZEOF_STRUCT_MEM;
            �м���ǿ����ڴ�飬���ڶ�η����һ���ֱ��ͷţ�һ����δ���ͷž�
            �ᵼ�³��ֶ�����������ڴ�����Ҳ�����ڴ���Ƭ��;�����Ƭ��̫���أ�
            ���ܻᵼ��ϵͳ�����ڴ���Ȼ�ܴ󣬵����޷�����һ�������Ĵ�������ڴ档
            �������״̬�ǿ����ڴ��Ϊ1��
 *@param    NONE
 *@retval   �����ڴ������
*/
uint16_t ebox_free_block_print(void)
{

    #if EBOX_DEBUG    
    #if USE_PRINTF
    eboxBlockLink_t *p;
    int i = 0;
    ebox_printf("\r\n------------------start------------------\r\n");
    ebox_printf(" free block\taddres\t|size\t|\r\n");
    for(p = (eboxBlockLink_t *)( &(heap[0]) ); p != NULL; p = ( p->nextFreeBlock))
    {
        ebox_printf("free block %d: ",i++);
        ebox_printf("|@0X%X\t|%06d\t|\r\n",p,p->blockSize);
    }
    ebox_printf("-------------------end-------------------\r\n");
    return i-2;
    #endif
    #endif
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


