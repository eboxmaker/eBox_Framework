#include "ebox_core.h"
#include "ebox_mem.h"

#include "fifo.h"
 
// #define min(a,b) ((a) < (b) ? (a):(b))
 
 
 /**
 *@brief    根据缓冲区和缓冲区大小初始化链表
 *@param    buffer  :   数据缓冲区
            size    :   数据缓冲区长度，必须是2的n次方
 *@retval   ebox_fifo对象的指针
*/
struct ebox_fifo *ebox_fifo_init(unsigned char *buffer,unsigned int size)
{
    struct ebox_fifo *fifo;

    fifo = ebox_malloc(sizeof(struct ebox_fifo));
    if (fifo == NULL)
        return NULL;

    fifo->buffer = buffer;
    fifo->size = size;
    fifo->in = fifo->out = 0;

    return fifo;
}
 
 
 /**
 *@brief    从buffer中向fifo中写入len长度的内容，
            此函数有一定的数据丢失风险，必须检查
            实际写入长度和要求写入长度的关系，并做出处理
 *@param    size     :   申请内存的长度，必须是2的n次方
 *@retval   ebox_fifo对象的指针
*/
 struct ebox_fifo *ebox_fifo_alloc(unsigned int size)
 {
     unsigned char *buffer = NULL;
     struct ebox_fifo *ret = NULL;
     /*
     * round up to the next power of 2, since our 'let the indices
     * wrap' tachnique works only in this case.
     */
     
     buffer = ebox_malloc(size);
     if (buffer == NULL)
        return NULL;
     
     ret = ebox_fifo_init(buffer, size);
     
     if (ret == NULL)
        return NULL;

     return ret;
 }
 
 
 /**
 *@brief    释放fifo的对象和对象中的缓冲区
 *@param    fifo    :   fifo对象的指针
 *@retval   NONE
*/
void ebox_fifo_free(struct ebox_fifo *fifo)
{
     ebox_free(fifo->buffer);
     ebox_free(fifo);
}


 /**
 *@brief    从buffer中向fifo中写入len长度的内容，
            此函数有一定的数据丢失风险，必须检查
            实际写入长度和要求写入长度的关系，并做出处理
 *@param    fifo    :   fifo对象的指针
            buffer  :   数据缓冲区
            len     :   要求写入的长度
 *@retval   实际写入数据长度
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

 
 /**
 *@brief    从fifo中读取len长度的内容，存储到buffer中
 *@param    fifo    :   fifo对象的指针
            buffer  :   数据缓冲区
            len     :   要求读取的长度
 *@retval   实际读出数据长度
*/
unsigned int ebox_fifo_get(struct ebox_fifo *fifo,unsigned char *buffer, unsigned int len)
{
    unsigned int l;

    //要求读取长度和可读数据长度去最小*/
    len = min(len, fifo->in - fifo->out); 

    /* first get the data from fifo->out until the end of the buffer*/
    l = min(len, fifo->size - (fifo->out & (fifo->size -1)));
    ebox_memcpy(buffer, fifo->buffer + (fifo->out & (fifo->size -1)), l);

    /* then get the rest (if any) from the beginning of the buffer*/
    ebox_memcpy(buffer + l, fifo->buffer, len - l);

    fifo->out += len;

    return len;
}
