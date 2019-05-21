/**
  ******************************************************************************
  * @file    dmx512.cpp
  * @author  shentq
  * @version V1.0
  * @date    2019/05/21
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
#include "ebox_mem.h"
#include "dmx512.h"

void DMXReceiver::begin(uint16_t maxSlotNum)
{
    this->maxSlotNum = maxSlotNum;
    last_on_time = millis();
    slot_counter = 0;
    rxBuf = (uint8_t *)ebox_malloc(maxSlotNum);
    
    uart->begin(250000,9,0,1,RxIt);
    uart->attach(this,&DMXReceiver::rx_event,RxIrq);
    uart->nvic(ENABLE,1,1);
    uart->interrupt(RxIrq,ENABLE);
}

uint8_t DMXReceiver::getValue(uint16_t ch)
{
    if(ch < 1 || ch > 512) return 0;
    return rxBuf[ch - 1];
}
bool DMXReceiver::is_online()
{
    if(millis() - last_on_time > 5000)
        return false;
    return true;
}

void DMXReceiver::rx_event()
{
 static uint16_t data=0;

    data =uart->read();
    
    if((data & 0X0100) == 0)
    {
        slot_counter = 0;
        state = RECV_SC;
        last_on_time = millis();
    }  
    else
    {
        switch(state)
        {
            case WAIT_BREAK:
                break;
            case RECV_SC:
                if(0==(data & 0XFF)) 
                {
                    state = RECV_SLOTS;
                }
                else
                {
                    state = WAIT_BREAK; 
                }
                break;
            case RECV_SLOTS:
                if(slot_counter < maxSlotNum)
                {
                    rxBuf[slot_counter++] = data & 0xFF;
                }
                else
                    state = WAIT_BREAK; 

                break;
            default:
                break;
        }

    }
}



//void DMXMaster::begin(uint16_t maxSlotNum)
//{
//    this->maxSlotNum = maxSlotNum;
//    last_send_time = millis();
//    slot_counter = 0;
//    txBuf = (uint8_t *)ebox_malloc(maxSlotNum);
//    uart->begin(250000,8,0,2);
//}
//void DMXMaster::loop()
//{
//    if(millis() - last_send_time > 100)
//    {
//        uart->_tx_pin->mode(OUTPUT);
//        uart->_tx_pin->reset();
//        delay_us(88);
//        
//        uart->_tx_pin->mode(AF_PP);

//        uart->write(txBuf,maxSlotNum);
//    }
//}

