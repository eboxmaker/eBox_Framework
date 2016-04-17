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
#include "socket.h"
#include "udp.h"

u8 mac[6] = {0x00, 0x08, 0xdc, 0x11, 0x11, 0x11}; /*定义Mac变量*/
u8 lip[4] = {192, 168, 1, 119}; /*定义lp变量*/
u8 sub[4] = {255, 255, 255, 0}; /*定义subnet变量*/
u8 gw[4] = {192, 168, 1, 1}; /*定义gateway变量*/
u8 dns[4] = {192, 168, 1, 1}; /*定dns变量*/

u8 ip[6];
u8 buf[100];

W5500 w5500(&PC13, &PC14, &PC15, &spi2);

UDP udp1;
UDP udp2;

void setup()
{
    ebox_init();
    uart1.begin(115200);
    uart1.printf("\r\nuart1 9600 ok!");

    w5500.begin(2, mac, lip, sub, gw, dns);

    attach_eth_to_socket(&w5500);

    w5500.getMAC (ip);
    uart1.printf("\r\nmac : %02x.%02x.%02x.%02x.%02x.%02x\r\n", ip[0], ip[1], ip[2], ip[3], ip[4], ip[5]);
    w5500.getIP (ip);
    uart1.printf("IP : %d.%d.%d.%d\r\n", ip[0], ip[1], ip[2], ip[3]);
    w5500.getSubnet(ip);
    uart1.printf("mask : %d.%d.%d.%d\r\n", ip[0], ip[1], ip[2], ip[3]);
    w5500.getGateway(ip);
    uart1.printf("GW : %d.%d.%d.%d\r\n", ip[0], ip[1], ip[2], ip[3]);
    uart1.printf("Network is ready.\r\n");

    if(udp1.begin(0, 30000) == 0)
        uart1.printf("\r\nudp1 server creat ok! listen on 30000");
    if(udp2.begin(1, 30001) == 0)
        uart1.printf("\r\nudp2 server creat ok! listen on 30001");

}
u16 len;
int main(void)
{
    setup();

    while(1)
    {

        if(udp1.recv(buf))
        {
            uart1.printf("\r\n============================");
            uart1.printf("\r\n本地端口:%d", udp1.localPort );
            uart1.printf("\r\n消息来源:%d.%d.%d.%d:%d", udp1.remoteIP[0], udp1.remoteIP[1], udp1.remoteIP[2], udp1.remoteIP[3], udp1.remotePort);
            uart1.printf("\r\n数据内容:");
            uart1.printf((const char *)buf);
            udp1.sendto(udp1.remoteIP, udp1.remotePort, buf, 100);
        }
        if(udp2.recv(buf))
        {
            uart1.printf("\r\n============================");
            uart1.printf("\r\n本地端口:%d", udp2.localPort );
            uart1.printf("\r\n消息来源:%d.%d.%d.%d:%d", udp2.remoteIP[0], udp2.remoteIP[1], udp2.remoteIP[2], udp2.remoteIP[3], udp2.remotePort);
            uart1.printf("\r\n数据内容:");
            uart1.printf((const char *)buf);
            udp2.sendto(udp2.remoteIP, udp2.remotePort, buf, 100);
        }

    }


}




