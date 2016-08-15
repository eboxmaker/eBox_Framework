/**
  ******************************************************************************
  * @file    button.cpp
  * @author  shentq
  * @version V1.2
  * @date    2016/08/14
  * @brief   
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
#include "Button.h"
/*----------------------------------------------------------------------*

 *----------------------------------------------------------------------*/
Button::Button(Gpio *pin, uint8_t pull_up)
{
    this->pin       = pin;
    this->pull_up   = pull_up;
    this->state     = pull_up;
}

void Button::begin()
{
    if (pull_up != 0)
    {
        pin->mode(INPUT_PU);
    }
    else
        pin->mode(INPUT_PD);

    //state = pin->read();//由于io反应慢导致读取此值默认一直为0；导致开机误触发一次按键释放
    if (pull_up == 0)
    {
        state = !state;
    }

    time = millis();

    last_state = state;
    changed = 0;
    last_change = time;
}
uint8_t Button::loop(void)
{
    static uint32_t ms;
    static uint8_t pinVal;

    ms = millis();
    pinVal = pin->read();
    if(pull_up == 0) pinVal = !pinVal;

    last_state = state;
    state = pinVal;
    time = ms;
    if(state != last_state)
    {
        if(long_press_flag == 1) //mask the signal fafter long pressed
        {
            long_press_flag = 0;
            long_press_times = 0;
            return state;
        }
        last_change = ms;
        changed = 1;
    }
    else
    {
        changed = 0;
    }
    return state;

}

//等待按键松开后才返回1；
uint8_t Button::release(void)
{
    if(state && changed)
    {
        changed = 0;
        return 1;
    }
    else
        return 0;
}
//按键按下返回1；
uint8_t Button::click(void)
{
    if(!state && changed)
    {
        changed = 0;
        return 1;
    }
    else
        return 0;
}

//
//ms: 长按按键ms 后触发
//times: 触发times后将不再触发
//times=0;则连续触发
uint8_t Button::pressed_for(uint32_t ms, uint8_t times)
{
    if(times == 0)
    {
        if(state == 0 && time - last_change >= ms )
        {
            long_press_flag = 1;
            return 1;
        }
        else
            return 0;
    }

    else if(state == 0 && time - last_change >= ms && long_press_times < times)
    {
        long_press_flag = 1;
        long_press_times++;
        return 1;
    }
    else
        return 0;



}



