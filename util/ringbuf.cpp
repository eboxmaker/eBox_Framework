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
RINGBUF::RINGBUF(unsigned char *buf,int lenght)
{
    head = 0;
    tail = 0;
    num = 0;
    this->buf = buf;
    this->max = lenght;
    for(int i = 0; i < lenght; i++)
        buf[i] = 0;
}
bool RINGBUF::write(unsigned char c)
{
    if(num < max)
    {
        buf[tail] = c;
        tail = (tail + 1) % max;
        num++;
        return true;
    }
    else
        return false;
    
}
unsigned char RINGBUF::read(void)
{
    int seek;
    if(num > 0)
    {
        seek = head;
        head = (head + 1) % max;
        num--;
        return buf[seek];
    }
    else
        return 0;   
}
int RINGBUF::read(unsigned char *buf,int lenght)
{
    int i = 0;
    for(;i < lenght;i++)
    {
        if(num > 0)
        {
            buf[i] = this->buf[head];
            head = (head + 1) % max;
            num--;
        }
        else
            break;
    }
    return i;

}

int RINGBUF::available()
{
    return num;
}
void RINGBUF::clear()
{
    head = 0;
    tail = 0;
    num = 0;
    for(int i = 0; i < max; i++)
        buf[i] = 0;
}
