/**
  ******************************************************************************
  * @file    DateTiemTest.cpp
  * @author  shentq
  * @version V2.0
  * @date    2021/07/23
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


/* Includes ------------------------------------------------------------------*/


#include "ebox.h"
#include "datetime.h"
#include "Linear.h"
#include "linearPiecewise.h"

LinearPiecewise lin;
void setup()
{
    ebox_init();
    uart1.begin(115200);
    lin.add_row(2,5);
    lin.add_row(1,3);
    lin.add_row(1,100);
    lin.add_row(4,9.5);
    lin.add_row(3,6.4);
    lin.exe();
    for(int i = -10 ; i < 60;i++)
    {
        double value = lin.get(i/10.0);
        uart1.printf("x=%0.1f,y=%0.3f\n",i/10.0,value);
    }
}
int main(void)
{

    setup();
    while(1)
    {

        delay_ms(1000);
    }
}




