#include "fifo.h"
#include <stdio.h>
#include <stdlib.h>

 
 #define min(a,b) ((a) < (b) ? (a):(b))
 /*

ebox_fifo_init

 */

struct ebox_fifo *ebox_fifo_init(unsigned char *buffer,unsigned int size)
{
    struct ebox_fifo *fifo;

    fifo = malloc(sizeof(struct ebox_fifo));
    if (!fifo)
    return NULL;

    fifo->buffer = buffer;
    fifo->size = size;
    fifo->in = fifo->out = 0;

    return fifo;
}
 
 
 /*
 ebox_fifo_alloc
*/
 struct ebox_fifo *ebox_fifo_alloc(unsigned int size)
 {
     unsigned char *buffer;
     struct ebox_fifo *ret;
     /*
     * round up to the next power of 2, since our 'let the indices
     * wrap' tachnique works only in this case.
     */
     
     buffer = ebox_malloc(size);
     if (!buffer)
     return NULL;
     
     ret = ebox_fifo_init(buffer, size);
     
     if (ret ==NULL)
     ebox_free(buffer);
     
     return ret;
 }
 /*
 * ebox_fifo_free
 */
void ebox_fifo_free(struct ebox_fifo *fifo)
{
     ebox_free(fifo->buffer);
     ebox_free(fifo);
}
     /*
     ebox_fifo_put()
     */
 unsigned int ebox_fifo_put(struct ebox_fifo *fifo,unsigned char *buffer, unsigned int len)
{
    unsigned int l;

    len = min(len, fifo->size - fifo->in + fifo->out);/*可能是缓冲区的空闲长度或者要写长度*/

    /* first put the data starting from fifo->in to buffer end*/
    l = min(len, fifo->size - (fifo->in & (fifo->size -1)));
    ebox_memcpy(fifo->buffer + (fifo->in & (fifo->size -1)), buffer, l);

    /* then put the rest (if any) at the beginning of the buffer*/
    ebox_memcpy(fifo->buffer, buffer + l, len - l);

    fifo->in += len;

    return len;
}

 
 /*
 ebox_fifo_get
 */
unsigned int ebox_fifo_get(struct ebox_fifo *fifo,unsigned char *buffer, unsigned int len)
{
    unsigned int l;

    len = min(len, fifo->in - fifo->out); /*可读数据*/

    /* first get the data from fifo->out until the end of the buffer*/
    l = min(len, fifo->size - (fifo->out & (fifo->size -1)));
    ebox_memcpy(buffer, fifo->buffer + (fifo->out & (fifo->size -1)), l);

    /* then get the rest (if any) from the beginning of the buffer*/
    ebox_memcpy(buffer + l, fifo->buffer, len - l);

    fifo->out += len;

    return len;
}
