 /**
  ******************************************************************************
  * @file    main.cpp
  * @author  shentq
  * @version V1.2
  * @date    2016/08/14
  * @brief   ebox application example .
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

#include "ebox.h"

#define N 10
u8 src_array[N];
u8 array_len = N - 1;

void random_init()
{
    for(int i = 0; i < N; i++)
        src_array[i] = i;
}
u8 random_no_repeat()
{
    u8 temp;

    if(array_len == 0)
        return src_array[0];
    else
    {
        u8 seed = random(array_len);
        temp = src_array[seed];
        if(seed != (array_len))
            src_array[seed] = src_array[array_len];
        array_len--;
        return temp;
    }
}


void setup()
{
    ebox_init();
    uart1.begin(9600);
    random_init();
    for(int i = 0; i < N; i++)
    {
        uart1.printf("%d:random=%d\r\n", i, random_no_repeat());
        delay_ms(100);
    }

}
int main(void)
{
    setup();
    while(1)
    {
    }
}




