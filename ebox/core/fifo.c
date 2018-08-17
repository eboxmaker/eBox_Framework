#include "ebox_core.h"
#include "ebox_mem.h"

#include "fifo.h"
 
// #define min(a,b) ((a) < (b) ? (a):(b))
 
 
 /**
 *@brief    ���ݻ������ͻ�������С��ʼ������
 *@param    buffer  :   ���ݻ�����
            size    :   ���ݻ��������ȣ�������2��n�η�
 *@retval   ebox_fifo�����ָ��
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
 *@brief    ��buffer����fifo��д��len���ȵ����ݣ�
            �˺�����һ�������ݶ�ʧ���գ�������
            ʵ��д�볤�Ⱥ�Ҫ��д�볤�ȵĹ�ϵ������������
 *@param    size     :   �����ڴ�ĳ��ȣ�������2��n�η�
 *@retval   ebox_fifo�����ָ��
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
 *@brief    �ͷ�fifo�Ķ���Ͷ����еĻ�����
 *@param    fifo    :   fifo�����ָ��
 *@retval   NONE
*/
void ebox_fifo_free(struct ebox_fifo *fifo)
{
     ebox_free(fifo->buffer);
     ebox_free(fifo);
}


 /**
 *@brief    ��buffer����fifo��д��len���ȵ����ݣ�
            �˺�����һ�������ݶ�ʧ���գ�������
            ʵ��д�볤�Ⱥ�Ҫ��д�볤�ȵĹ�ϵ������������
 *@param    fifo    :   fifo�����ָ��
            buffer  :   ���ݻ�����
            len     :   Ҫ��д��ĳ���
 *@retval   ʵ��д�����ݳ���
*/
 unsigned int ebox_fifo_put(struct ebox_fifo *fifo,unsigned char *buffer, unsigned int len)
{
    unsigned int l;

    len = min(len, fifo->size - fifo->in + fifo->out);/*�����ǻ������Ŀ��г��Ȼ���Ҫд����*/

    /* first put the data starting from fifo->in to buffer end*/
    l = min(len, fifo->size - (fifo->in & (fifo->size -1)));
    ebox_memcpy(fifo->buffer + (fifo->in & (fifo->size -1)), buffer, l);

    /* then put the rest (if any) at the beginning of the buffer*/
    ebox_memcpy(fifo->buffer, buffer + l, len - l);

    fifo->in += len;

    return len;
}

 
 /**
 *@brief    ��fifo�ж�ȡlen���ȵ����ݣ��洢��buffer��
 *@param    fifo    :   fifo�����ָ��
            buffer  :   ���ݻ�����
            len     :   Ҫ���ȡ�ĳ���
 *@retval   ʵ�ʶ������ݳ���
*/
unsigned int ebox_fifo_get(struct ebox_fifo *fifo,unsigned char *buffer, unsigned int len)
{
    unsigned int l;

    //Ҫ���ȡ���ȺͿɶ����ݳ���ȥ��С*/
    len = min(len, fifo->in - fifo->out); 

    /* first get the data from fifo->out until the end of the buffer*/
    l = min(len, fifo->size - (fifo->out & (fifo->size -1)));
    ebox_memcpy(buffer, fifo->buffer + (fifo->out & (fifo->size -1)), l);

    /* then get the rest (if any) from the beginning of the buffer*/
    ebox_memcpy(buffer + l, fifo->buffer, len - l);

    fifo->out += len;

    return len;
}
