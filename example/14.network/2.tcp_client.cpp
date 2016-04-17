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
#include "tcp.h"

u8 mac[6] = {0x00, 0x08, 0xdc, 0x11, 0x11, 0x11}; /*定义Mac变量*/
u8 lip[4] = {192, 168, 1, 119}; /*定义lp变量*/
u8 sub[4] = {255, 255, 255, 0}; /*定义subnet变量*/
u8 gw[4] = {192, 168, 1, 1}; /*定义gateway变量*/
u8 dns[4] = {192, 168, 1, 1}; /*定dns变量*/

u8 rip[4] = {192, 168, 1, 108}; /*定义lp变量*/
u8 buf[1024];
u8 ip[6];
u16 len;


W5500 w5500(&PC13, &PC14, &PC15, &spi2);

TCPCLIENT tcp;



void setup()
{
    ebox_init();
    uart1.begin(115200);

    w5500.begin(2, mac, lip, sub, gw, dns);


    attach_eth_to_socket(&w5500);

    w5500.getMAC (ip);
    uart1.printf("mac : %02x.%02x.%02x.%02x.%02x.%02x\r\n", ip[0], ip[1], ip[2], ip[3], ip[4], ip[5]);
    w5500.getIP (ip);
    uart1.printf("IP : %d.%d.%d.%d\r\n", ip[0], ip[1], ip[2], ip[3]);
    w5500.getSubnet(ip);
    uart1.printf("mask : %d.%d.%d.%d\r\n", ip[0], ip[1], ip[2], ip[3]);
    w5500.getGateway(ip);
    uart1.printf("GW : %d.%d.%d.%d\r\n", ip[0], ip[1], ip[2], ip[3]);
    uart1.printf("Network is ready.\r\n");

    tcp.begin(SOCKET7, 3000);
    tcp.connect(rip, 8080);


}
int main(void)
{
    setup();

    while(1)
    {

        len = tcp.recv(buf);
        if(len > 0)
        {
            uart1.printf("\r\n============================");
            uart1.printf("\r\n本地端口:%d", tcp.localPort );
            uart1.printf("\r\n消息来源:%d.%d.%d.%d:%d", tcp.remoteIP[0], tcp.remoteIP[1], tcp.remoteIP[2], tcp.remoteIP[3], tcp.remotePort);
            uart1.printf("\r\n数据长度:%d", len);
            uart1.printf("\r\n数据内容:");
            uart1.printf((const char *)buf);
            tcp.send(buf, len);
        }






    }


}




