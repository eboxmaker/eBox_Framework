/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox
#include "ebox.h"
#include "bsp_ebox.h"
#include "rtcmillis.h"
#include "ebox_mem.h"
#include ".\RFID-RC522\RFID.h"


#define COLORED     0
#define UNCOLORED   1

RtcMillis LocalTime;
RFID rfid(&spi1,&PA4, &PA2); 

void setup()
{
    u8 ret;
    ebox_init();
    LocalTime.begin();
    LocalTime.dateTime.parse(String(__DATE__),String(__TIME__),8);
    UART.begin(115200);
    print_log();
    rfid.init();

    
}

    int serNum0;
    int serNum1;
    int serNum2;
    int serNum3;
    int serNum4;
int main(void)
{
    setup();
    while(1)
    {
         /* This displays an image */

        /* Deep sleep */
//        LocalTime.update();
//        LocalTime.dateTime.print(UART);
//        delay_ms(1000);
        
        
        
        if (rfid.isCard()) {
        if (rfid.readCardSerial()) {
            if (rfid.serNum[0] != serNum0
                && rfid.serNum[1] != serNum1
                && rfid.serNum[2] != serNum2
                && rfid.serNum[3] != serNum3
                && rfid.serNum[4] != serNum4
            ) {
                /* With a new cardnumber, show it. */
                Serial.println(" ");
                Serial.println("Card found");
                serNum0 = rfid.serNum[0];
                serNum1 = rfid.serNum[1];
                serNum2 = rfid.serNum[2];
                serNum3 = rfid.serNum[3];
                serNum4 = rfid.serNum[4];

                //Serial.println(" ");
                Serial.println("Cardnumber:");
                Serial.print("Dec: ");
                Serial.print(rfid.serNum[0],DEC);
                Serial.print(", ");
                Serial.print(rfid.serNum[1],DEC);
                Serial.print(", ");
                Serial.print(rfid.serNum[2],DEC);
                Serial.print(", ");
                Serial.print(rfid.serNum[3],DEC);
                Serial.print(", ");
                Serial.print(rfid.serNum[4],DEC);
                Serial.println(" ");

                Serial.print("Hex: ");
                Serial.print(rfid.serNum[0],HEX);
                Serial.print(", ");
                Serial.print(rfid.serNum[1],HEX);
                Serial.print(", ");
                Serial.print(rfid.serNum[2],HEX);
                Serial.print(", ");
                Serial.print(rfid.serNum[3],HEX);
                Serial.print(", ");
                Serial.print(rfid.serNum[4],HEX);
                Serial.println(" ");
             } else {
               /* If we have the same ID, just write a dot. */
               Serial.print(".");
             }
          }
    }

    rfid.halt();
    }


}




