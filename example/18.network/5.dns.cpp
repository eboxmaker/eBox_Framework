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
#include "dns.h"


/**
	*	1	此例程需要调用eDrive目录下的w5500驱动和network组件
	*	2	此例程演示了DNS域名解析
	*/



/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"w5500 dns example"
#define EXAMPLE_DATE	"2018-08-11"


u8 mac[6] = {0x00, 0x08, 0xdc, 0x11, 0x11, 0x11}; /*定义Mac变量*/
u8 ip[4] = {192, 168, 1, 119}; /*定义lp变量*/
u8 sub[4] = {255, 255, 255, 0}; /*定义subnet变量*/
u8 gw[4] = {192, 168, 1, 1}; /*定义gateway变量*/
u8 dns[4] = {192, 168, 1, 1}; /*定dns变量*/

u8 buf[100];

W5500 w5500(&PC13, &PC14, &PC15, &spi2);

Dns ddns;
u8 name[] = "www.nciae.edu.cn";
uint16_t len;
u8 rip[4] = {192, 168, 1, 108}; /*定义lp变量*/
int ret;
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



    ddns.begin(SOCKET1, 3000);

    ret = ddns.query((char *)name);
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




