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
#include "oneButton/oneButton.h"
OneButton btn(&PA8,false);
OneButton btn2(&PA7,false);
void click()
{
    uart1.printf("按键1click\n");
}
void clickbtn2()
{
    uart1.printf("按键2click\n");
}
void click_para(void* t)
{
    char *str = (char *)t;
    uart1.printf("按键click:%s\n",str);
}
void doubleclick()
{
    uart1.printf("按键双击\n");
}
void longPress()
{
    static uint32_t lastTime = 0;
    if(millis() - lastTime > 200)
    {
        lastTime = millis();
        uart1.printf("按键1长按\n");
    }
}
void longPressStart()
{
    uart1.printf("按键1长按开始\n");
}
void longPressStop()
{
    uart1.printf("按键1长按结束\n");
}
void setup()
{
    ebox_init();
    uart1.begin(115200);
    btn.begin();
    btn.attachClick(click);
    btn.attachClick(click_para,(void*)"btn1");
    btn.attachDoubleClick(doubleclick);
    btn.attachDuringLongPress(longPress);
    btn.attachLongPressStart(longPressStart);
    btn.attachLongPressStop(longPressStop);
    
    btn2.begin();
    btn2.attachClick(clickbtn2);
    btn2.attachClick(click_para,(void*)"btn2");
    btn2.attachDoubleClick(doubleclick);
    
}
int main(void)
{

    setup();
    while(1)
    {
        btn.loop();
        btn2.loop();
        delay_ms(1);
    }
}
