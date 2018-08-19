#ifndef __EBOX_MEM_H
#define __EBOX_MEM_H
#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

#include "ebox_type.h"
    
typedef struct EBOX_BLOCK_LINK
{
	struct EBOX_BLOCK_LINK *nextFreeBlock;	/*<< The next free block in the list. */
	size_t blockSize;						/*<< The size of the free block. */
} eboxBlockLink_t;


//ebox Dynamic memory manager
void    ebox_heap_init(void *begin_addr, void *end_addr);
void    *ebox_malloc( size_t xWantedSize );
void    ebox_free( void *ptr );
void    *ebox_realloc(void *ptr, size_t size);// repeat malloc 
size_t  ebox_get_sizeof_ptr(void *ptr);// get ptr mem size 
size_t  ebox_get_free(void);// get free mem size 


void    *malloc(size_t size);
void    free(void *ptr);
void    *realloc(void *ptr, size_t size);

float   ebox_mem_usage(void);
float   ebox_mem_used(void);
size_t  ebox_get_heap_start_addr(void);
size_t  ebox_get_heap_end_addr(void);
uint16_t ebox_free_block_print(void);
    
int ebox_memcmp(const void * cs,const void * ct, size_t count);
void *ebox_memcpy(void * dst, const void *src, size_t count);    
    
#ifdef __cplusplus
} 
#endif // __cplusplus

#endif
