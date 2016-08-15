/**
  ******************************************************************************
  * @file    tm1638.cpp
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
 
#include "TM1638.h"

Tm1638::Tm1638(Gpio *p_data_pin, Gpio *p_sck_pin, Gpio *p_rck_pin)
{
    data_pin 	= p_data_pin;
    sck_pin 	= p_sck_pin;
    rck_pin 	= p_rck_pin;

}
void Tm1638::begin()
{
    data_pin->mode(OUTPUT_PP);
    sck_pin->mode(OUTPUT_PP);
    rck_pin->mode(OUTPUT_PP);

    data_pin->write(HIGH);
    sck_pin->write(HIGH);
    rck_pin->write(HIGH);

}

void Tm1638::delay(uint16_t delayvalue)
{
    uint16_t i = 0;
    for(i = 0; i < delayvalue; i++);
}

void Tm1638::write(uint8_t DATA)
{
    uint8_t i;
    for(i = 0; i < 8; i++)
    {
        sck_pin->write(LOW);
        delay(100);
        if(DATA & 0X01)
            data_pin->write(HIGH);
        else
            data_pin->write(LOW);
        DATA >>= 1;
        delay(100);
        sck_pin->write(HIGH);
        delay(100);
    }
}

void Tm1638::writeCmd(uint8_t Cmd)
{
    rck_pin->write(LOW);
    delay(10);
    write(Cmd);//shift_out(data_pin,sck_pin,bit_oder,Cmd);
    rck_pin->write(HIGH);
}

uint8_t Tm1638::read()
{
    uint8_t ReadValue = 0;
    uint8_t i;

    for(i = 0; i < 8; i++)
    {
        sck_pin->write(LOW);
        ReadValue = ReadValue << 1;
        delay(100);
        ReadValue |= data_pin->read();
        sck_pin->write(HIGH);
        delay(100);
    }
    return ReadValue;
}

uint16_t Tm1638::readkey()
{
    uint8_t c[4], i;
    uint16_t key_value = 0;
    rck_pin->write(LOW);
    delay(10);
    write(0x42);
    data_pin->mode(INPUT);
    delay(10);
    for(i = 0; i < 4; i++)
        c[i] = read();
    delay(10);
    data_pin->mode(OUTPUT_PP);
    delay(10);
    rck_pin->write(HIGH);					//4个字节数据合成一个字节
    key_value = c[1];
    key_value = ((key_value) << 8) | c[0];
    return key_value;
}

void Tm1638::LedDisplay(uint8_t ddata0, uint8_t ddata1, uint8_t ddata2, uint8_t ddata3,
                         uint8_t ddata4, uint8_t ddata5, uint8_t ddata6, uint8_t ddata7, uint8_t ddata8, uint8_t ddata9)

{
    uint8_t writedata0, writedata1, writedata2, writedata3, writedata4, writedata5, writedata6, writedata7;
    uint8_t data0, data1, data2, data3, data4, data5, data6, data7;
    data0 = ddata4 - 0x30;
    data1 = ddata5 - 0x30;
    data2 = ddata6 - 0x30;
    data3 = ddata7 - 0x30;
    data4 = ddata0 - 0x30;
    data5 = ddata1 - 0x30;
    data6 = ddata2 - 0x30;
    data7 = ddata3 - 0x30;

    writedata0 = (tab[data0] & 0x01) + ((tab[data1] & 0x01) << 1) + ((tab[data2] & 0x01) << 2) + ((tab[data3] & 0x01) << 3) + ((tab[data4] & 0x01) << 4)
                 + ((tab[data5] & 0x01) << 5) + ((tab[data6] & 0x01) << 6) + ((tab[data7] & 0x01) << 7);

    writedata1 = ((tab[data0] & 0x02) >> 1) + ((tab[data1] & 0x02)) + ((tab[data2] & 0x02) << 1) + ((tab[data3] & 0x02) << 2) + ((tab[data4] & 0x02) << 3)
                 + ((tab[data5] & 0x02) << 4) + ((tab[data6] & 0x02) << 5) + ((tab[data7] & 0x02) << 6);

    writedata2 = ((tab[data0] & 0x04) >> 2) + ((tab[data1] & 0x04) >> 1) + ((tab[data2] & 0x04)) + ((tab[data3] & 0x04) << 1) + ((tab[data4] & 0x04) << 2)
                 + ((tab[data5] & 0x04) << 3) + ((tab[data6] & 0x04) << 4) + ((tab[data7] & 0x04) << 5);

    writedata3 = ((tab[data0] & 0x08) >> 3) + ((tab[data1] & 0x08) >> 2) + ((tab[data2] & 0x08) >> 1) + ((tab[data3] & 0x08)) + ((tab[data4] & 0x08) << 1)
                 + ((tab[data5] & 0x08) << 2) + ((tab[data6] & 0x08) << 3) + ((tab[data7] & 0x08) << 4);

    writedata4 = ((tab[data0] & 0x10) >> 4) + ((tab[data1] & 0x10) >> 3) + ((tab[data2] & 0x10) >> 2) + ((tab[data3] & 0x10) >> 1) + ((tab[data4] & 0x10))
                 + ((tab[data5] & 0x10) << 1) + ((tab[data6] & 0x10) << 2) + ((tab[data7] & 0x10) << 3);

    writedata5 = ((tab[data0] & 0x20) >> 5) + ((tab[data1] & 0x20) >> 4) + ((tab[data2] & 0x20) >> 3) + ((tab[data3] & 0x20) >> 2) + ((tab[data4] & 0x20) >> 1)
                 + ((tab[data5] & 0x20)) + ((tab[data6] & 0x20) << 1) + ((tab[data7] & 0x20) << 2);

    writedata6 = ((tab[data0] & 0x40) >> 6) + ((tab[data1] & 0x40) >> 5) + ((tab[data2] & 0x40) >> 4) + ((tab[data3] & 0x40) >> 3) + ((tab[data4] & 0x40) >> 2)
                 + ((tab[data5] & 0x40) >> 1) + ((tab[data6] & 0x40)) + ((tab[data7] & 0x40) << 1);

    writedata7 = ddata8;
    writeCmd(0x8a);//亮度   Write_COM(0x8a);//亮度
    writeCmd(0x40); //写 数据命令
    delay(1);
    rck_pin->write(LOW);
    delay(1);
    write(0xc0);		//写地址命令
    write(writedata0);
    if((ddata9 & 0x01) > 0)
        write(0x01);
    else
        write(0x00);
    write(writedata1);
    if((ddata9 & 0x02) > 0)
        write(0x01);
    else
        write(0x00);
    write(writedata2);
    if((ddata9 & 0x04) > 0)
        write(0x01);
    else
        write(0x00);
    write(writedata3);
    if((ddata9 & 0x08) > 0)
        write(0x01);
    else
        write(0x00);
    write(writedata4);
    if((ddata9 & 0x10) > 0)
        write(0x01);
    else
        write(0x00);
    write(writedata5);
    if((ddata9 & 0x20) > 0)
        write(0x01);
    else
        write(0x00);
    write(writedata6);
    if((ddata9 & 0x40) > 0)
        write(0x01);
    else
        write(0x00);
    write(writedata7);
    if((ddata9 & 0x80) > 0)
        write(0x01);
    else
        write(0x00);

    rck_pin->write(HIGH);


}


