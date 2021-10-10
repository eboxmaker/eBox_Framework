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
#include "hx711.h"
#include "linear.h"
#include ".\OneButton\OneButton.h"

#define COLORED     0
#define UNCOLORED   1

RtcMillis LocalTime;
RFID rfid(&spi1,&PA4, &PA2); 
Balance bl(&PB8,&PB9);
OneButton btn(&PA8, false);

void click()
{
    uart1.printf("°´¼ü1click\n");
}
void doubleclick()
{
    uart1.printf("°´¼üË«»÷\n");
}
void setup()
{
    u8 ret;
    ebox_init();
    LocalTime.begin();
    LocalTime.dateTime.parse(String(__DATE__),String(__TIME__),8);
    UART.begin(115200);
    print_log();
    rfid.init();
    bl.begin();
    
    btn.begin();
    btn.attachClick(click);
    btn.attachDoubleClick(doubleclick);
}
int serNum0;
int serNum1;
int serNum2;
int serNum3;
int serNum4;
uint32_t last;
int main(void)
{
    setup();
    while(1)
    {
         /* This displays an image */

        /* Deep sleep */
//        LocalTime.update();
//        LocalTime.dateTime.print(UART);
        btn.loop();

        bl.loop();
        
        if(millis() - last > 1000)
        {
            last = millis();
//            uart1.printf("hx value:%0.1f\n",bl.get_raw());
            uart1.printf("hx weight:%0.1fg\n",bl.get_weight());
        }
        
        String str = uart1.readString();
        if(str == "1")
        {
            bl.add_row(50);
        }
        if(str == "2")
        {
            bl.add_row(500);
        }
        if(str == "0")
        {
            bl.calibration();
        }
    }


}

    
//        if (rfid.isCard()) {
//        if (rfid.readCardSerial()) {
//            if (rfid.serNum[0] != serNum0
//                || rfid.serNum[1] != serNum1
//                || rfid.serNum[2] != serNum2
//                || rfid.serNum[3] != serNum3
//                || rfid.serNum[4] != serNum4
//            ) {
//                /* With a new cardnumber, show it. */
//                Serial.println(" ");
//                Serial.println("Card found");
//                serNum0 = rfid.serNum[0];
//                serNum1 = rfid.serNum[1];
//                serNum2 = rfid.serNum[2];
//                serNum3 = rfid.serNum[3];
//                serNum4 = rfid.serNum[4];

//                //Serial.println(" ");
//                Serial.println("Cardnumber:");
//                Serial.print("Dec: ");
//                Serial.print(rfid.serNum[0],DEC);
//                Serial.print(", ");
//                Serial.print(rfid.serNum[1],DEC);
//                Serial.print(", ");
//                Serial.print(rfid.serNum[2],DEC);
//                Serial.print(", ");
//                Serial.print(rfid.serNum[3],DEC);
//                Serial.print(", ");
//                Serial.print(rfid.serNum[4],DEC);
//                Serial.println(" ");

//                Serial.print("Hex: ");
//                Serial.print(rfid.serNum[0],HEX);
//                Serial.print(", ");
//                Serial.print(rfid.serNum[1],HEX);
//                Serial.print(", ");
//                Serial.print(rfid.serNum[2],HEX);
//                Serial.print(", ");
//                Serial.print(rfid.serNum[3],HEX);
//                Serial.print(", ");
//                Serial.print(rfid.serNum[4],HEX);
//                Serial.println(" ");
//             } else {
//               /* If we have the same ID, just write a dot. */
//               Serial.print(".");
//             }
//          }
//    }

//    rfid.halt();


