#ifndef __SOCKET_H
#define __SOCKET_H
#include "w5500.h"





extern W5500* eth;

void attach_eth_to_socket(W5500* e);
extern int  _socket(SOCKET s, int8_t protocol, uint16_t port, int8_t flag); // Opens a socket(TCP or UDP or IP_RAW mode)
extern void _close(SOCKET s); // Close socket
extern int  _connect(SOCKET s, uint8_t *addr, uint16_t port); // Establish TCP connection (Active connection)
extern void _disconnect(SOCKET s); // disconnect the connection
extern int  _listen(SOCKET s);	// Establish TCP connection (Passive connection)
extern uint16_t _send(SOCKET s, const uint8_t * buf, uint16_t len); // Send data (TCP)
extern uint16_t _recv(SOCKET s, uint8_t * buf, uint16_t len);	// Receive data (TCP)
extern uint16_t _sendto(SOCKET s, const uint8_t *buf, uint16_t len, uint8_t *addr, uint16_t port); // Send data (UDP/IP RAW)
extern uint16_t _recvfrom(SOCKET s, uint8_t * buf, uint16_t len, uint8_t * addr, uint16_t  *port); // Receive data (UDP/IP RAW)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//util

char*		inet_ntoa(unsigned long addr);
char*		inet_ntoa_pad(unsigned long addr);
void		inet_addr_(unsigned char* addr,unsigned char *ip);
char		VerifyIPAddress_orig(char* src	);
char		VerifyIPAddress(char* src, uint8_t * ip);
unsigned long		GetDestAddr(SOCKET s);
unsigned int		GetDestPort(SOCKET s);
uint16_t					htons(uint16_t hostshort);
unsigned long		htonl(unsigned long hostlong);
unsigned long		ntohs(unsigned short netshort);
unsigned long		ntohl(unsigned long netlong);
int8_t					CheckDestInLocal(uint32_t destip);
SOCKET					getSocket(	unsigned char status,	SOCKET start);
unsigned short	checksum(unsigned char * src,unsigned int len);
#ifndef NO_USE_SOCKUTIL_FUNC
uint32_t	GetIPAddress(void);
uint32_t	GetGWAddress(void);
uint32_t	GetSubMask(void);
void		GetMacAddress(char* mac);
void		GetDestMacAddr(SOCKET s, int8_t* mac);

void GetNetConfig(void);
#endif


#endif
