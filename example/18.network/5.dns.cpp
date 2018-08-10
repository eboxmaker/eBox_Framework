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
	*	1	��������Ҫ����eDriveĿ¼�µ�w5500������network���
	*	2	��������ʾ��DNS��������
	*/



/* ���������������̷������� */
#define EXAMPLE_NAME	"w5500 dns example"
#define EXAMPLE_DATE	"2018-08-11"


u8 mac[6] = {0x00, 0x08, 0xdc, 0x11, 0x11, 0x11}; /*����Mac����*/
u8 ip[4] = {192, 168, 1, 119}; /*����lp����*/
u8 sub[4] = {255, 255, 255, 0}; /*����subnet����*/
u8 gw[4] = {192, 168, 1, 1}; /*����gateway����*/
u8 dns[4] = {192, 168, 1, 1}; /*��dns����*/

u8 buf[100];

W5500 w5500(&PC13, &PC14, &PC15, &spi2);

Dns ddns;
u8 name[] = "www.nciae.edu.cn";
uint16_t len;
u8 rip[4] = {192, 168, 1, 108}; /*����lp����*/
int ret;
void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME,EXAMPLE_DATE);
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

    ret = ddns.query((char*)name);
    if(ret == 1) /*����DNS����*/
        uart1.printf("Get [%s]'s IP address [%d.%d.%d.%d] from %d.%d.%d.%d\r\n", name, ddns.domain_ip[0], ddns.domain_ip[1], ddns.domain_ip[2], ddns.domain_ip[3], dns[0], dns[1], dns[2], dns[3]);
    else if(ret == -1)
        uart1.printf("δ֪����.\r\n");
    else if(ret == -2)
        uart1.printf("��ȡ��ʱ\r\n");

}
int main(void)
{
    setup();

    while(1)
    {

    }


}




