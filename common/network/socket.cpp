/**
  ******************************************************************************
  * @file    socket.cpp
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


/* Includes ------------------------------------------------------------------*/
#include "socket.h"
#include "util.h"
#include "string.h"
#include "w5500.h"
static uint16_t local_port;

W5500 *eth;


void attach_eth_to_socket(void *e)
{
    eth = (W5500*)e;
}
/**
@brief   This Socket function initialize the channel in perticular mode, and set the port and wait for W5200 done it.
@return  1 for sucess else -1:hard error,-2:protocol error.
*/
int _socket(SOCKET s, int8_t protocol, uint16_t port, int8_t flag)
{
    //   int ret = -3;
    //	 uint16_t i;
    if ( ((protocol & 0x0F) == Sn_MR_TCP) || ((protocol & 0x0F) == Sn_MR_UDP) || ((protocol & 0x0F) == Sn_MR_IPRAW) || \
            ((protocol & 0x0F) == Sn_MR_MACRAW) || (protocol & 0x0F) == Sn_MR_PPPOE)
    {
        _close(s);
        eth->write(Sn_MR(s) , protocol | flag);
        if (port != 0)
        {
            eth->write( Sn_PORT0(s) , (int8_t)((port & 0xff00) >> 8));
            eth->write( Sn_PORT1(s) , (int8_t)(port & 0x00ff));
        }
        else
        {
            local_port++; // if don't set the source port, set local_port number.
            eth->write(Sn_PORT0(s) , (int8_t)((local_port & 0xff00) >> 8));
            eth->write(Sn_PORT1(s) , (int8_t)(local_port & 0x00ff));
        }
        eth->write( Sn_CR(s) , Sn_CR_OPEN); // run sockinit Sn_CR

        /* wait to process the command... */
        while( eth->read(Sn_CR(s)) )
        {
            //            if(i++>0xfff0)
            //            {
            //                return -1;
            //            }
        }
        return  1;
        /* ------- */

    }
    else
    {
        return  -2;
    }
    //   return ret;
}

/**
@brief   This function close the socket and parameter is "s" which represent the socket number
*/
void _close(SOCKET s)
{


    eth->write( Sn_CR(s) , Sn_CR_CLOSE);

    /* wait to process the command... */
    while( eth->read(Sn_CR(s) ) )
        ;
    /* ------- */
    /* all clear */
    eth->write( Sn_IR(s) , 0xFF);
}
/**
@brief   This function established  the connection for the channel in passive (server) mode. This function waits for the request from the peer.
@return  1 for success else 0.
*/
bool _listen(SOCKET s)
{
    bool ret ;
    if (eth->read( Sn_SR(s) ) == SOCK_INIT)
    {
        eth->write( Sn_CR(s) , Sn_CR_LISTEN);
        /* wait to process the command... */
        while( eth->read(Sn_CR(s) ) )
            ;
        /* ------- */
        ret = true;
    }
    else
    {
        ret = false;
    }
    return ret;
}
/**
@brief   This function established  the connection for the channel in Active (client) mode.
      This function waits for the untill the connection is established.

@return  1 for success else -1:param error,-2:time out.
*/
int _connect(SOCKET s, uint8_t *addr, uint16_t port)
{
    int ret = -3;
    if(((addr[0] == 0xFF) && (addr[1] == 0xFF) && (addr[2] == 0xFF) && (addr[3] == 0xFF)) ||
            ((addr[0] == 0x00) && (addr[1] == 0x00) && (addr[2] == 0x00) && (addr[3] == 0x00)) ||
            (port == 0x00))
    {
        ret = -1;
    }
    else
    {
        // set destination IP
        eth->write( Sn_DIPR0(s), addr[0]);
        eth->write( Sn_DIPR1(s), addr[1]);
        eth->write( Sn_DIPR2(s), addr[2]);
        eth->write( Sn_DIPR3(s), addr[3]);
        eth->write( Sn_DPORT0(s), (int8_t)((port & 0xff00) >> 8));
        eth->write( Sn_DPORT1(s), (int8_t)(port & 0x00ff));
        eth->write( Sn_CR(s) , Sn_CR_CONNECT);
        /* wait for completion */
        while ( eth->read(Sn_CR(s) ) ) ;
        while(eth->read(Sn_SR(s)) != SOCK_ESTABLISHED)
        {
            if (eth->getSn_IR(s) & Sn_IR_TIMEOUT)
            {
                eth->write(Sn_IR(s), (Sn_IR_TIMEOUT));  // clear TIMEOUT Interrupt
                return -2;
            }
        }
        ret = 1;

        //        while ( eth->read(Sn_SR(s)) != SOCK_SYNSENT )
        //        {
        //            if(eth->read(Sn_SR(s)) == SOCK_ESTABLISHED)
        //            {
        //                ret =1;
        //                break ;
        //            }
        //            if (eth->getSn_IR(s) & Sn_IR_TIMEOUT)
        //            {
        //                eth->write(Sn_IR(s), (Sn_IR_TIMEOUT));  // clear TIMEOUT Interrupt
        //                ret = -2;
        //                break;
        //            }
        //        }
    }

    return ret;
}



