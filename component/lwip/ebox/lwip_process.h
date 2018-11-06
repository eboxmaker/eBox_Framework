#ifndef __LWIP_PROCESS_H
#define __LWIP_PROCESS_H


#include "ebox.h"
#include "enc28j60.h"



#include "lwip/etharp.h" // etharp_tmr函数所在的头文件
#include "lwip/init.h" // lwip_init函数所在的头文件
#include "lwip/priv/tcp_priv.h" // tcp_tmr函数所在的头文件
#include "netif/ethernet.h" // ethernet_input函数所在头文件
#include "lwip/udp.h"

#include "eth_port.h"

extern Enc28j60 eth;
extern struct ip4_addr ipaddr, netmask, gw;
extern struct netif enc28j60;


void lwip_init_app();
void lwip_process();


#endif
