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
#include "bsp_ebox.h"
double position0[3] = {0,0,0};
double position1[3] = {0,0.25,0};
double position2[3] = {0.64,0.8,0};
double position3[3] = {0.1,0.2,0};
double position4[3] = {0.9,1.2,0};

Lcd lcd(&PB5, &PB6, &PB4, &PB3, &spi1);
void setup()
{
    ebox_init();
    uart1.begin(115200);
    lcd.begin(1);
    lcd.clear(RED);
    lcd.column_order(1);
    lcd.row_order(0);
    
    steper.Xpwm = &PB8;//电机X
    steper.Ypwm = &PB9;//电机Y
    steper.begin();
    cnc.begin();
    cnc.print_info();
    lcd.draw_line(0,80,128,80,BLACK);
    lcd.draw_line(64,0,64,160,BLACK);
    lcd.draw_circle(64+25,80,25,BLACK);


}
int main(void)
{
    setup();
    while(1)
    {
    cnc.move(position1);
    while(!cnc.is_motion_over());
    cnc.print_position();


    cnc.dda_circle(0,25,-25,-0,0);

    while(!cnc.is_motion_over());
        
    cnc.move(position2);
    while(!cnc.is_motion_over());
    cnc.print_position();
        
    cnc.move(position3);
    while(!cnc.is_motion_over());
    cnc.print_position();
        
    cnc.move(position4);
    while(!cnc.is_motion_over());
    cnc.print_position();
        


//    while(1);
//    cnc.move_signal_to(X_AXIS,0);
//        delay_ms(2000);
//    cnc.print_position();
//        uart1.printf("hello World !\r\n");
    }
}




