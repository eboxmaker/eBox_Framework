/*
file   : button.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentqlf@163.com.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentqlf@163.com assumes no responsibility for any errors contained herein.
*/

#include "Button.h"
/*----------------------------------------------------------------------*

 *----------------------------------------------------------------------*/
BUTTON::BUTTON(GPIO* Pin, uint8_t PullUp)
{
    pin = Pin;
    pull_up = PullUp;
}

void BUTTON::begin()
{
    pin->mode(INPUT);
    if (pull_up != 0){
        pin->write(HIGH);       //enable pullup resistor
		 }
        
    state = pin->read();
    if (pull_up == 0){
         state = !state;
	 }

    time = millis();
    last_state = state;
    changed = 0;
    last_change = time;
}
uint8_t BUTTON::loop(void)
{
    static uint32_t ms;
    static uint8_t pinVal;

	ms = millis();
	pinVal = pin->read();
	if(pull_up == 0) pinVal = !pinVal;

	last_state = state;
	state = pinVal;
	time = ms;

	if(state != last_state){
		if(long_press_flag == 1){//mask the signal fafter long pressed 
			long_press_flag = 0;
			long_press_times = 0;
			return state;
			}
		last_change = ms;
		changed = 1;
	}
	else{
		changed = 0;
	}
	return state;
	
}

//�ȴ������ɿ���ŷ���1��
uint8_t BUTTON::release(void)
{
	if(state&&changed){
		changed = 0;
		return 1;
	}
	else
		return 0;
}
//�������·���1��
uint8_t BUTTON::click(void)
{
	if(!state&&changed){
		changed = 0;
		return 1;
	}
	else
		return 0;
}

//
//ms: ��������ms �󴥷�
//times: ����times�󽫲��ٴ���
//times=0;����������
uint8_t BUTTON::pressed_for(uint32_t ms,uint8_t times)
{
	if(times == 0){
		if(state == 0 && time - last_change >= ms ){
			long_press_flag = 1;
			return 1;
		}
		else
			return 0;
	}
		
	else if(state == 0 && time - last_change >= ms && long_press_times < times){
			long_press_flag = 1;
			long_press_times++;
			return 1;
		}
		else
			return 0;



}



