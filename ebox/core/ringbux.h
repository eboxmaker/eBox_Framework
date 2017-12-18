#ifndef __RINGBUFX_H
#define __RINGBUFX_H
#include "ebox_type.h"

struct my_fifo {
 unsigned char *buffer;/* the buffer holding the data*/
 unsigned int size;/* the size of the allocated buffer*/
unsigned int in;/* data is added at offset (in % size)*/
 unsigned int out;/* data is extracted from off. (out % size)*/
};


#endif