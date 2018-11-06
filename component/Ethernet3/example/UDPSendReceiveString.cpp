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

#include "../Ethernet3/utility/w5500.h"
#include "../Ethernet3/Ethernet3.h"
#include "../Ethernet3/EthernetUdp3.h"         // UDP library from: bjoern@cs.stanford.edu 12/30/2008

/**
	*	1	��������Ҫ����eDriveĿ¼�µ�w5500ģ��
	*	2	��������ʾ��w5500�ĳ�ʼ����������Ϣ��ӡ
	*/



/* ���������������̷������� */
#define EXAMPLE_NAME	"w5500 io test example"
#define EXAMPLE_DATE	"2018-08-11"

//W5500Class w5500(&PC13, &PC14, &PC15, &spi2);

//u8 mac[6] = {0x00, 0x08, 0xdc, 0x11, 0x11, 0x11}; /*����Mac����*/
//u8 ip[4] = {192, 168, 1, 119}; /*����lp����*/
//u8 sub[4] = {255, 255, 255, 0}; /*����subnet����*/
//u8 gw[4] = {192, 168, 1, 1}; /*����gateway����*/
//u8 dns[4] = {192, 168, 1, 1}; /*��dns����*/

u8 buf[100];

uint8_t mac[] =
{
    0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 1, 177);
unsigned int localPort = 8888;      // local port to listen on
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
uint8_t  ReplyBuffer[] = "acknowledged";       // a string to send back

// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;


void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);
    Ethernet.begin(mac, ip);
    Udp.begin(localPort);
}
int main(void)
{
    setup();

    while(1)
    {
        int packetSize = Udp.parsePacket();
        if (packetSize)
        {
            UART.print("Received packet of size ");
            UART.println(packetSize);
            UART.print("From ");
            IPAddress remote = Udp.remoteIP();
            for (int i = 0; i < 4; i++)
            {
                UART.print(remote[i], DEC);
                if (i < 3)
                {
                    UART.print(".");
                }
            }
            UART.print(", port ");
            UART.println(Udp.remotePort());

            // read the packet into packetBufffer
            Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
            UART.println("Contents:");
            UART.println(packetBuffer);

            // send a reply, to the IP address and port that sent us the packet we received
            Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
            Udp.write((const uint8_t *)ReplyBuffer, sizeof(ReplyBuffer));
            Udp.endPacket();
        }
        delay_ms(10);
        //        UART.print("Received packet of size ");
    }


}




