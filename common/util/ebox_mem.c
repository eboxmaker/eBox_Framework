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