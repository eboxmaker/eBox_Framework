/**
  ******************************************************************************
  * @file    ds3231.h
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

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __DS3231_H__
#define __DS3231_H__

#include "ebox_core.h"
#include "basicRtc.h"
#include "ebox_uart.h"
#include "ebox_exti.h"

/*
 *������ֻ֧��24Сʱ��
*/

#define DS3231_ADDRESS 0xD0    //����д��ַ 
#define DS3231_SECOND       0x00    //��
#define DS3231_MINUTE       0x01    //��
#define DS3231_HOUR         0x02    //ʱ
#define DS3231_WEEK         0x03    //����
#define DS3231_DAY          0x04    //��
#define DS3231_MONTH        0x05    //��
#define DS3231_YEAR         0x06    //��
//����1
#define DS3231_SALARM1ECOND 0x07    //��
#define DS3231_ALARM1MINUTE 0x08    //��
#define DS3231_ALARM1HOUR   0x09    //ʱ
#define DS3231_ALARM1WEEK   0x0A    //����/��
//����2
#define DS3231_ALARM2MINUTE 0x0b    //��
#define DS3231_ALARM2HOUR   0x0c    //ʱ
#define DS3231_ALARM2WEEK   0x0d    //����/��
#define DS3231_CONTROL      0x0e    //���ƼĴ���
#define DS3231_STATUS       0x0f    //״̬�Ĵ���
#define BSY                 2       //æ
#define OSF                 7       //����ֹͣ��־
#define DS3231_XTAL         0x10    //�����ϻ��Ĵ���
#define DS3231_TEMPERATUREH 0x11    //�¶ȼĴ������ֽ�(8λ)
#define DS3231_TEMPERATUREL 0x12    //�¶ȼĴ������ֽ�(��2λ)  
//timer.w_year,timer.w_month,timer.w_date,timer.hour,timer.min,timer.sec

class DS3231:public BasicRtc
{

public:
    typedef enum{
        HOUR24,
        HOUR12,
    }HourMode_t;

public:
    DS3231(I2c *i2c,uint16_t slaveAddr)
    {
        this->i2c = i2c;
        this->intPin = NULL;
        this->slaveAddr = slaveAddr;
        cfg.speed = I2c::K200;
        cfg.regAddrBits = I2c::BIT8;
    };
    DS3231(I2c *i2c,Gpio *intPin,uint16_t slaveAddr)
    {
        this->i2c = i2c;
        this->slaveAddr = slaveAddr;
        this->intPin = intPin;
        cfg.speed = I2c::K200;
        cfg.regAddrBits = I2c::BIT8;
    };    
    
    virtual void        begin();
    virtual void        event();
    virtual void        loop();
    
private:
    virtual void        write_dt(DateTime_t &dt);
    virtual DateTime_t  read_dt();

    I2c *i2c;
    Gpio *intPin;
    Exti *exti;
    I2c::Config_t cfg;
    uint32_t last_time;
    uint16_t slaveAddr;
    void set_hour_mode(HourMode_t mode);
    HourMode_t get_hour_mode();

    HourMode_t hour_mode;

};


#endif
