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
#include "tingATCmd.h"

Ting ting;
uint8_t send_buf[10]={'1','2','3','4','5','6','7','8','9','0'};
uint8_t recv_buf[10];
uint8_t len;

char msg[10];
uint8_t msg_len;

uint8_t is_sender = 0;


tLoRaSettings LoRaSettings =
{
    433000000,        // RFFrequency
    20,               // Power
    6,                // SignalBw [0: 7.8kHz, 1: 10.4 kHz, 2: 15.6 kHz, 3: 20.8 kHz, 4: 31.2 kHz,
                      // 5: 41.6 kHz, 6: 62.5 kHz, 7: 125 kHz, 8: 250 kHz, 9: 500 kHz, other: Reserved]
    10,                // SpreadingFactor [6: 64, 7: 128, 8: 256, 9: 512, 10: 1024, 11: 2048, 12: 4096  chips]
    1,                // ErrorCoding [1: 4/5, 2: 4/6, 3: 4/7, 4: 4/8]
    true,             // CrcOn [0: OFF, 1: ON]
    false,            // ImplicitHeaderOn [0: OFF, 1: ON]
    true,                // RxSingleOn [0: Continuous, 1 Single]
    false,                // FreqHopOn [0: OFF, 1: ON]
    4,                // HopPeriod Hops every frequency hopping period symbols
    //100,              // TxPacketTimeout
    3000,              // RxPacketTimeout
    8,              // PayloadLength (used for implicit header mode)
    4,                  //PreambleLength(4-x)
};
void setup()
{
    ebox_init();
    PA2.mode(INPUT_PD);
    PA3.mode(INPUT_PD);
    PB2.mode(INPUT);
    PB3.mode(INPUT);
    PB4.mode(INPUT);
    PB5.mode(INPUT);
    PB6.mode(INPUT);
    PA4.mode(INPUT);
    PA5.mode(INPUT);
    PA6.mode(INPUT);
    PA7.mode(INPUT);
    uart1.begin(115200);
    ting.begin(&PA1,&PA8,&uart3,115200);
    uart1.printf("start\r\n");
    ting.config(&LoRaSettings);
    ting.set_addr(0xffff);
    ting.set_dest(0xffff);
    ting.rx();
    ting.pwm1(0,1600,1000);
    ting.pwm2(0,1600,200);

}
uint16_t addr1;
uint16_t addr;

int main(void)
{
    setup();
    while(1)
    {
        if(is_sender == 1)
        {
            if(ting.send(send_buf,10) == ERR_OK)
                uart1.printf("SEND OK\r\n");
        }
        else
        {
            len = ting.available();
            if(len >= 10)
            {
                ting.read(recv_buf,len);
                
                uart1.println("recv data:");
                uart1.write(recv_buf,len);
                uart1.println();

                ting.rssi(msg,&msg_len);
                uart1.write(msg,msg_len);
            }
        }

    }

}


