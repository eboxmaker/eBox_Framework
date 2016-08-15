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


#include "tcp.h"

TCPCLIENT mqtt_tcp;
int transport_init(int local_sock,int local_port)
{
    mqtt_tcp.begin(local_sock,local_port);
    return 1;
}
/*
@return  sended data size for success else 0.
*/
int transport_sendPacketBuffer(unsigned char* buf, int buflen)
{
	int rc = 0;
    rc = mqtt_tcp.send(buf,buflen);
	return rc;
}

/*
@return  received data size for success else 0.
非阻塞式接收，需在应用层做循环判断
*/
int transport_getdata(unsigned char* buf, int count)
{
	return mqtt_tcp.recv(buf,count);
}
///*
//@return  received data size for success else 0.
//阻塞式接收，超时返回0
//*/
//int transport_getdata(unsigned char* buf, int count)
//{
////阻塞式接收
//    int rc = 0;
//    uint32_t last_time = millis();
//    
//    while(rc == 0 ){
//        rc = mqtt_tcp.recv(buf,count);	
//        if(millis() - last_time > 1500)
//            return 0;
//	}
//	return rc;
//}

/*
@return  received data size for success else 0.
*/
int transport_getdatanb(void *sck, unsigned char* buf, int count)
{
    int rc;
    while(rc == 0)
    {
        rc = mqtt_tcp.recv(buf,count);
    }
	return rc;
}

/**
return >=0 for a socket descriptor, <0 for an error code
@todo Basically moved from the sample without changes, should accomodate same usage for 'sock' for clarity,
removing indirections
@return  1 for success else 0:time out.
*/
int transport_open(char* addr, int port)
{    
	return mqtt_tcp.connect((unsigned char *)addr,port);   
}
/*
@return  1 for success else 0:time out.
*/

int transport_close(int sock)
{
    mqtt_tcp.stop();
	return 1;
}
int transport_connnected()
{
    return mqtt_tcp.connected();

}
