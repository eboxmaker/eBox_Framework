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
Button::Button(GPIO* pin, uint8_t pullUpEnable)
{
    _pin = pin;
    _pullUpEnable = pullUpEnable;
}

void Button::begin()
{
    _pin->mode(INPUT);
    if (_pullUpEnable != 0){
        _pin->write(HIGH);       //enable pullup resistor
		 }
        
    _state = _pin->read();
    if (_pullUpEnable == 0){
         _state = !_state;
	 }

    _time = millis();
    _lastState = _state;
    _changed = 0;
    _lastChange = _time;
}
uint8_t Button::loop(void)
{
    static uint32_t ms;
    static uint8_t pinVal;

	ms = millis();
	pinVal = _pin->read();
	if(_pullUpEnable == 0) pinVal = !pinVal;

	_lastState = _state;
	_state = pinVal;
	_time = ms;

	if(_state != _lastState){
		if(_longpressflag == 1){//mask the signal fafter long pressed 
			_longpressflag = 0;
			_longpresstimes = 0;
			return _state;
			}
		_lastChange = ms;
		_changed = 1;
	}
	else{
		_changed = 0;
	}
	return _state;
	
}

//�ȴ������ɿ���ŷ���1��
uint8_t Button::release(void)
{
	if(_state&&_changed){
		_changed = 0;
		return 1;
	}
	else
		return 0;
}
//�������·���1��
uint8_t Button::click(void)
{
	if(!_state&&_changed){
		_changed = 0;
		return 1;
	}
	else
		return 0;
}

//
//ms: ��������ms �󴥷�
//times: ����times�󽫲��ٴ���
//times=0;����������
uint8_t Button::pressedFor(uint32_t ms,uint8_t times)
{
	if(times == 0){
		if(_state == 0 && _time - _lastChange >= ms ){
			_longpressflag = 1;
			return 1;
		}
		else
			return 0;
	}
		
	else if(_state == 0 && _time - _lastChange >= ms && _longpresstimes < times){
			_longpressflag = 1;
			_longpresstimes++;
			return 1;
		}
		else
			return 0;



}



