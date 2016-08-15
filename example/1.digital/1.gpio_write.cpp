/**
  ******************************************************************************
  * @file   : *.cpp
  * @author : shentq
  * @version: V1.2
  * @date   : 2016/08/14

  * @brief   ebox application example .
  *
  * Copyright 2016 shentq. All Rights Reserved.         
  ******************************************************************************
 */


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


