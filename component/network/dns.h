/**
  ******************************************************************************
  * @file    dns.h
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
#ifndef __UDP_H
#define __UDP_H
#include "socket.h"
#include "w5500.h"




#define  INITRTT     2000L	/* Initial smoothed response time */
#define  MAXCNAME    10	/* Maximum amount of cname recursion */

#define  TYPE_A		1	/* Host address */
#define  TYPE_NS		2	/* Name server */
#define  TYPE_MD		3	/* Mail destination (obsolete) */
#define  TYPE_MF		4	/* Mail forwarder (obsolete) */
#define  TYPE_CNAME	5	/* Canonical name */
#define  TYPE_SOA    6	/* Start of Authority */
#define  TYPE_MB		7	/* Mailbox name (experimental) */
#define  TYPE_MG		8	/* Mail group member (experimental) */
#define  TYPE_MR		9	/* Mail rename name (experimental) */
#define  TYPE_NULL	10	/* Null (experimental) */
#define  TYPE_WKS    11	/* Well-known sockets */
#define  TYPE_PTR    12	/* Pointer record */
#define  TYPE_HINFO	13	/* Host information */
#define  TYPE_MINFO	14	/* Mailbox information (experimental)*/
#define  TYPE_MX		15	/* Mail exchanger */
#define  TYPE_TXT    16	/* Text strings */
#define  TYPE_ANY    255/* Matches any type */

#define  CLASS_IN    1  /* The ARPA Internet */

/* Round trip timing parameters */
#define  AGAIN       8	/* Average RTT gain = 1/8 */
#define  LAGAIN      3	/* Log2(AGAIN) */
#define  DGAIN       4  /* Mean deviation gain = 1/4 */
#define  LDGAIN      2  /* log2(DGAIN) */

#define  IPPORT_DOMAIN	53

/* Header for all domain messages */
struct dhdr
{
    uint16_t  id;		/* Identification */
    int8_t 	qr;		/* Query/Response */
#define	QUERY		0
#define	RESPONSE	1
    int8_t 	opcode;
#define	IQUERY		1
    int8_t 	aa;		/* Authoratative answer */
    int8_t 	tc;		/* Truncation */
    int8_t 	rd;		/* Recursion desired */
    int8_t 	ra;		/* Recursion available */
    int8_t 	rcode;		/* Response code */
#define	NO_ERROR	0
#define	FORMAT_ERROR	1
#define	SERVER_FAIL	2
#define	NAME_ERROR	3
#define	NOT_IMPL	4
#define	REFUSED		5
    uint16_t  qdcount;	/* Question count */
    uint16_t  ancount;	/* Answer count */
    uint16_t  nscount;	/* Authority (name server) count */
    uint16_t  arcount;	/* Additional record count */
};

#define	MAX_DNS_BUF_SIZE	256		/* maximum size of DNS buffer. */

class DNS
{
public:
    uint8_t domain_ip[4];
    uint16_t msg_id;
public:
    int 	begin(SOCKET p_s, uint16_t p_port);
    int     query(char *name);
    bool    get_domain_ip(uint8_t *ip);
private:
    int 	parse_name(uint8_t *msg, uint8_t *compressed, uint16_t len);
    uint8_t *question(uint8_t *msg, uint8_t *cp);
    uint8_t *answer(uint8_t *msg, uint8_t *cp);
    uint8_t parseMSG(struct dhdr *pdhdr, uint8_t *pbuf);
    int 	makequery(uint16_t op, uint8_t *name, uint8_t *buf, uint16_t len);
    SOCKET s;
    uint16_t port;

};

#endif

