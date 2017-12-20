#ifndef __HTTPD_H
#define __HTTPD_H
#ifdef __cplusplus
 extern "C" {
#endif
     
#include "stm32f10x.h"
#include "web_html.h"

#include "lwip/etharp.h" // etharp_tmr�������ڵ�ͷ�ļ�
#include "lwip/init.h" // lwip_init�������ڵ�ͷ�ļ�
#include "lwip/priv/tcp_priv.h" // tcp_tmr�������ڵ�ͷ�ļ�
#include "netif/ethernet.h" // ethernet_input��������ͷ�ļ�

#include <string.h>
#include <stdio.h>

static err_t http_accept(void *arg,struct tcp_pcb *pcb,err_t err);
static err_t http_recv(void *arg, struct tcp_pcb *pcb,struct pbuf *p,err_t err);

void httpd_init(void);

     
     
#ifdef __cplusplus
}
#endif
#endif

