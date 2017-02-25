/**
  ******************************************************************************
  * @file   : *.cpp
  * @author : shentq
  * @version: V1.2
  * @date   : 2016/08/14

  * @brief   ebox application example .
  *
  * Copyright 2016 shentq. All Rights Reserved.         
  ******************************************************************************
 */
 
 
#include "ebox.h"
#include "color_convert.h"
#include "bsp.h"
COLOR_HSV hsv;
COLOR_RGB rgb;
CanTxMsg TxMessage;
uint8_t data[8] = {1, 2, 3, 4, 5, 6, 7, 8};


u8 index = 0x20;
u8 r;
u16 _color[3600];

void setup()
{
    ebox_init();
//    PB8.mode(OUTPUT_PP);
    can1.begin(BSP_CAN_500KBPS);
    /* Transmit */
    TxMessage.StdId = 0x321;
    TxMessage.RTR = CAN_RTR_DATA;
    TxMessage.IDE = CAN_ID_STD;
    TxMessage.DLC = 8;
    int i = 0;
    TxMessage.Data[i++] = 0x31;
    TxMessage.Data[i++] = 0x32;
    TxMessage.Data[i++] = 0x33;
    TxMessage.Data[i++] = 0x34;
    TxMessage.Data[i++] = 0x35;
    TxMessage.Data[i++] = 0x36;
    TxMessage.Data[i++] = 0x37;
    TxMessage.Data[i++] = 0x38;

    lcd.begin(1);
    lcd.clear(RED);
    uart1.begin(9600);

    lcd.column_order(1);
    lcd.row_order(1);

    lcd.front_color = RED;
    lcd.back_color = BLACK;
    hsv.s = 1;
    hsv.v = 0.5;
    hsv.h = 0;

    lcd.front_color = RED;


    lcd.printf(2, 2, "can tx");





}
int main(void)
{
    setup();
    while(1)
    {
        can1.write(&TxMessage);
        delay_ms(1000);
    }

}


