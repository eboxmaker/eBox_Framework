#ifndef __SOCKET_H
#define __SOCKET_H
#include "w5500.h"





extern W5500* eth;

void attachEthToSocket(W5500* e);
extern int _socket(SOCKET s, uint8 protocol, uint16 port, uint8 flag); // Opens a socket(TCP or UDP or IP_RAW mode)
extern void _close(SOCKET s); // Close socket
extern int _connect(SOCKET s, uint8 * addr, uint16 port); // Establish TCP connection (Active connection)
extern void _disconnect(SOCKET s); // disconnect the connection
extern uint8 _listen(SOCKET s);	// Establish TCP connection (Passive connection)
extern uint16 _send(SOCKET s, const uint8 * buf, uint16 len); // Send data (TCP)
extern uint16 _recv(SOCKET s, uint8 * buf, uint16 len);	// Receive data (TCP)
extern uint16 _sendto(SOCKET s, const uint8 * buf, uint16 len, uint8 * addr, uint16 port); // Send data (UDP/IP RAW)
extern uint16 _recvfrom(SOCKET s, uint8 * buf, uint16 len, uint8 * addr, uint16  *port); // Receive data (UDP/IP RAW)



#endif
