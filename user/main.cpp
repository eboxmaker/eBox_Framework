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
#include "udp.h"

int ee_check();
int w25x16_check();
int sd_check();
int wifi_check();
int w5500_check();

u8 mac[6] = {0x00, 0x08, 0xdc, 0x11, 0x11, 0x11}; /*定义Mac变量*/
u8 lip[4] = {192, 168, 1, 119}; /*定义lp变量*/
u8 sub[4] = {255, 255, 255, 0}; /*定义subnet变量*/
u8 gw[4] = {192, 168, 1, 1}; /*定义gateway变量*/
u8 dns[4] = {192, 168, 1, 1}; /*定dns变量*/


static FATFS fs;            // Work area (file system object) for logical drive
FRESULT res;
DWORD free_clust;//空簇，空扇区大小


LCD lcd(&PB5, &PB6, &PB4, &PB3, &spi1);
SD sd(&PB12, &spi2);
EEPROM ee(&i2c2);
W25X flash(&PA15, &spi1);
W5500 w5500(&PC13, &PC14, &PC15, &spi2);

uint8_t wbuf[512];
uint8_t rbuf[512];


void setup()
{
    int ret;
    ebox_init();
    uart1.begin(115200);
    uart1.printf("uart is ok!\r\n");
    lcd.begin(1);
    flash.begin(2);
    sd.begin(3);
    ee.begin(400000);
    wifi.begin(&PA4, &uart2, 115200);
    w5500.begin(2, mac, lip, sub, gw, dns);

    lcd.clear(BLACK);
    attach_sd_to_fat(&sd);
    

}
u32 count;
int main(void)
{
    int ret;
    setup();
    
    lcd.front_color = WHITE;
    uart1.printf("start check all dev\r\n");
    ret = ee_check();
    if(ret == 1)
    {
        uart1.printf("eeprom ...[ERR]\r\n");
        lcd.printf(0,0,"eeprom...[ERR]");
    }
    else
    {
        uart1.printf("eeprom check ......[OK]\r\n");
        lcd.printf(0,0,"eeprom...[OK]");
    }
    delay_ms(500);
    ret = w25x16_check();
    if(ret == 1)
    {
        uart1.printf("W25X16 ...[ERR]\r\n");
        lcd.printf(0,1*16,"W25X16...[ERR]");
    }
    else
    {
        uart1.printf("W25X16 check ......[OK]\r\n");
        lcd.printf(0,1*16,"W25X16...[OK]");
    }
    delay_ms(500);
    ret = sd_check();
    if(ret == 1)
    {
        uart1.printf("SDCARD ...[ERR]\r\n");
        lcd.printf(0,2*16,"SDCARD...[ERR]");
    }
    else
    {
        uart1.printf("SDCARD check ......[OK]\r\n");
        lcd.printf(0,2*16,"SDCARD...[OK]");
    }
    delay_ms(500);
        
    ret = wifi_check();
    if(ret == 1)
    {
        uart1.printf("WIFI ...[ERR]\r\n");
        lcd.printf(0,3*16,"WIFI.....[ERR]");
    }
    else
    {
        uart1.printf("WIFI check ......[OK]\r\n");
        lcd.printf(0,3*16,"WIFI.....[OK]");
    }
    delay_ms(500);
    
    ret = w5500_check();
    if(ret == 1)
    {
        uart1.printf("W5500 ...[ERR]\r\n");
        lcd.printf(0,4*16,"W5500....[ERR]");
    }
    else
    {
        uart1.printf("W5500 check ......[OK]\r\n");
        lcd.printf(0,4*16,"W5500....[OK]");
    }
    delay_ms(500);
    

    while(1)
    {

        delay_ms(1000);
    }

}

int ee_check()
{
    int ret;
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
    int ret;
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
    if(res == FR_OK)
        return 0;
    else
        return 1;
}
int wifi_check()
{
    int ret;
    ret = wifi.kick();
    if(ret)
        return 0;
    else
        return 1;
}

int w5500_check()
{
    uint8_t buf[6];
    w5500.getMAC (buf);
    for(int i = 0; i < 6; i++)
    {
        if(buf[i] != mac[i])
            return 1;
    }
    
    return 0;


}
