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
#include "./epaper/epd2in13b.h"
#include "imagedata.h"
#include "./epaper/epdpaint.h"

#define COLORED     0
#define UNCOLORED   1

RtcMillis LocalTime;
Epd epd(&PA1,&PA2,&PA3,&PA4,&spi1);

void setup()
{
    u8 ret;
    ebox_init();
    LocalTime.begin();
    LocalTime.dateTime.parse(String(__DATE__),String(__TIME__),8);
    UART.begin(115200);
    print_log();

    if (epd.Init() != 0) {
        Serial.print("e-Paper init failed");
        return;
    }
      /* This clears the SRAM of the e-paper display */
    epd.ClearFrame();
    unsigned char image[1024];
    Paint paint(image, 128, 18);    //width should be the multiple of 8 
    
    paint.Clear(COLORED);
    paint.DrawStringAt(8, 2, "Hello world", &Font12, UNCOLORED);
    epd.SetPartialWindowRed(paint.GetImage(), 0, 50, paint.GetWidth(), paint.GetHeight());
    epd.DisplayFrame();
    UART.printf("第一个执行完成\n");
    paint.Clear(UNCOLORED);
    paint.DrawStringAt(8, 2, "e-Paper Demo", &Font12, COLORED);
    epd.SetPartialWindowBlack(paint.GetImage(), 0, 8, paint.GetWidth(), paint.GetHeight());

    paint.SetWidth(64);
    paint.SetHeight(64);

    paint.Clear(UNCOLORED);
    paint.DrawRectangle(0, 0, 40, 50, COLORED);
    paint.DrawLine(0, 0, 40, 50, COLORED);
    paint.DrawLine(40, 0, 0, 50, COLORED);
    epd.SetPartialWindowBlack(paint.GetImage(), 8, 72, paint.GetWidth(), paint.GetHeight());

    paint.Clear(UNCOLORED);
    paint.DrawCircle(16, 16, 15, COLORED);
    epd.SetPartialWindowBlack(paint.GetImage(), 64, 72, paint.GetWidth(), paint.GetHeight());

    paint.Clear(UNCOLORED);
    paint.DrawFilledRectangle(0, 0, 40, 50, COLORED);
    epd.SetPartialWindowRed(paint.GetImage(), 8, 144, paint.GetWidth(), paint.GetHeight());

    paint.Clear(UNCOLORED);
    paint.DrawFilledCircle(16, 16, 15, COLORED);
    epd.SetPartialWindowRed(paint.GetImage(), 64, 144, paint.GetWidth(), paint.GetHeight());

    /* This displays the data from the SRAM in e-Paper module */
    epd.DisplayFrame();
    UART.printf("第二个执行完成\n");
    /* This displays an image */
    

    /* Deep sleep */
    epd.Sleep();
}
int main(void)
{
    setup();
    while(1)
    {
         /* This displays an image */

        /* Deep sleep */
        LocalTime.update();
        LocalTime.dateTime.print(UART);
        delay_ms(1000);
    }


}




