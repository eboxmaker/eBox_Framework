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

#include "w5500.h"
#include "socket.h"
#include "tcp.h"


/**
	*	1	此例程需要调用eDrive目录下的w5500驱动和network组件
	*	2	此例程演示了创建一个TCP的客户端，连接远程服务器，接收并返回数据
	*/



/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"w5500 udp example"
#define EXAMPLE_DATE	"2018-08-11"


u8 mac[6] = {0x00, 0x08, 0xdc, 0x11, 0x11, 0x11}; /*定义Mac变量*/
u8 ip[4] = {192, 168, 1, 119}; /*定义lp变量*/
u8 sub[4] = {255, 255, 255, 0}; /*定义subnet变量*/
u8 gw[4] = {192, 168, 1, 1}; /*定义gateway变量*/
u8 dns[4] = {192, 168, 1, 1}; /*定dns变量*/

u8 buf[100];

W5500 w5500(&PC13, &PC14, &PC15, &spi2);

TCPServer tcpServer;
uint16_t len;
u8 rip[4] = {192, 168, 1, 108}; /*定义lp变量*/

void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);
    w5500.begin(2, mac, ip, sub, gw, dns);

    attach_eth_to_socket(&w5500);

    w5500.getMAC (buf);
    UART.printf("\r\nmac : %02x.%02x.%02x.%02x.%02x.%02x\r\n", buf[0], buf[1], buf[2], buf[3], buf[4], buf[5]);
    w5500.getIP (buf);
    UART.printf("IP : %d.%d.%d.%d\r\n", buf[0], buf[1], buf[2], buf[3]);
    w5500.getSubnet(buf);
    UART.printf("mask : %d.%d.%d.%d\r\n", buf[0], buf[1], buf[2], buf[3]);
    w5500.getGateway(buf);
    UART.printf("GW : %d.%d.%d.%d\r\n", buf[0], buf[1], buf[2], buf[3]);
    UART.printf("Network is ready.\r\n");



    if(tcpServer.begin(SOCKET0, 3000) == 0)
        UART.printf("tcp server creat ok!listen on 3000\r\n");

}
int main(void)
{
    setup();

    while(1)
    {
        len = tcpServer.recv(buf);
        if(len > 0)
        {
            UART.printf("\r\n==================");
            UART.printf("\r\n客户端:%d.%d.%d.%d:%d", tcpServer.remoteIP[0], tcpServer.remoteIP[1], tcpServer.remoteIP[2], tcpServer.remoteIP[3], tcpServer.remotePort);
            UART.printf("\r\n数据长度：%d", len);
            UART.printf("\r\n数据内容：");
            UART.printf((const char *)buf);
            tcpServer.send(buf, len);
        }
    }


}




