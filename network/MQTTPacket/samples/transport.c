/*******************************************************************************
 * Copyright (c) 2014 IBM Corp.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Ian Craggs - initial API and implementation and/or initial documentation
 *    Sergio R. Caprile - "commonalization" from prior samples and/or documentation extension
 *******************************************************************************/


#if !defined(SOCKET_ERROR)
	/** error in socket operation */
	#define SOCKET_ERROR -1
#endif

#if defined(WIN32)
/* default on Windows is 64 - increase to make Linux and Windows the same */
#define FD_SETSIZE 1024
#include <winsock2.h>
#include <ws2tcpip.h>
#define MAXHOSTNAMELEN 256
#define EAGAIN WSAEWOULDBLOCK
#define EINTR WSAEINTR
#define EINVAL WSAEINVAL
#define EINPROGRESS WSAEINPROGRESS
#define EWOULDBLOCK WSAEWOULDBLOCK
#define ENOTCONN WSAENOTCONN
#define ECONNRESET WSAECONNRESET
#define ioctl ioctlsocket
#define socklen_t int
#else
#define INVALID_SOCKET SOCKET_ERROR
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#endif

#include "socket.h"
#include "transport.h"
/**
This simple low-level implementation assumes a single connection for a single thread. Thus, a static
variable is used for that connection.
On other scenarios, the user must solve this by taking into account that the current implementation of
MQTTPacket_read() has a function pointer for a function call to get the data to a buffer, but no provisions
to know the caller or other indicator (the socket id): int (*getfn)(unsigned char*, int)
*/
static int mysock = INVALID_SOCKET;

static uint8_t  my_socket = 7;
uint16_t local_port = 30000;
int transport_sendPacketBuffer(int sock, unsigned char* buf, int buflen)
{
	int rc = 0;
    my_socket = sock;
    rc = _send(sock,buf,buflen);
	return rc;
}


int transport_getdata(unsigned char* buf, int count)
{
    int rc = 0;
    while(rc == 0)
    {
         rc = _recv(my_socket,buf,count);
    }
	return rc;
}

int transport_getdatanb(void *sck, unsigned char* buf, int count)
{
    int rc;
     rc = _recv(my_socket,buf,count);
	return rc;
}

/**
return >=0 for a socket descriptor, <0 for an error code
@todo Basically moved from the sample without changes, should accomodate same usage for 'sock' for clarity,
removing indirections
*/
#define Sn_MR_TCP 0x01
int transport_open(char* addr, int port)
{
    int i = 200;
    int ret;
	while(--i)
	{
		switch(socket_status(my_socket))/*获取socket0的状态*/
		{
		case 0x13:
			 ret = _connect(my_socket, addr ,port);/*在TCP模式下向服务器发送连接请求*/ 
             break;
		 case 0x17:
             return ret;
		 case 0x00:
			ret = _socket(my_socket,Sn_MR_TCP,local_port,0x20);/*打开socket的一个端口*/
             break;
		}
        
	}
return ret;  
}

int transport_close(int sock)
{
    _close(my_socket);
	return 0;
}
