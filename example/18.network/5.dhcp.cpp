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
#include "dhcp.h"
#include "dns.h"
u8 mac[6] = {0x01,0x01,0x01,0x01,0x01,0x01};


u8 buf[100];

W5500 w5500(&PC13, &PC14, &PC15, &spi2);
void setup()
{
    int ret;
    ebox_init();
    uart1.begin(115200);
    uart1.printf("\r\nuart1 9600 ok!");

    w5500.begin(2, mac);
    attach_eth_to_socket(&w5500);

    ret = dhcp.begin(mac);
    if(ret == EOK)
    {
        w5500.setSHAR(dhcp.net.mac);/*≈‰÷√Macµÿ÷∑*/
        w5500.setSIPR(dhcp.net.ip);/*≈‰÷√Ipµÿ÷∑*/
        w5500.setSUBR(dhcp.net.subnet);/*≈‰÷√◊”Õ¯—⁄¬Î*/
        w5500.setGAR(dhcp.net.gw);/*≈‰÷√ƒ¨»œÕ¯πÿ*/
        w5500.getMAC (buf);
        uart1.printf("\r\nmac : %02x.%02x.%02x.%02x.%02x.%02x\r\n", buf[0], buf[1], buf[2], buf[3], buf[4], buf[5]);
        w5500.getIP (buf);
        uart1.printf("IP : %d.%d.%d.%d\r\n", buf[0], buf[1], buf[2], buf[3]);
        w5500.getSubnet(buf);
        uart1.printf("mask : %d.%d.%d.%d\r\n", buf[0], buf[1], buf[2], buf[3]);
        w5500.getGateway(buf);
        uart1.printf("GW : %d.%d.%d.%d\r\n", buf[0], buf[1], buf[2], buf[3]);
        uart1.printf("Network is ready.\r\n");
    }

}
u16 len;
int main(void)
{
  uint8_t dhcpret=0;
    setup();

    while(1)
    {

//        uart1.printf("\r\nret = %d",dhcpret);
    }

    
}




