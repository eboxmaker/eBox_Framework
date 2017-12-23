#ifndef __MEM_TEST_H
#define __MEM_TEST_H
#ifdef __cplusplus
extern "C"{
#endif // __cplusplus
#include "stm32f10x.h"
#include <stdlib.h>

    
  void memHeapInit(void *begin_addr, void *end_addr);
void *_memMalloc( size_t xWantedSize );

#ifdef __cplusplus
}
#endif // __cplusplus
#endif