/**
@brief   This function used for disconnect the socket and parameter is "s" which represent the socket number
@return  1 for success else 0.
*/
bool _disconnect(SOCKET s)
{
    eth->write( Sn_CR(s) , Sn_CR_DISCON);

    /* wait to process the command... */
    while( eth->read(Sn_CR(s)) )
        ;
    /* ------- */
    return true;
}
/**
@brief   This function used to send the data in TCP mode
@return   sended data size for success else 0.
*/
int _send(SOCKET s, const uint8_t *buf, uint16_t len)
{
    int8_t status = 0;
    int ret = 0;
    uint16_t freesize = 0;

    if (len > eth->getTxMAX(s))
        ret = 0; // check size not to exceed MAX size.
    else
        ret = len;

    // if freebuf is available, start.
    do
    {
        freesize = eth->get_tx_free_size(s);
        status = eth->read(Sn_SR(s));
        if ((status != SOCK_ESTABLISHED) && (status != SOCK_CLOSE_WAIT))
        {
            ret = 0;
            break;
        }
    }
    while (freesize < ret);


    // copy data
    eth->send_data_processing(s, (uint8_t *)buf, ret);
    eth->write( Sn_CR(s) , Sn_CR_SEND);

    /* wait to process the command... */
    while( eth->read(Sn_CR(s) ) );

    while ( (eth->read(Sn_IR(s) ) & Sn_IR_SEND_OK) != Sn_IR_SEND_OK )
    {
        status = eth->read(Sn_SR(s));
        if ((status != SOCK_ESTABLISHED) && (status != SOCK_CLOSE_WAIT) )
        {
            //      printf("SEND_OK Problem!!\r\n");
            _close(s);
            return 0;
        }
    }
    eth->write( Sn_IR(s) , Sn_IR_SEND_OK);

    //#ifdef __DEF_IINCHIP_INT__
    //   putISR(s, getISR(s) & (~Sn_IR_SEND_OK));
    //#else
    eth->write( Sn_IR(s) , Sn_IR_SEND_OK);
    //#endif

    return ret;
}
/**
@brief   This function is an application I/F function which is used to receive the data in TCP mode.
      It continues to wait for data as much as the application wants to receive.

@return  received data size for success else 0.
*/
int _recv(SOCKET s, uint8_t *buf, uint16_t len)
{
    uint16_t ret = 0;
    if ( len > 0 )
    {
        eth->recv_data_processing(s, buf, len);
        eth->write( Sn_CR(s) , Sn_CR_RECV);
        /* wait to process the command... */
        while( eth->read(Sn_CR(s) ));
        /* ------- */
        ret = len;
    }
    return ret;
}
/**
@brief   This function is an application I/F function which is used to send the data for other then TCP mode.
      Unlike TCP transmission, The peer's destination address and the port is needed.

@return  This function return send data size for success else 0.
*/
int _sendto(SOCKET s, const uint8_t *buf, uint16_t len, uint8_t *addr, uint16_t port)
{
    uint16_t ret = 0;

    if (len > eth->getTxMAX(s))
        ret = eth->getTxMAX(s); // check size not to exceed MAX size.
    else
        ret = len;

    if( ((addr[0] == 0x00) && (addr[1] == 0x00) && (addr[2] == 0x00) && (addr[3] == 0x00)) || ((port == 0x00)) )//||(ret == 0) )
    {
        /* added return value */
        ret = 0;
    }
    else
    {
        eth->write( Sn_DIPR0(s), addr[0]);
        eth->write( Sn_DIPR1(s), addr[1]);
        eth->write( Sn_DIPR2(s), addr[2]);
        eth->write( Sn_DIPR3(s), addr[3]);
        eth->write( Sn_DPORT0(s), (int8_t)((port & 0xff00) >> 8));
        eth->write( Sn_DPORT1(s), (int8_t)(port & 0x00ff));
        // copy data
        eth->send_data_processing(s, (uint8_t *)buf, ret);
        eth->write( Sn_CR(s) , Sn_CR_SEND);
        /* wait to process the command... */
        while( eth->read( Sn_CR(s) ) )
            ;
        /* ------- */

        while( (eth->read( Sn_IR(s) ) & Sn_IR_SEND_OK) != Sn_IR_SEND_OK )
        {
            if (eth->read( Sn_IR(s) ) & Sn_IR_TIMEOUT)
            {
                /* clear interrupt */
                eth->write( Sn_IR(s) , (Sn_IR_SEND_OK | Sn_IR_TIMEOUT)); /* clear SEND_OK & TIMEOUT */
                return 0;
            }
        }
        eth->write( Sn_IR(s) , Sn_IR_SEND_OK);
    }
    return ret;
}

