/**
  ******************************************************************************
  * @file    color_convert.h
  * @author  shentq
  * @version V1.2
  * @date    2016/08/14
  * @brief   
  ******************************************************************************
  * @attention
  *
  * No part of this software may be used for any commercial activities by any form 
  * or means, without the prior written consent of shentq. This specification is 
  * preliminary and is subject to change at any time without notice. shentq assumes
  * no responsibility for any errors contained herein.
  * <h2><center>&copy; Copyright 2015 shentq. All Rights Reserved.</center></h2>
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RINGBUF_H
#define __RINGBUF_H

#include "ebox_type.h"
#include "ebox_mem.h"

/**
ʹ�÷�����
�û�����һ���Զ����unsigned char/uint8_t���͵Ļ�����.���磺uint8_t buf[512];
ʹ��RINGBUF ringbuf(buf,512);����һ������
�û����Ե���ringbuf.write(c);�������ݣ�����true�ɹ���
����ringbuf.available()���ɻ�û������������ݵĳ��ȣ�0��ʾ�գ�
�����Ϊ�գ����Ե���ringbuf.read()��ȡһ���ֽڣ�
Ҳ���Ե���ringbuf.read(userbuf,length);����ȡ�ض����ȵ����ݣ������userbuf�С�
*/
class RINGBUF
{


public:
    RINGBUF();
    void begin(unsigned char *buf, int lenght);
    void write(unsigned char c);
    unsigned char read(void);
    int available();
    void clear();

private:
    volatile int head;
    volatile int tail;
    int max;
    unsigned char *buf;

};


template <class T>
class RingBuf
{


public:
    RingBuf()
    {
        head = 0;
        tail = 0;
        buf = 0;
    }
    
    
    ~RingBuf()
    {
        ebox_free(buf);
    }
    
    bool begin(uint16_t size)
    {
        buf = (T *)ebox_malloc(size*sizeof(T));
        max = size;
        if(buf != NULL)
            return true;
        else
            return false;
    }
    bool write(T c)
    {
        int i = (head + 1) % max;

        // If the output buffer is full, there's nothing for it other than to
        // wait for the interrupt handler to empty it a bit
        if (i == tail) return false;
        buf[head] = c;
        head = i;
        return true;

    }
    
    
    
    T read(void)
    {
        // if the head isn't ahead of the tail, we don't have any characters
        if (head == tail)
        {
            return 0;
        }
        else
        {
            T c = buf[tail];
            tail = (unsigned int)(tail + 1) % max;
            return c;
        }
    }
    int available()
    {
        return (int)(max + head - tail) % max;
    }
    void clear()
    {
        head = 0;
        tail = 0;
        for(int i = 0; i < max; i++)
        {
            buf[i] = 0;
        }
    }

    bool isfull()
    {
        if(((head + 1) % max) == tail)
            return true;
        else
            return false;
    }
//private:
    volatile int head;
    volatile int tail;
    int max;
    T *buf;
};
typedef RingBuf<uint8_t>    RingBufUint8;
typedef RingBuf<int8_t>     RingBufInt8;
typedef RingBuf<uint16_t>   RingBufUint16;
typedef RingBuf<int16_t>    RingBufInt16;
typedef RingBuf<uint32_t>   RingBufUint32;
typedef RingBuf<int32_t>    RingBufInt32;
typedef RingBuf<float>      RingBufFloat;


#endif
