/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox
#include "ebox.h"
#include "mmc_sd.h"
#include "wrapperdiskio.h"
#include "ff.h"
#include "lcd_1.8.h"
#include "at24c02.h"
#include "w25x16.h"
#include "ESP8266.h"
#include "w5500.h"


int ee_check();
int w25x16_check();
int sd_check();
int wifi_check();


static FATFS fs;            // Work area (file system object) for logical drive
FRESULT res;
DWORD free_clust;//ø’¥ÿ£¨ø’…»«¯¥Û–°


LCD lcd(&PB5, &PB6, &PB4, &PB3, &spi1);
SD sd(&PB12, &spi2);
EEPROM ee(&i2c2);
W25X flash(&PA15, &spi1);
W5500 w5500(&PC13, &PC14, &PC15, &spi2);

int ret;
uint8_t wbuf[512];
uint8_t rbuf[512];


void setup()
{
    ebox_init();
    uart1.begin(9600);
    lcd.begin(1);
    flash.begin(2);
    sd.begin(3);
    ee.begin(400000);
    wifi.begin(&PA4, &uart2, 115200);
    
    lcd.clear(BLACK);
    attach_sd_to_fat(&sd);
    attach_eth_to_socket(&w5500);

}
u32 count;
int main(void)
{
    setup();
    
    ret = ee_check();
    if(ret == 1)
        uart1.printf("eeprom check ......[err]\r\n");
    else
        uart1.printf("eeprom check ......[OK]\r\n");
    delay_ms(500);
    ret = w25x16_check();
    if(ret == 1)
        uart1.printf("w25x16 check ......[err]\r\n");
    else
        uart1.printf("w25x16 check ......[OK]\r\n");
    delay_ms(500);
    ret = sd_check();
    if(ret == 1)
        uart1.printf("sdcard check ......[err]\r\n");
    else
        uart1.printf("sdcard check ......[OK]\r\n");
    delay_ms(500);
        
    ret = wifi_check();
    if(ret == 1)
        uart1.printf("wifi check ........[err]\r\n");
    else
        uart1.printf("wifi check ........[OK]\r\n");
    delay_ms(500);
    

    while(1)
    {

        delay_ms(1000);
    }

}

int ee_check()
{
    for(uint16_t i = 0; i < 256; i++)
    {
        wbuf[i] = random() % 256;
    }
    ee.write_byte(0, wbuf, 256);
    ee.read_byte(0, rbuf, 256);
    for(int i = 0; i < 256; i++)
    {
        if(wbuf[i] != rbuf[i])
        {
            ret = 1;
            break;
        }
    }

    return ret;
}

int w25x16_check()
{
    for(uint16_t i = 0; i < 256; i++)
    {
        wbuf[i] = random() % 256;
    }
    flash.write(wbuf, 0, 256);
    flash.read(rbuf, 0, 256);
    for(int i = 0; i < 256; i++)
    {
        if(wbuf[i] != rbuf[i])
        {
            ret = 1;
            break;
        }
    }
    return ret;

}
int sd_check()
{
    res = f_mount(&fs, "0:", 1);
    return res;
}
int wifi_check()
{
    ret = wifi.kick();
    if(ret)
        return 0;
    else
        return 1;
}

void w5500_check()
{


}