/**
@brief   This function is an application I/F function which is used to receive the data in other then
   TCP mode. This function is used to receive UDP, IP_RAW and MAC_RAW mode, and handle the header as well.

@return  This function return received data size for success else -1.
*/
int _recvfrom(SOCKET s, uint8_t *buf, uint16_t len, uint8_t *addr, uint16_t *port)
{
    uint8_t head[8];
    uint16_t data_len = 0;
    uint16_t ptr = 0;
    uint32_t addrbsb = 0;
    if ( len > 0 )
    {
        ptr     = eth->read(Sn_RX_RD0(s) );
        ptr     = ((ptr & 0x00ff) << 8) + eth->read(Sn_RX_RD1(s));
        addrbsb = (uint32_t)(ptr << 8) +  (s << 5) + 0x18;

        switch (eth->read(Sn_MR(s) ) & 0x07)
        {
        case Sn_MR_UDP :
            eth->read(addrbsb, head, 0x08);
            ptr += 8;
            // read peer's IP address, port number.
            addr[0]  = head[0];
            addr[1]  = head[1];
            addr[2]  = head[2];
            addr[3]  = head[3];
            *port    = head[4];
            *port    = (*port << 8) + head[5];
            data_len = head[6];
            data_len = (data_len << 8) + head[7];

            addrbsb = (uint32_t)(ptr << 8) +  (s << 5) + 0x18;
            eth->read(addrbsb, buf, data_len);
            ptr += data_len;

            eth->write( Sn_RX_RD0(s), (int8_t)((ptr & 0xff00) >> 8));
            eth->write( Sn_RX_RD1(s), (int8_t)(ptr & 0x00ff));
            break;

        case Sn_MR_IPRAW :
            eth->read(addrbsb, head, 0x06);
            ptr += 6;
            addr[0]  = head[0];
            addr[1]  = head[1];
            addr[2]  = head[2];
            addr[3]  = head[3];
            data_len = head[4];
            data_len = (data_len << 8) + head[5];

            addrbsb  = (uint32_t)(ptr << 8) +  (s << 5) + 0x18;
            eth->read(addrbsb, buf, data_len);
            ptr += data_len;

            eth->write( Sn_RX_RD0(s), (int8_t)((ptr & 0xff00) >> 8));
            eth->write( Sn_RX_RD1(s), (int8_t)(ptr & 0x00ff));
            break;

        case Sn_MR_MACRAW :
            eth->read(addrbsb, head, 0x02);
            ptr += 2;
            data_len = head[0];
            data_len = (data_len << 8) + head[1] - 2;
            if(data_len > 1514)
            {
                //           printf("data_len over 1514\r\n");
                while(1);
            }

            addrbsb  = (uint32_t)(ptr << 8) +  (s << 5) + 0x18;
            eth->read(addrbsb, buf, data_len);
            ptr += data_len;

            eth->write( Sn_RX_RD0(s), (int8_t)((ptr & 0xff00) >> 8));
            eth->write( Sn_RX_RD1(s), (int8_t)(ptr & 0x00ff));
            break;

        default :
            break;
        }
        eth->write( Sn_CR(s) , Sn_CR_RECV);

        /* wait to process the command... */
        while( eth->read( Sn_CR(s)) ) ;
        /* ------- */
    }
    return data_len;
}
////////////////////////////////////////////////////////////////////////////////////////////
uint8_t socket_status(SOCKET s)
{
    return eth->getSn_SR(s);
}

