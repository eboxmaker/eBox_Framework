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
#include "interface/basicRtc.h"

    
void DS3231::begin()
{  
    if(intPin != NULL)
    {
        exti = new Exti(intPin);
        exti->begin();
        exti->attach(this,&DS3231::event,RISE);
        exti->nvic(ENABLE);
        exti->interrupt(RISE,ENABLE);
    }
    i2c->begin(&cfg);
    set_hour_mode(HOUR24);
    
    uint8_t temp;
    i2c->take(&cfg);
    temp = i2c->read(slaveAddr,DS3231_CONTROL);
    temp = temp & B11111011;
    i2c->write(slaveAddr,DS3231_CONTROL,temp);
    i2c->release();
    
}

//uint8_t slaveAddr, uint8_t regAddr, uint8_t *data, uint16_t nRead
DateTime_t DS3231::read_dt()
{
    
    uint8_t buf[8];
    i2c->take(&cfg);
    i2c->read_buf(slaveAddr,DS3231_SECOND, buf, 7);
    i2c->release();

    //	timer.w_year,timer.w_month,timer.w_date,timer.hour,timer.min,timer.sec
    /******将读取的十六进制数据转换为十进制数据******/
    dateTime.year 	= bcd_to_dec(buf[6]);
    dateTime.month	= bcd_to_dec(buf[5]);
    dateTime.date		= bcd_to_dec(buf[4]);
    dateTime.week    = bcd_to_dec(buf[3]);
    dateTime.hour		= bcd_to_dec(buf[2]);
    dateTime.min		= bcd_to_dec(buf[1]);
    dateTime.sec	  = bcd_to_dec(buf[0]);
    return dateTime;
}

void	DS3231::write_dt(DateTime_t &dt)
{
    DateTime_t tBCD;
    dt_check(dt);
    tBCD.week = dec_to_bcd(dt.week);
    tBCD.year = dec_to_bcd(dt.year);
    tBCD.month = dec_to_bcd(dt.month);
    tBCD.date = dec_to_bcd(dt.date);
    tBCD.hour = dec_to_bcd(dt.hour);
    tBCD.min = dec_to_bcd(dt.min);
    tBCD.sec = dec_to_bcd(dt.sec);

    

    i2c->take(&cfg);
//    i2c->read_buf(slaveAddr,DS3231_SECOND, buf, 7);

    i2c->write(slaveAddr,DS3231_WEEK, tBCD.week); //修改周
    i2c->write(slaveAddr,DS3231_YEAR, tBCD.year); //修改年
    i2c->write(slaveAddr,DS3231_MONTH, tBCD.month); //修改月
    i2c->write(slaveAddr,DS3231_DAY, tBCD.date);  //修改日
    i2c->write(slaveAddr,DS3231_HOUR, tBCD.hour); //修改时
    i2c->write(slaveAddr,DS3231_MINUTE, tBCD.min); //修改分
    i2c->write(slaveAddr,DS3231_SECOND, tBCD.sec ); //修改秒
    i2c->release();
}


void DS3231::set_hour_mode(HourMode_t mode)
{
    hour_mode = mode;
}
DS3231::HourMode_t DS3231::get_hour_mode()
{
    return hour_mode;
}
void DS3231::event()
{
    read_dt();
}


void DS3231::loop()
{
    if(millis() - last_time >= 1000)
    {
        read_dt();
        time_buf[0] = dateTime.sec;
        time_buf[1] = dateTime.min;
        time_buf[2] = dateTime.hour;
        time_buf[3] = dateTime.date;
        time_buf[4] = dateTime.month;
        time_buf[5] = dateTime.year;
    }
}


