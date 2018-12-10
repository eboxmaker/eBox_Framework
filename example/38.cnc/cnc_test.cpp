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
#include "bsp.h"
double position1[3] = {0, 0, 0};
double position2[3] = {5, 1, 0};
double position3[3] = {1, 5, 0};
double position4[3] = {1, 100, 0};
void setup()
{
    ebox_init();
    uart1.begin(512000);
    steper.Xpwm = &PB8;//电机X
    steper.Ypwm = &PB9;//电机Y
    steper.begin();
    cnc.begin();
    cnc.print_info();

}
int main(void)
{
    setup();
    while(1)
    {
        cnc.move(position1);
        while(!cnc.is_motion_over());
        cnc.print_position();

        cnc.move(position2);
        while(!cnc.is_motion_over());
        cnc.print_position();

        cnc.move(position3);
        while(!cnc.is_motion_over());
        cnc.print_position();

        cnc.move(position4);
        while(!cnc.is_motion_over());
        cnc.print_position();

        cnc.move(position2);
        while(!cnc.is_motion_over());
        cnc.print_position();
        cnc.move_signal_to(X_AXIS, 10.23);
        while(!cnc.is_motion_over());
        cnc.print_position();
        //    cnc.move_signal_to(X_AXIS,0);
        //        delay_ms(2000);
        //    cnc.print_position();
        //        uart1.printf("hello World !\r\n");
    }
}




