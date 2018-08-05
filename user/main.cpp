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
double position1[3] = {0,0,0};
double position2[3] = {10,50,0};
double position3[3] = {0.64,0.8,0};
double position4[3] = {1,100,0};
double y,y1,y2,x;

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
    lcd.draw_circle(64,80,50,BLACK);
//    test();

    int x,y;
    float t;
    while(1)
    {
        x = 64+50*sin(TWO_PI*t/2);
        y = 80+50*cos(TWO_PI*t/2);
        t+=0.01;
        lcd.draw_pixel(x,y,BLACK);
        if(t > TWO_PI)break;
    }
//    for(float x = -1; x < 2; x++)
//    {
//        for(float y = -1; y < 2; y+=0.5)
//            cnc.guaxiang(x,y);
//    }
//    while(1);
//    cnc.bfd_circle();
//    while(1);

//    cnc.test_circle(64,80,50);
//    cnc.ppc_circle(50,0,0,50,50,0);
//    cnc.ppc_circle(0,50,-50,0,50,0);
    cnc.dda_circle(50,2,-50,0,50,0);
    while(!cnc.is_motion_over());
    cnc.dda_circle(50,2,-50,0,50,0);
    while(!cnc.is_motion_over());
    cnc.dda_circle(50,2,-50,0,50,0);
    while(1);
    cnc.move(position2);
    while(!cnc.is_motion_over());
    cnc.print_position();
    cnc.move(position3);
    while(!cnc.is_motion_over());
    cnc.print_position();
    while(1);
//    for(int i = -60 ; i < 61; i++)
//    {
//        x = i/10.0;
//        y = 1/(1+pow(EULER,-x));
//        y1 = (pow(EULER,-x) )/ pow((1+pow(EULER,-x)),2);
//        y2 = -pow(EULER,-x)*(3 - pow(EULER,-x))/pow((1+pow(EULER,-x)),3);
//        //y1 = 1/((-x)*pow(EULER,-x-1));
//        uart1.printf("%f\t%f\t%f\r\n",y,y1,y2);
//    }
//    while(1);
//    uart1.printf("euler:%0.30lf",EULER);
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
    cnc.move_signal_to(X_AXIS,10.23);
    while(!cnc.is_motion_over());
    cnc.print_position();
//    cnc.move_signal_to(X_AXIS,0);
//        delay_ms(2000);
//    cnc.print_position();
//        uart1.printf("hello World !\r\n");
    }
}




