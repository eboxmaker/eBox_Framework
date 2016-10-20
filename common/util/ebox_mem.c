#include "ebox_mem.h"
#include "heap.h"
void *ebox_malloc(size_t sz)
{
   return memMalloc(sz,SRAM_IN - 1);       
}
void ebox_free(void *ptr)
{
   memFree(ptr,SRAM_IN - 1);
   ptr = NULL;
}
size_t ebox_get_free()
{
    return memGetFreeHeapSize(SRAM_IN - 1);
}