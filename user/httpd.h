#ifndef __HTTPD_H
#define __HTTPD_H
#ifdef __cplusplus
 extern "C" {
#endif
     
#include "stm32f10x.h"
#include "web_html.h"

#include "lwip/etharp.h" // etharp_tmr函数所在的头文件
#include "lwip/init.h" // lwip_init函数所在的头文件
#include "lwip/priv/tcp_priv.h" // tcp_tmr函数所在的头文件
#include "netif/ethernet.h" // ethernet_input函数所在头文件

#include <string.h>
#include <stdio.h>

static err_t http_accept(void *arg,struct tcp_pcb *pcb,err_t err);
static err_t http_recv(void *arg, struct tcp_pcb *pcb,struct pbuf *p,err_t err);

void httpd_init(void);

     
     
#ifdef __cplusplus
}
#endif
#endif

