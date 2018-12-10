/**
  ******************************************************************************
  * @file   : *.cpp
  * @author : shentq
  * @version: V1.2
  * @date   : 2016/08/14

  * @brief   ebox application example .
  *
  * Copyright 2016 shentq. All Rights Reserved.
  ******************************************************************************
 */


#include "ebox.h"
#include "enc28j60.h"
#include "ebox_printf.h"
#include "lwip_process.h"
struct udp_pcb *upcb;
ip_addr_t addr1;
struct pbuf *q = NULL;
const char *reply = "I'm here ! Are you There ? \n";


void udp_send()
{

    q = pbuf_alloc(PBUF_TRANSPORT, strlen(reply) + 1, PBUF_RAM);

    q->payload = (void *) reply;

    IP4_ADDR(&addr1, 192, 168, 1, 104);
    upcb = udp_new();
    // netconn_new();
    udp_bind(upcb, IP_ADDR_ANY, 1000);

    if(!q)
    {
        ebox_printf("out of PBUF_RAM\n");
    }
    else
    {
        ebox_printf("RAM IS OK\r\n");
    }

}


void setup()
{
    ebox_init();
    uart1.begin(115200);
    uart1.printf("being\r\n");
    lwip_init_app();
    udp_send();
}

uint32_t last_time = 0;
int main(void)
{
    setup();

    while(1)
    {

        if(millis() - last_time > 1000)
        {
            last_time = millis();
            udp_sendto(upcb, q, &addr1, 8080);
        }

        lwip_process();
    }
}
