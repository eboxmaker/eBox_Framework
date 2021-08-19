/**
  ******************************************************************************
  * @file    ds3231.cpp
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
#include "DS3231.h"

void test()
{

}

void DS3231::begin()
{  
    if(intPin != NULL)
    {
        exti = new Exti(intPin);
        exti->begin();
        exti->attach(this,&DS3231::event);
        exti->nvic(ENABLE);
        exti->interrupt(Exti::TrigRise,ENABLE);
    }
    i2c->begin();
    set_hour_mode(HOUR24);
    
    
    uint8_t temp;
    i2c->requestFrom(slaveAddr,1,DS3231_CONTROL,1,true);
    if(i2c->available())
    {
        temp = i2c->read();
    }
    temp = temp & B11111011;

    
    i2c->beginTransmission(slaveAddr);
    i2c->write(DS3231_CONTROL);
    i2c->write(temp);
    i2c->endTransmission();
    
    
}
void DS3231::begin(DateTime &dt)
{
    begin();
    set(dt);
}
void DS3231::update()
{
    read_dt();
}
void DS3231::set(DateTime &dt)
{
    
    dateTime = dt;
    
    uint8_t buf[8];
    int i = 0;
    buf[i++] = dec_to_bcd(dt.second);
    buf[i++] = dec_to_bcd(dt.minute);
    buf[i++] = dec_to_bcd(dt.hour);
    buf[i++] = dec_to_bcd(dt.dayOfWeek());
    buf[i++] = dec_to_bcd(dt.day);
    buf[i++] = dec_to_bcd(dt.month);
    buf[i++] = dec_to_bcd(dt.year - 2000);

    i2c->beginTransmission(slaveAddr);
    i2c->write(DS3231_SECOND);
    i2c->write(buf,i);
    i2c->endTransmission();
}
DateTime DS3231::now()
{
    return dateTime;
}
  
//uint8_t slaveAddr, uint8_t regAddr, uint8_t *data, uint16_t nRead
void DS3231::read_dt()
{
    
    uint8_t buf[8];
    int i = 0;
    uint8_t num = i2c->requestFrom(slaveAddr,7,DS3231_SECOND,1,true);
    while(i2c->available())
    {
        buf[i] = i2c->read();
        i++;
    }
    
    //	timer.w_year,timer.w_month,timer.w_date,timer.hour,timer.min,timer.sec
    /******将读取的十六进制数据转换为十进制数据******/
    dateTime.year 	= bcd_to_dec(buf[6]) + 2000;
    dateTime.month	= bcd_to_dec(buf[5]);
    dateTime.day		= bcd_to_dec(buf[4]);
//    dateTime.week    = bcd_to_dec(buf[3]);
    dateTime.hour		= bcd_to_dec(buf[2]);
    dateTime.minute		= bcd_to_dec(buf[1]);
    dateTime.second	  = bcd_to_dec(buf[0]);
    return ;
}


void DS3231::set_hour_mode(HourMode_t mode)
{

}

void DS3231::event()
{
    event_flag = true;
}


void DS3231::loop()
{
    if(millis() - last_time >= 1000 || event_flag == true)
    {
        last_time = millis();
        event_flag = false;
        read_dt();
    }
}


