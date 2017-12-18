#ifndef __RINGBUFX_H
#define __RINGBUFX_H

#ifdef __cplusplus
 extern "C" {
#endif
#include "ebox_type.h"

struct my_fifo {
 unsigned char *buffer;/* the buffer holding the data*/
 unsigned int size;/* the size of the allocated buffer*/
unsigned int in;/* data is added at offset (in % size)*/
 unsigned int out;/* data is extracted from off. (out % size)*/
};

 struct my_fifo *my_fifo_init(unsigned char *buffer,unsigned int size);
 struct my_fifo *my_fifo_alloc(unsigned int size);
 void my_fifo_free(struct my_fifo *fifo);
  unsigned int my_fifo_put(struct my_fifo *fifo, unsigned char *buffer, unsigned int len);
  unsigned int my_fifo_get(struct my_fifo *fifo, unsigned char *buffer, unsigned int len);
#ifdef __cplusplus
}
#endif

#endif