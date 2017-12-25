/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox


#include "ebox.h"
#include "w5500.h"


u8 mac[6] = {0x00, 0x08, 0xdc, 0x11, 0x11, 0x11}; /*定义Mac变量*/
u8 lip[4] = {192, 168, 1, 119}; /*定义lp变量*/
u8 sub[4] = {255, 255, 255, 0}; /*定义subnet变量*/
u8 gw[4] = {192, 168, 1, 1}; /*定义gateway变量*/
u8 dns[4] = {192, 168, 1, 1}; /*定dns变量*/

u8 ip[6];
u8 buf[100]={'1'};

W5500 w5500(&PC13, &PC14, &PC15, &spi2);


void setup()
{
    ebox_init();
    uart1.begin(115200);
    uart1.printf("\r\nuart1 115200 ok!");

    w5500.begin(2, mac, lip, sub, gw, dns);


    w5500.getMAC (ip);
    uart1.printf("\r\nmac : %02x.%02x.%02x.%02x.%02x.%02x\r\n", ip[0], ip[1], ip[2], ip[3], ip[4], ip[5]);
    w5500.getIP (ip);
    uart1.printf("IP : %d.%d.%d.%d\r\n", ip[0], ip[1], ip[2], ip[3]);
    w5500.getSubnet(ip);
    uart1.printf("mask : %d.%d.%d.%d\r\n", ip[0], ip[1], ip[2], ip[3]);
    w5500.getGateway(ip);
    uart1.printf("GW : %d.%d.%d.%d\r\n", ip[0], ip[1], ip[2], ip[3]);
    uart1.printf("Network is ready.\r\n");



}
u16 len;
uint8_t remot_ip[4]={192,168,1,105};
int main(void)
{
    setup();

    while(1)
    {
        delay_ms(1000);
    }


}




