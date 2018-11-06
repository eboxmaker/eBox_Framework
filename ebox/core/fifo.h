#ifndef __RINGBUFX_H
#define __RINGBUFX_H
#include "ebox_type.h"

#ifdef __cplusplus
extern "C" {
#endif


struct ebox_fifo
{
    unsigned char *buffer;/* the buffer holding the data*/
    unsigned int size;/* the size of the allocated buffer*/
    unsigned int in;/* data is added at offset (in % size)*/
    unsigned int out;/* data is extracted from off. (out % size)*/
};

struct ebox_fifo  *ebox_fifo_init(unsigned char *buffer, unsigned int size);
struct ebox_fifo  *ebox_fifo_alloc(unsigned int size);
void            ebox_fifo_free(struct ebox_fifo *fifo);
unsigned int    ebox_fifo_put(struct ebox_fifo *fifo, unsigned char *buffer, unsigned int len);
unsigned int    ebox_fifo_get(struct ebox_fifo *fifo, unsigned char *buffer, unsigned int len);



#ifdef __cplusplus
}
#endif

#endif

