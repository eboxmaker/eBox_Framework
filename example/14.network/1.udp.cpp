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

u8 mac[6] = {0x00, 0x08, 0xdc, 0x11, 0x11, 0x11}; /*����Mac����*/
u8 lip[4] = {192, 168, 1, 119}; /*����lp����*/
u8 sub[4] = {255, 255, 255, 0}; /*����subnet����*/
u8 gw[4] = {192, 168, 1, 1}; /*����gateway����*/
u8 dns[4] = {192, 168, 1, 1}; /*��dns����*/

u8 ip[6];
u8 buf[100]={'1'};

W5500 w5500(&PC13, &PC14, &PC15, &spi2);

UDP udp1;

void setup()
{
    ebox_init();
    uart1.begin(115200);
    uart1.printf("\r\nuart1 115200 ok!");

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

    if(udp1.begin(0, 3000) == 0)
        uart1.printf("\r\nudp1 server creat ok! listen on 30000");


}
u16 len;
uint8_t remot_ip[4]={192,168,1,105};
int main(void)
{
    setup();

    while(1)
    {
        udp1.sendto(remot_ip,777,buf,1);

        if(udp1.recv(buf))
        {
            uart1.printf("\r\n============================");
            uart1.printf("\r\n���ض˿�:%d", udp1.localPort );
            uart1.printf("\r\n��Ϣ��Դ:%d.%d.%d.%d:%d", udp1.remoteIP[0], udp1.remoteIP[1], udp1.remoteIP[2], udp1.remoteIP[3], udp1.remotePort);
            uart1.printf("\r\n��������:");
            uart1.printf((const char *)buf);
            udp1.sendto(udp1.remoteIP, udp1.remotePort, buf, 100);
        }
        delay_ms(1000);
    }


}




