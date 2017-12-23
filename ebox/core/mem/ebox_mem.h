#ifndef __EBOX_MEM_H
#define __EBOX_MEM_H
#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

#include "ebox_type.h"
#include "mcu_config.h"
#include "stdio.h"

void    ebox_heap_init(void *begin_addr, void *end_addr);
void    *ebox_malloc( size_t xWantedSize );
void    ebox_free( void *ptr );
size_t  ebox_get_free();
size_t  ebox_get_sram_start_addr();
size_t  ebox_get_sram_end_addr();

    
int ebox_memcmp(const void * cs,const void * ct, size_t count);
void *ebox_memcpy(void * dst, const void *src, size_t count);    
    
#ifdef __cplusplus
} 
#endif // __cplusplus

#endif
