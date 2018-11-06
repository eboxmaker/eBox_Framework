

#ifndef __ETH_PORT_H
#define __ETH_PORT_H
#ifdef __cplusplus
extern "C" {
#endif
#include "ebox_type.h"

#include "lwip/etharp.h" // etharp_tmr�������ڵ�ͷ�ļ�
#include "lwip/init.h" // lwip_init�������ڵ�ͷ�ļ�
#include "lwip/priv/tcp_priv.h" // tcp_tmr�������ڵ�ͷ�ļ�
#include "netif/ethernet.h" // ethernet_input��������ͷ�ļ�


void    ENC28J60_Init(uint8_t *mac_addr);
void    ENC28J60_InitSend(uint16_t len);
void    ENC28J60_WriteBuf(uint8_t *p, uint16_t len);
void    ENC28J60_BeginSend();
uint16_t    ENC28J60_GetPacketLength();

void    ENC28J60_ReadBuf(uint8_t *p, uint16_t len);

void    ENC28J60_EndReceive(void);

err_t ethernetif_init(struct netif *netif);
void ethernetif_input(struct netif *netif);

#ifdef __cplusplus
}
#endif


#endif



