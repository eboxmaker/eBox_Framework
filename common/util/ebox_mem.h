#ifndef __EBOX_MEM_H
#define __EBOX_MEM_H
#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

    
#include "stdio.h"

void    *ebox_malloc(size_t sz);
void    ebox_free(void *ptr);
size_t  ebox_get_free();
int ebox_memcmp(const void * cs,const void * ct, size_t count);
void *ebox_memcpy(void * dst, const void *src, size_t count);    
#define ebox_free_1(pv)    memFree(pv,0)
    
#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif
