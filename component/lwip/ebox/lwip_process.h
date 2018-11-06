#ifndef __LWIP_PROCESS_H
#define __LWIP_PROCESS_H


#include "ebox.h"
#include "enc28j60.h"



#include "lwip/etharp.h" // etharp_tmr�������ڵ�ͷ�ļ�
#include "lwip/init.h" // lwip_init�������ڵ�ͷ�ļ�
#include "lwip/priv/tcp_priv.h" // tcp_tmr�������ڵ�ͷ�ļ�
#include "netif/ethernet.h" // ethernet_input��������ͷ�ļ�
#include "lwip/udp.h"

#include "eth_port.h"

extern Enc28j60 eth;
extern struct ip4_addr ipaddr, netmask, gw;
extern struct netif enc28j60;


void lwip_init_app();
void lwip_process();


#endif