/**

@brief   get socket RX recv buf size

@return  This gives size of received data in receive buffer.
*/
uint16_t recv_available(SOCKET s)
{
    return eth->get_rx_recv_size(s);
}
bool   get_remote_ip(SOCKET s, uint8_t *ip)
{
    bool ret;
    eth->getSn_DIPR(s, ip);
    if(ip != NULL)
        ret = true;
    else
        ret = false;
    return ret;
}
uint16_t get_remote_port(SOCKET s)
{
    return eth->getSn_DPORT(s);
}
bool client_connecte_event(SOCKET s)
{
    if(eth->getSn_IR(s) & Sn_IR_CON)
    {
        eth->setSn_IR(s, Sn_IR_CON);/*Sn_IRµÄµÚ0Î»ÖÃ1*/
        return true;
    }
    return false;
}
bool  get_local_dns(uint8_t *dns)
{
    dns[0] = eth->dns[0];
    dns[1] = eth->dns[1];
    dns[2] = eth->dns[2];
    dns[3] = eth->dns[3];
    return true;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
@brief	Convert 32bit Address(Host Ordering) into Dotted Decimal Format
@return 	a char pointer to a static buffer containing the text address in standard ".'' notation. Otherwise, it returns NULL.
*/
char *inet_ntoa(
    unsigned long addr	/**< Pointer variable to store converted value(INPUT) */
)
{
    static char addr_str[32];
    memset(addr_str, 0, 32);
    sprintf(addr_str, "%d.%d.%d.%d", (int)(addr >> 24 & 0xFF), (int)(addr >> 16 & 0xFF), (int)(addr >> 8 & 0xFF), (int)(addr & 0xFF));
    return addr_str;
}

char *inet_ntoa_pad(unsigned long addr)
{
    static char addr_str[16];
    memset(addr_str, 0, 16);
    //printf(addr_str,"%03d.%03d.%03d.%03d",(int)(addr>>24 & 0xFF),(int)(addr>>16 & 0xFF),(int)(addr>>8 & 0xFF),(int)(addr & 0xFF));
    return addr_str;
}


/*
void inet_addr_(unsigned char* addr,unsigned char *ip)
{
	int i;
//	uint32_t inetaddr = 0;
	char taddr[30];
	char * nexttok;
	char num;
	strcpy(taddr,(char *)addr);

	nexttok = taddr;
	for(i = 0; i < 4 ; i++)
	{
		nexttok = strtok(nexttok,".");
		if(nexttok[0] == '0' && nexttok[1] == 'x') num = ATOI(nexttok+2,0x10);
		else num = ATOI(nexttok,10);

		ip[i] = num;
		nexttok = NULL;
	}
}
*/
/**
@brief	Verify dotted notation IP address string
@return 	success - 1, fail - 0
*/
char VerifyIPAddress_orig(
    char *src	/**< pointer to IP address string */
)
{
    int i;
    int tnum;
    char tsrc[50];
    char *tok = tsrc;

    strcpy(tsrc, src);

    for(i = 0; i < 4; i++)
    {
        tok = strtok(tok, ".");
        if ( !tok ) return 0;
        if(tok[0] == '0' && tok[1] == 'x')
        {
            if(!ValidATOI(tok + 2, 0x10, &tnum)) return 0;
        }
        else if(!ValidATOI(tok, 10, &tnum)) return 0;

        if(tnum < 0 || tnum > 255) return 0;
        tok = NULL;
    }
    return 1;
}

char VerifyIPAddress(char *src, uint8_t *ip)
{
    int i;
    int tnum;
    char tsrc[50];
    char *tok = tsrc;

    strcpy(tsrc, src);

    for(i = 0; i < 4; i++)
    {
        tok = strtok(tok, ".");
        if ( !tok ) return 0;
        if(tok[0] == '0' && tok[1] == 'x')
        {
            if(!ValidATOI(tok + 2, 0x10, &tnum)) return 0;
        }
        else if(!ValidATOI(tok, 10, &tnum)) return 0;

        ip[i] = tnum;

        if(tnum < 0 || tnum > 255) return 0;
        tok = NULL;
    }
    return 1;
}

/**
@brief	Output destination IP address of appropriate channel
@return 	32bit destination address (Host Ordering)
*/
unsigned long GetDestAddr(
    SOCKET s	/**< Channel number which try to get destination IP Address */
)
{
    unsigned long addr = 0;
    int i = 0;
    for(i = 0; i < 4; i++)
    {
        addr <<= 8;
        addr += eth->read(Sn_DIPR0(s) + i);
    }
    return addr;
}

/**
@brief	Output destination port number of appropriate channel
@return 	16bit destination port number
*/
unsigned int GetDestPort(
    SOCKET s	/**< Channel number which try to get destination port */
)
{
    uint16_t port;
    port = ((uint16_t) eth->read(Sn_DPORT0(s))) & 0x00FF;
    port <<= 8;
    port += ((uint16_t) eth->read(Sn_DPORT0(s) + 1)) & 0x00FF;
    return port;
}


/**
@brief	htons function converts a unsigned short from host to TCP/IP network byte order (which is big-endian).
@return 	the value in TCP/IP network byte order
*/
uint16_t htons(
    uint16_t hostshort	/**< A 16-bit number in host byte order.  */
)
{
#if ( SYSTEM_ENDIAN == _ENDIAN_LITTLE_ )
    return swaps(hostshort);
#else
    return hostshort;
#endif
}


/**
@brief	htonl function converts a unsigned long from host to TCP/IP network byte order (which is big-endian).
@return 	the value in TCP/IP network byte order
*/
unsigned long htonl(
    unsigned long hostlong		/**< hostshort  - A 32-bit number in host byte order.  */
)
{
#if ( SYSTEM_ENDIAN == _ENDIAN_LITTLE_ )
    return swapl(hostlong);
#else
    return hostlong;
#endif
}


/**
@brief	ntohs function converts a unsigned short from TCP/IP network byte order to host byte order (which is little-endian on Intel processors).
@return 	a 16-bit number in host byte order
*/
unsigned long ntohs(
    unsigned short netshort	/**< netshort - network odering 16bit value */
)
{
#if ( SYSTEM_ENDIAN == _ENDIAN_LITTLE_ )
    return htons(netshort);
#else
    return netshort;
#endif
}


/**
@brief	converts a unsigned long from TCP/IP network byte order to host byte order (which is little-endian on Intel processors).
@return 	a 16-bit number in host byte order
*/
unsigned long ntohl(unsigned long netlong)
{
#if ( SYSTEM_ENDIAN == _ENDIAN_LITTLE_ )
    return htonl(netlong);
#else
    return netlong;
#endif
}

// destip : BigEndian
int8_t CheckDestInLocal(uint32_t destip)
{
    int i = 0;
    int8_t *pdestip = (int8_t *)&destip;
    for(i = 0; i < 4; i++)
    {
        if((pdestip[i] & eth->read(SUBR0 + i)) != (eth->read(SIPR0 + i) & eth->read(SUBR0 + i)))
            return 1;	// Remote
    }
    return 0;
}


/**
@brief	Get handle of socket which status is same to 'status'
@return 	socket number
*/
SOCKET getSocket(
    unsigned char status, 	/**< socket's status to be found */
    SOCKET start			/**< base of socket to be found */
)
{
    SOCKET i;
    if(start > 3) start = 0;

    for(i = start; i < MAX_SOCK_NUM ; i++) if( eth->getSn_SR(i) == status ) return i;
    return MAX_SOCK_NUM;
}


/**
@brief	Calculate checksum of a stream
@return 	checksum
*/
unsigned short checksum(
    unsigned char *src, 	/**< pointer to stream  */
    unsigned int len		/**< size of stream */
)
{
    uint16_t sum, tsum, i, j;
    uint32_t lsum;

    j = len >> 1;

    lsum = 0;

    for (i = 0; i < j; i++)
    {
        tsum = src[i * 2];
        tsum = tsum << 8;
        tsum += src[i * 2 + 1];
        lsum += tsum;
    }

    if (len % 2)
    {
        tsum = src[i * 2];
        lsum += (tsum << 8);
    }


    sum = lsum;
    sum = ~(sum + (lsum >> 16));
    return (uint16_t) sum;
}



#ifndef NO_USE_SOCKUTIL_FUNC
/**
@brief	Get Source IP Address of iinChip.
@return 	Source IP Address(32bit Address-Host Ordering)
*/
uint32_t GetIPAddress(void)
{
    uint32_t ip = 0;
    int i;
    for(i = 0; i < 4; i++)
    {
        ip <<= 8;
        ip += (char)eth->read(SIPR0 + i);
    }
    return ip;
}


/**
@brief	Get Gateway IP Address of iinChip.
@return 	Gateway IP Address(32bit Address-Host Ordering)
*/
uint32_t GetGWAddress(void)
{
    uint32_t ip = 0;
    int i;
    for(i = 0; i < 4; i++)
    {
        ip <<= 8;
        ip += (char)eth->read(GAR0 + i);
    }
    return ip;
}


/**
@brief	Get Subnet mask of iinChip.
@return 	Subnet Mask(32bit Address-Host Ordering)
*/
uint32_t GetSubMask(void)
{
    uint32_t ip = 0;
    int i;
    for(i = 0; i < 4; i++)
    {
        ip <<= 8;
        ip += (char)eth->read(SUBR0 + i);
    }
    return ip;
}


/**
@brief	Get Mac Address of iinChip.
@return 	Subnet Mask(32bit Address-Host Ordering)
*/
void GetMacAddress(
    char *mac	/**< Pointer to store Mac Address(48bit Address)(INPUT, OUTPUT) */
)
{
    int i = 0;
    for(i = 0; i < 6; i++)*mac++ = eth->read(SHAR0 + i);
}

void GetDestMacAddr(SOCKET s, int8_t *mac)
{
    int i = 0;
    for(i = 0; i < 6; i++)*mac++ = eth->read(Sn_DHAR0(s) + i);
}


/**
@brief	Read established network information(G/W, IP, S/N, Mac) of iinChip and Output that through Serial.
		Mac Address is output into format of Dotted HexaDecimal.Others are output into format of Dotted Decimal Format.
*/
void GetNetConfig(void)
{
    char addr[6];
    __IO uint32_t iaddr;
    //printf("\r\n================================================\r\n");
    //printf("       Net Config Information\r\n");
    //printf("================================================\r\n");
    GetMacAddress(addr);
    //printf("MAC ADDRESS      : 0x%02X.0x%02X.0x%02X.0x%02X.0x%02X.0x%02X\r\n",addr[0],addr[1],addr[2],addr[3],addr[4],addr[5]);

    iaddr = GetSubMask();
    //printf("SUBNET MASK      : %s\r\n",inet_ntoa(iaddr));

    iaddr = GetGWAddress();
    //printf("G/W IP ADDRESS   : %s\r\n",inet_ntoa(iaddr));

    iaddr = GetIPAddress();
    //printf("LOCAL IP ADDRESS : %s\r\n",inet_ntoa(iaddr));
    //printf("================================================\r\n");
}
#endif

