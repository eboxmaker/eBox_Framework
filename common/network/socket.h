/**
  ******************************************************************************
  * @file    socket.h
  * @author  shentq
  * @version V1.2
  * @date    2016/08/14
  * @brief   
  ******************************************************************************
  * @attention
  *
  * No part of this software may be used for any commercial activities by any form 
  * or means, without the prior written consent of shentq. This specification is 
  * preliminary and is subject to change at any time without notice. shentq assumes
  * no responsibility for any errors contained herein.
  * <h2><center>&copy; Copyright 2015 shentq. All Rights Reserved.</center></h2>
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __SOCKET_H
#define __SOCKET_H
#include "stm32f10x.h"
#include "stdbool.h"
#include "netconfig.h"
#include "w5500.h"

#ifdef __cplusplus
extern "C" {
#endif
typedef int8_t 			SOCKET;
//如果socket想要被c文件调用可以将此函数声明去掉
void attach_eth_to_socket(void *e);

extern int  _socket(SOCKET s, int8_t protocol, uint16_t port, int8_t flag); // Opens a socket(TCP or UDP or IP_RAW mode)
extern void _close(SOCKET s); // Close socket
extern int  _connect(SOCKET s, uint8_t *addr, uint16_t port); // Establish TCP connection (Active connection)
extern bool _disconnect(SOCKET s); // disconnect the connection
extern bool _listen(SOCKET s);	// Establish TCP connection (Passive connection)
extern int _send(SOCKET s, const uint8_t *buf, uint16_t len);  // Send data (TCP)
extern int _recv(SOCKET s, uint8_t *buf, uint16_t len);	// Receive data (TCP)
extern int _sendto(SOCKET s, const uint8_t *buf, uint16_t len, uint8_t *addr, uint16_t port); // Send data (UDP/IP RAW)
extern int _recvfrom(SOCKET s, uint8_t *buf, uint16_t len, uint8_t *addr, uint16_t  *port);   // Receive data (UDP/IP RAW)


///////////////////////////////////////////////////////////////////////////////////
//------网卡socket状态表--------
/* Sn_SR values */
#define SOCK_CLOSED                  0x00     /**< closed */
#define SOCK_INIT                    0x13     /**< init state */
#define SOCK_LISTEN                  0x14     /**< listen state */
#define SOCK_SYNSENT                 0x15     /**< connection state */
#define SOCK_SYNRECV                 0x16     /**< connection state */
#define SOCK_ESTABLISHED             0x17     /**< success to connect */
#define SOCK_FIN_WAIT                0x18     /**< closing state */
#define SOCK_CLOSING                 0x1A     /**< closing state */
#define SOCK_TIME_WAIT               0x1B     /**< closing state */
#define SOCK_CLOSE_WAIT              0x1C     /**< closing state */
#define SOCK_LAST_ACK                0x1D     /**< closing state */
#define SOCK_UDP                     0x22     /**< udp socket */
#define SOCK_IPRAW                   0x32     /**< ip raw mode socket */
#define SOCK_MACRAW                  0x42     /**< mac raw mode socket */
#define SOCK_PPPOE                   0x5F     /**< pppoe socket */

/* 网络协议类型 */
#define Sn_MR_CLOSE                  0x00     /**< unused socket */
#define Sn_MR_TCP                    0x01     /**< TCP */
#define Sn_MR_UDP                    0x02     /**< UDP */
#define Sn_MR_IPRAW                  0x03      /**< IP LAYER RAW SOCK */
#define Sn_MR_MACRAW                 0x04      /**< MAC LAYER RAW SOCK */
#define Sn_MR_PPPOE                  0x05     /**< PPPoE */
#define Sn_MR_UCASTB                 0x10     /**< Unicast Block in UDP Multicating*/
#define Sn_MR_ND                     0x20     /**< No Delayed Ack(TCP) flag */
#define Sn_MR_MC                     0x20     /**< Multicast IGMP (UDP) flag */
#define Sn_MR_BCASTB                 0x40     /**< Broadcast blcok in UDP Multicating */
#define Sn_MR_MULTI                  0x80     /**< support UDP Multicating */


//网卡的功能
extern uint8_t  socket_status(SOCKET s);
extern uint16_t  recv_available(SOCKET s);

extern bool     get_remote_ip(SOCKET s, uint8_t *ip);
extern uint16_t get_remote_port(SOCKET s);
extern bool     client_connecte_event(SOCKET s);

extern bool     get_local_dns(uint8_t *dns);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//util

char		*inet_ntoa(unsigned long addr);
char		*inet_ntoa_pad(unsigned long addr);
void		inet_addr_(unsigned char *addr, unsigned char *ip);
char		VerifyIPAddress_orig(char *src	);
char		VerifyIPAddress(char *src, uint8_t *ip);
unsigned long		GetDestAddr(SOCKET s);
unsigned int		GetDestPort(SOCKET s);
uint16_t					htons(uint16_t hostshort);
unsigned long		htonl(unsigned long hostlong);
unsigned long		ntohs(unsigned short netshort);
unsigned long		ntohl(unsigned long netlong);
int8_t					CheckDestInLocal(uint32_t destip);
SOCKET					getSocket(	unsigned char status,	SOCKET start);
unsigned short	checksum(unsigned char *src, unsigned int len);
#ifndef NO_USE_SOCKUTIL_FUNC
uint32_t	GetIPAddress(void);
uint32_t	GetGWAddress(void);
uint32_t	GetSubMask(void);
void		GetMacAddress(char *mac);
void		GetDestMacAddr(SOCKET s, int8_t *mac);

void GetNetConfig(void);
#endif

#ifdef __cplusplus
}
#endif
#endif
