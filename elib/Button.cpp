/*----------------------------------------------------------------------*
 * ebox Button Library v1.0                                           
 *----------------------------------------------------------------------*/

#include "Button.h"
/*----------------------------------------------------------------------*

 *----------------------------------------------------------------------*/
Button::Button(void)
{

}
Button::Button(uint8_t pin, uint8_t puEnable)
{
    _pin = pin;
    _puEnable = puEnable;
    pinMode(_pin, INPUT);
    if (_puEnable != 0){
        digitalWrite(_pin, HIGH);       //enable pullup resistor
		 }
        
    _state = digitalRead(_pin);
    if (_puEnable == 0){
         _state = !_state;
	 }

    _time = millis();
    _lastState = _state;
    _changed = 0;
    _lastChange = _time;
}

uint8_t Button::read(void)
{
    static uint32_t ms;
    static uint8_t pinVal;

	ms = millis();
	pinVal = digitalRead(_pin);
	if(_puEnable == 0) pinVal = !pinVal;

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

//等待按键松开后才返回1；
uint8_t Button::Release(void)
{
	if(_state&&_changed){
		_changed = 0;
		return 1;
	}
	else
		return 0;
}
//按键按下返回1；
uint8_t Button::Click(void)
{
	if(!_state&&_changed){
		_changed = 0;
		return 1;
	}
	else
		return 0;
}

//
//ms: 长按按键ms 后触发
//times: 触发times后将不再触发
//times=0;则连续触发
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



