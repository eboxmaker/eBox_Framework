/**
  ******************************************************************************
  * @file    tm1638.h
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
#ifndef  __TM1638_H
#define  __TM1638_H

#include "ebox.h"

//uint8_t static DisBuffer[8]={0,0,0,0,0,0,0,0};    //  /*显示缓存区*/	//各个数码管显示的值
static uint8_t const tab[] =
{
    0x3F,/*0*/
    0x06,/*1*/
    0x5B,/*2*/
    0x4F,/*3*/
    0x66,/*4*/
    0x6D,/*5*/
    0x7D,/*6*/
    0x07,/*7*/
    0x7F,/*8*/
    0x6F,/*9*/
    0x48,/*:*/
    0xC8,/*;*/
    0x00,/*<*/
    0x48,/*=*/
    0x4C,/*>*/
    0xD3,/*?*/
    0xDC,/*@*/
    0x77,/*A*/
    0x7C,/*b*/
    0x39,/*C*/
    0x5E,/*d*/
    0x79,/*E*/
    0x71,/*F*/
    0x3D,/*G*/
    0x76,/*H*/
    0x30,/*I*/
    0x3C,/*J*/
    0x78,/*K*/
    0x38,/*L*/
    0xD4,/*m*/
    0x54,/*n*/
    0x5C,/*o*/
    0x73,/*P*/
    0xBF,/*Q*/
    0x50,/*r*/
    0x6D,/*S*/
    0x78,/*t*/
    0x1C,/*u*/
    0x3E,/*V*/
    0x9C,/*w*/
    0xF6,/*X*/
    0x72,/*y*/
    0x5B,/*z*/
    0x40,/*-*/
    0x00 /* */
};


class Tm1638
{
public:

    Tm1638(Gpio *p_data_pin, Gpio *p_sck_pin, Gpio *p_rck_pin);
    void begin();
    void delay(uint16_t delayvalue);
    void write(uint8_t DATA);
    void writeCmd(uint8_t Cmd);
    uint8_t read( );
    uint16_t readkey();
    void LedDisplay(uint8_t ddata0, uint8_t ddata1, uint8_t ddata2, uint8_t ddata3, uint8_t ddata4, uint8_t ddata5, uint8_t ddata6, uint8_t ddata7, uint8_t ddata8, uint8_t ddata9);

private:
    Gpio *data_pin;
    Gpio *sck_pin;
    Gpio *rck_pin;
};

#endif


