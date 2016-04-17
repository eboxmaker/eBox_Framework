/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/


#include "string.h"
#include "ebox.h"
#include "w5500.h"

#include "MQTTPacket.h"
#include "transport.h"
#include "stdlib.h"
#include "mqtt_api.h"

int toStop = 0;

MQTT mqtt;


u8 mac[6] = {0x00, 0x08, 0xdc, 0x11, 0x11, 0x11}; /*定义Mac变量*/
u8 ip[4] = {192, 168, 1, 198}; /*定义lp变量*/
u8 sub[4] = {255, 255, 255, 0}; /*定义subnet变量*/
u8 gw[4] = {192, 168, 1, 1}; /*定义gateway变量*/
u8 _dns[4] = {192, 168, 1, 1};


char host[] = "m2m.eclipse.org";
//char host[]="messagesight.demos.ibm.com";
u16 host_port = 1883;
W5500 w5500(&PC13, &PC14, &PC15, &spi2);

int ret;
u8 buf[6];
void setup()
{
    ebox_init();
    uart1.begin(9600);
    uart1.printf("uart is ok !\r\n");

    w5500.begin(2, mac, ip, sub, gw, _dns);
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

    mqtt.begin(1, 3000);
    mqtt.set_server_domain((char *)host, 1883);
    mqtt.set_user("shentqlf", "123");

}
int value = 0;
char string[10] = {0};
int main(void)
{
    setup();
    while(1)
    {
        sprintf(string, "value = %d", value);
        value++;
        value %= 1000000;
        mqtt.publish((char *)"planets/earth", string);
        mqtt.subscribe("planets/earth");
    }
}

