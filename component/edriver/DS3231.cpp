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

    
void DS3231::begin()
{  
    i2c->begin(&cfg);
}
uint8_t DS3231::bcd_to_dec(uint8_t bcd_code)
{
    uint8_t temp, dec;
    temp = (bcd_code >> 4) * 10;
    dec = (bcd_code & 0x0f) + temp;
    return dec;
}
uint8_t DS3231::dec_to_bcd(uint8_t dec)
{
    uint8_t temp, temp2, bcd;
    temp = dec / 10;
    temp2 = dec % 10;

    bcd =  (temp << 4) + temp2;

    return bcd;
}
//uint8_t slaveAddr, uint8_t regAddr, uint8_t *data, uint16_t nRead
void	DS3231::get_date_time(DateTime_t *t)
{
    uint8_t buf[8];
    i2c->take(&cfg);
    i2c->read_buf((uint8_t)DS3231_SECOND, buf, 7);
    i2c->release();

    //	timer.w_year,timer.w_month,timer.w_date,timer.hour,timer.min,timer.sec
    /******����ȡ��ʮ����������ת��Ϊʮ��������******/
    t->year 	= bcd_to_dec(buf[6]);
    t->month	= bcd_to_dec(buf[5]);
    t->date		= bcd_to_dec(buf[4]);
    t->week 	= bcd_to_dec(buf[3]);
    t->hour		= bcd_to_dec(buf[2]);
    t->min		= bcd_to_dec(buf[1]);
    t->sec	  = bcd_to_dec(buf[0]);
}
void DS3231::get_date(char *buf)
{
    uint8_t tmpbuf[3];
    i2c->take(&cfg);

    i2c->read_buf((uint8_t)DS3231_DAY, tmpbuf, 3);				//����
    i2c->release();

    buf[0] = uint8_t( (tmpbuf[0] >> 4) + 0x30);
    buf[1] = uint8_t( (tmpbuf[0] & 0x0f) + 0x30);
    buf[2] = uint8_t( (tmpbuf[1] >> 4) + 0x30);
    buf[3] = uint8_t( (tmpbuf[1] & 0x0f) + 0x30);
    buf[4] = uint8_t( (tmpbuf[2] >> 4) + 0x30);
    buf[5] = uint8_t( (tmpbuf[2] & 0x0f) + 0x30);
    buf[6] = '\0';
}
void DS3231::get_time(char *buf)
{
    uint8_t tmpbuf[3];

    i2c->take(&cfg);
    i2c->read_buf( (uint8_t)DS3231_SECOND, tmpbuf, 3);
    i2c->release();

    buf[0] = uint8_t( (tmpbuf[2] >> 4) + 0x30);
    buf[1] = uint8_t( (tmpbuf[2] & 0x0f) + 0x30);
    buf[2] = uint8_t( (tmpbuf[1] >> 4) + 0x30);
    buf[3] = uint8_t( (tmpbuf[1] & 0x0f) + 0x30);
    buf[4] = uint8_t( (tmpbuf[0] >> 4) + 0x30);
    buf[5] = uint8_t( (tmpbuf[0] & 0x0f) + 0x30);
    buf[6] = '\0';


}
void	DS3231::set_time(void *dt)
{
    DateTime_t tBCD;
    DateTime_t *t = (DateTime_t *)dt;

    tBCD.week = dec_to_bcd(t->week);
    tBCD.year = dec_to_bcd(t->year);
    tBCD.month = dec_to_bcd(t->month);
    tBCD.date = dec_to_bcd(t->date);
    tBCD.hour = dec_to_bcd(t->hour);
    tBCD.min = dec_to_bcd(t->min);
    tBCD.sec = dec_to_bcd(t->sec);


    i2c->take(&cfg);
    i2c->write( (uint8_t)DS3231_WEEK, tBCD.week); //�޸���
    i2c->write( (uint8_t)DS3231_YEAR, tBCD.year); //�޸���
    i2c->write( (uint8_t)DS3231_MONTH, tBCD.month); //�޸���
    i2c->write( (uint8_t)DS3231_DAY, tBCD.date);  //�޸���
    i2c->write( (uint8_t)DS3231_HOUR, tBCD.hour); //�޸�ʱ
    i2c->write( (uint8_t)DS3231_MINUTE, tBCD.min); //�޸ķ�
    i2c->write( (uint8_t)DS3231_SECOND, tBCD.sec ); //�޸���
    i2c->release();
}

