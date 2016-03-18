/*
file   : ringbuf.cpp
author : shentq
version: V1.0
date   : 2016/1/28

Copyright 2015 shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/

#include "ringbuf.h"
RINGBUF::RINGBUF()
{
    head = 0;
    tail = 0;
}
void RINGBUF::begin(unsigned char *buf, int lenght)
{
    head = 0;
    tail = 0;
    this->buf = buf;
    this->max = lenght;
    for(int i = 0; i < lenght; i++)
        buf[i] = 0;
}
bool RINGBUF::write(unsigned char c)
{
    int i = (head + 1) % max;

    // If the output buffer is full, there's nothing for it other than to
    // wait for the interrupt handler to empty it a bit
    // ???: return 0 here instead?
    while (i == tail)
        ;

    buf[head] = c;
    head = i;

}
unsigned char RINGBUF::read(void)
{
    // if the head isn't ahead of the tail, we don't have any characters
    if (head == tail)
    {
        return -1;
    }
    else
    {
        unsigned char c = buf[tail];
        tail = (unsigned int)(tail + 1) % max;
        return c;
    }
}

int RINGBUF::available()
{
    return (int)(max + head - tail) % max;
}
void RINGBUF::clear()
{
    head = 0;
    tail = 0;
    for(int i = 0; i < max; i++)
    {
        buf[i] = 0;
    }
}
