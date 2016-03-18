/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox
#include "ebox.h"
void setup()
{
    ebox_init();
    PB8.mode(OUTPUT_PP);
}
int main(void)
{
    setup();
    while(1)
    {
        PB8.set();
        PB8.reset();
    }

}


