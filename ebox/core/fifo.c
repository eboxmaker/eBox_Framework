#include "fifo.h"
#include <stdio.h>
#include <stdlib.h>

 
 #define min(a,b) ((a) < (b) ? (a):(b))
 /*

my_fifo_init

 */

 struct my_fifo *my_fifo_init(unsigned char *buffer,unsigned int size)
 {
 struct my_fifo *fifo;
 
 fifo = malloc(sizeof(struct my_fifo));
 if (!fifo)
 return NULL;

 fifo->buffer = buffer;
 fifo->size = size;
 fifo->in = fifo->out = 0;

 return fifo;
 }
 
 
 /*
 my_fifo_alloc
*/
 struct my_fifo *my_fifo_alloc(unsigned int size)
 {
 unsigned char *buffer;
 struct my_fifo *ret;
 /*
 * round up to the next power of 2, since our 'let the indices
 * wrap' tachnique works only in this case.
 */
 
 buffer = malloc(size);
 if (!buffer)
 return NULL;
 
 ret = my_fifo_init(buffer, size);
 
 if (ret ==NULL)
 free(buffer);
 
 return ret;
 }
 /*
 * my_fifo_free
 */
 void my_fifo_free(struct my_fifo *fifo)
 {
 free(fifo->buffer);
 free(fifo);
 }
 /*
 my_fifo_put()
 */
 unsigned int my_fifo_put(struct my_fifo *fifo,
 unsigned char *buffer, unsigned int len)
 {
 unsigned int l;
 
 len = min(len, fifo->size - fifo->in + fifo->out);/*可能是缓冲区的空闲长度或者要写长度*/
 
 /* first put the data starting from fifo->in to buffer end*/
 l = min(len, fifo->size - (fifo->in & (fifo->size -1)));
 memcpy(fifo->buffer + (fifo->in & (fifo->size -1)), buffer, l);
 
 /* then put the rest (if any) at the beginning of the buffer*/
 memcpy(fifo->buffer, buffer + l, len - l);
 
 fifo->in += len;
 
 return len;
 }

 
 /*
 my_fifo_get
 */
 unsigned int my_fifo_get(struct my_fifo *fifo,
 unsigned char *buffer, unsigned int len)
 {
 unsigned int l;
 
 len = min(len, fifo->in - fifo->out); /*可读数据*/
 
 /* first get the data from fifo->out until the end of the buffer*/
 l = min(len, fifo->size - (fifo->out & (fifo->size -1)));
 memcpy(buffer, fifo->buffer + (fifo->out & (fifo->size -1)), l);
 
 /* then get the rest (if any) from the beginning of the buffer*/
 memcpy(buffer + l, fifo->buffer, len - l);
 
 fifo->out += len;
 
 return len;
 }