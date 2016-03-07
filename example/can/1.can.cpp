/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox
#include "ebox.h"
#include "can.h"



CanTxMsg TxMessage;
uint8_t data[8] = {1,2,3,4,5,6,7,8};
void setup()
{
	ebox_init();
    can.begin(BSP_CAN_500KBPS);
	PB8.mode(OUTPUT_PP);
  /* Transmit */
  TxMessage.StdId = 0x321;
  TxMessage.ExtId = 0x01;
  TxMessage.RTR = CAN_RTR_DATA;
  TxMessage.IDE = CAN_ID_STD;
  TxMessage.DLC = 8;
    int i = 0;
    TxMessage.Data[i++] = 1;
    TxMessage.Data[i++] = 2;
    TxMessage.Data[i++] = 3;
    TxMessage.Data[i++] = 4;
    TxMessage.Data[i++] = 5;
    TxMessage.Data[i++] = 6;
    TxMessage.Data[i++] = 7;
    TxMessage.Data[i++] = 8;
}
int main(void)
{
	setup();
	while(1)
	{
        can.write(&TxMessage);
        delay_ms(1000);

	}

}


