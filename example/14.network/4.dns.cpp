/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox


#include "string.h"
#include "ebox.h"
#include "w5500.h"
#include "tcp.h"
#include "dns.h"

u8 mac[6] = {0x00, 0x08, 0xdc, 0x11, 0x11, 0x11}; /*定义Mac变量*/
u8 ip[4] = {192, 168, 1, 199}; /*定义lp变量*/
u8 sub[4] = {255, 255, 255, 0}; /*定义subnet变量*/
u8 gw[4] = {192, 168, 1, 1}; /*定义gateway变量*/
u8 dns[4] = {192, 168, 1, 1};


DNS ddns;

W5500 w5500(&PC13, &PC14, &PC15, &spi2);

int ret;
u8 name[] = "www.nciae.edu.cn";
u8 buf[6];

void setup()
{
    ebox_init();
    uart1.begin(115200);
    ret = sizeof(long);
    ret = sizeof(int);
    ret = sizeof(short int);
    ret = sizeof(short);

    w5500.begin(2, mac, ip, sub, gw, dns);


    attach_eth_to_socket(&w5500);

    w5500.getMAC (buf);
    uart1.printf("mac : %02x.%02x.%02x.%02x.%02x.%02x\r\n", buf[0], buf[1], buf[2], buf[3], buf[4], buf[5]);
    w5500.getIP (buf);
    uart1.printf("IP : %d.%d.%d.%d\r\n", buf[0], buf[1], buf[2], buf[3]);
    w5500.getSubnet(buf);
    uart1.printf("mask : %d.%d.%d.%d\r\n", buf[0], buf[1], buf[2], buf[3]);
    w5500.getGateway(buf);
    uart1.printf("GW : %d.%d.%d.%d\r\n", buf[0], buf[1], buf[2], buf[3]);
    uart1.printf("Network is ready.\r\n");

    ddns.begin(SOCKET1, 3000);

    ret = ddns.query((char*)name);
    if(ret == 1) /*发送DNS请求*/
        uart1.printf("Get [%s]'s IP address [%d.%d.%d.%d] from %d.%d.%d.%d\r\n", name, ddns.domain_ip[0], ddns.domain_ip[1], ddns.domain_ip[2], ddns.domain_ip[3], dns[0], dns[1], dns[2], dns[3]);
    else if(ret == -1)
        uart1.printf("未知错误.\r\n");
    else if(ret == -2)
        uart1.printf("获取超时\r\n");
}

int main(void)
{
    setup();
    while(1)
    {
    }
}




