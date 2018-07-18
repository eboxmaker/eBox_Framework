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
#include "cnc.h"
CNC cnc;
float position[3] = {50,10,0};
float position2[3] = {10,50,0};
void setup()
{
    ebox_init();
    uart1.begin(512000);
    steper.Xpwm = &PB8;
    steper.Ypwm = &PB9;
    steper.begin();
    cnc.print_info();
    
 
}
int main(void)
{
    setup();
    while(1)
    {
    cnc.move(position);
    cnc.move(position2);
    cnc.move_signal_to(Y_AXIS,50.23);
    cnc.move_signal_to(X_AXIS,50.23);
//        uart1.printf("hello World !\r\n");
        delay_ms(1000);
    }
}




