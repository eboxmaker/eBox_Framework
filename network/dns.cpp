#include "dns.h"
#include "string.h"

uint8_t DNS_GET_IP[4];
uint16_t MSG_ID = 0x1122;

uint8_t BUFPUB[2048];




/*
********************************************************************************
*              CONVERT A DOMAIN NAME TO THE HUMAN-READABLE FORM
*
* Description : This function converts a compressed domain name to the human-readable form
* Arguments   : msg        - is a pointer to the reply message
*               compressed - is a pointer to the domain name in reply message.
*               buf        - is a pointer to the buffer for the human-readable form name.
*               len        - is the MAX. size of buffer.
* Returns     : the length of compressed message
* Note        :
********************************************************************************
*/
int parse_name(uint8_t * msg, uint8_t * compressed, /*char * buf,*/ uint16_t len)
{
  uint16_t slen;		/* Length of current segment */
  uint8_t  * cp;
  int16_t  clen = 0;		/* Total length of compressed name */
  int16_t  indirect = 0;	/* Set if indirection encountered */
  int16_t  nseg = 0;		/* Total number of segments in name */
  int8_t   name[MAX_DNS_BUF_SIZE];
  int8_t   *buf;
  
  buf = name;
  
  cp = compressed;
  
  for (;;)
  {
    slen = *cp++;	/* Length of this segment */
    
    if (!indirect) clen++;
    
    if ((slen & 0xc0) == 0xc0)
    {
      if (!indirect)
        clen++;
      indirect = 1;
      /* Follow indirection */
      cp = &msg[((slen & 0x3f)<<8) + *cp];
      slen = *cp++;
    }
    
    if (slen == 0)	/* zero length == all done */
      break;
    
    len -= slen + 1;
    
    if (len <= 0) return -1;
    
    if (!indirect) clen += slen;
    
    while (slen-- != 0) *buf++ = (int8_t)*cp++;
    *buf++ = '.';
    nseg++;
  }
  
  if (nseg == 0)
  {
    /* Root name; represent as single dot */
    *buf++ = '.';
    len--;
  }
  
  *buf++ = '\0';
  len--;
  
  return clen;	/* Length of compressed message */
}



/*
********************************************************************************
*              PARSE QUESTION SECTION
*
* Description : This function parses the qeustion record of the reply message.
* Arguments   : msg - is a pointer to the reply message
*               cp  - is a pointer to the qeustion record.
* Returns     : a pointer the to next record.
* Note        :
********************************************************************************
*/
uint8_t * dns_question(uint8_t * msg, uint8_t * cp)
{
  int16_t len;
  //	int8_t  xdata name[MAX_DNS_BUF_SIZE];
  
  len = parse_name(msg, cp, /*name,*/ MAX_DNS_BUF_SIZE);
  
  if (len == -1) return 0;
  
  cp += len;
  cp += 2;		/* type */
  cp += 2;		/* class */
  
  return cp;
}


/*
********************************************************************************
*              PARSE ANSER SECTION
*
* Description : This function parses the answer record of the reply message.
* Arguments   : msg - is a pointer to the reply message
*               cp  - is a pointer to the answer record.
* Returns     : a pointer the to next record.
* Note        :
********************************************************************************
*/
uint8_t * dns_answer(uint8_t * msg, uint8_t * cp)
{
  int16_t len, type;
  //	int8_t  xdata name[MAX_DNS_BUF_SIZE];
  
  len = parse_name(msg, cp, /*name,*/ MAX_DNS_BUF_SIZE);
  
  if (len == -1) return 0;
  
  cp += len;
  type = ntohs(*((uint16_t*)&cp[0]));
  cp += 2;		/* type */
  cp += 2;		/* class */
  cp += 4;		/* ttl */
  cp += 2;		/* len */
  
  switch (type)
  {
    case TYPE_A:
      DNS_GET_IP[0] = *cp++;
      DNS_GET_IP[1] = *cp++;
      DNS_GET_IP[2] = *cp++;
      DNS_GET_IP[3] = *cp++;
      break;
    case TYPE_CNAME:
    case TYPE_MB:
    case TYPE_MG:
    case TYPE_MR:
    case TYPE_NS:
    case TYPE_PTR:
      /* These types all consist of a single domain name */
      /* convert it to ascii format */
      len = parse_name(msg, cp, /*name,*/ MAX_DNS_BUF_SIZE);
      if (len == -1) return 0;
      
      cp += len;
      break;
    case TYPE_HINFO:
      len = *cp++;
      cp += len;
      
      len = *cp++;
      cp += len;
      break;
    case TYPE_MX:
      cp += 2;
      /* Get domain name of exchanger */
      len = parse_name(msg, cp,/* name,*/ MAX_DNS_BUF_SIZE);
      if (len == -1) return 0;
      
      cp += len;
      break;
    case TYPE_SOA:
      /* Get domain name of name server */
      len = parse_name(msg, cp,/* name,*/ MAX_DNS_BUF_SIZE);
      if (len == -1) return 0;
      
      cp += len;
      
      /* Get domain name of responsible person */
      len = parse_name(msg, cp,/* name,*/ MAX_DNS_BUF_SIZE);
      if (len == -1) return 0;
      
      cp += len;
      
      cp += 4;
      cp += 4;
      cp += 4;
      cp += 4;
      cp += 4;
      break;
    case TYPE_TXT:
      /* Just stash */
      break;
    default:
    /* Ignore */
    break;
  }
  
  return cp;
}

/*
********************************************************************************
*              PARSE THE DNS REPLY
*
* Description : This function parses the reply message from DNS server.
* Arguments   : dhdr - is a pointer to the header for DNS message
*               buf  - is a pointer to the reply message.
*               len  - is the size of reply message.
* Returns     : None
* Note        :
********************************************************************************
*/
uint8_t parseMSG(struct dhdr * pdhdr, uint8_t * pbuf)
{
  uint16_t tmp;
  uint16_t i;
  uint8_t * msg;
  uint8_t * cp;
  
  msg = pbuf;
  memset(pdhdr, 0, sizeof(pdhdr));
  
  pdhdr->id = ntohs(*((uint16_t*)&msg[0]));
  tmp = ntohs(*((uint16_t*)&msg[2]));
  if (tmp & 0x8000) pdhdr->qr = 1;
  
  pdhdr->opcode = (tmp >> 11) & 0xf;
  
  if (tmp & 0x0400) pdhdr->aa = 1;
  if (tmp & 0x0200) pdhdr->tc = 1;
  if (tmp & 0x0100) pdhdr->rd = 1;
  if (tmp & 0x0080) pdhdr->ra = 1;
  
  pdhdr->rcode = tmp & 0xf;
  pdhdr->qdcount = ntohs(*((uint16_t*)&msg[4]));
  pdhdr->ancount = ntohs(*((uint16_t*)&msg[6]));
  pdhdr->nscount = ntohs(*((uint16_t*)&msg[8]));
  pdhdr->arcount = ntohs(*((uint16_t*)&msg[10]));
  
  /* Now parse the variable length sections */
  cp = &msg[12];
  
  /* Question section */
  for (i = 0; i < pdhdr->qdcount; i++)
  {
    cp = dns_question(msg, cp);
  }
  
  /* Answer section */
  for (i = 0; i < pdhdr->ancount; i++)
  {
    cp = dns_answer(msg, cp);
  }
  
  /* Name server (authority) section */
  for (i = 0; i < pdhdr->nscount; i++)
  {
    ;
  }
  
  /* Additional section */
  for (i = 0; i < pdhdr->arcount; i++)
  {
    ;
  }
  
  if(pdhdr->rcode == 0) return 1;		// No error
  else return 0;
}

/*
********************************************************************************
*              MAKE DNS QUERY MESSAGE
*
* Description : This function makes DNS query message.
* Arguments   : op   - Recursion desired
*               name - is a pointer to the domain name.
*               buf  - is a pointer to the buffer for DNS message.
*               len  - is the MAX. size of buffer.
* Returns     : the pointer to the DNS message.
* Note        :
********************************************************************************
*/
int dns_makequery(uint16_t op, uint8_t * name, uint8_t * buf, uint16_t len)
{
  uint8_t  *cp;
  uint8_t   *cp1;
  //	int8_t   sname[MAX_DNS_BUF_SIZE];
  uint8_t  *dname;
  uint16_t p;
  uint16_t dlen;
  
  cp = buf;
  
  MSG_ID++;
  *(uint16_t*)&cp[0] = htons(MSG_ID);
  p = (op << 11) | 0x0100;			/* Recursion desired */
  *(uint16_t*)&cp[2] = htons(p);
  *(uint16_t*)&cp[4] = htons(1);
  *(uint16_t*)&cp[6] = htons(0);
  *(uint16_t*)&cp[8] = htons(0);
  *(uint16_t*)&cp[10]= htons(0);
  
  cp += sizeof(uint16_t)*6;
  //	strcpy(sname, name);
  dname = name;
  dlen = strlen((char*)dname);
  for (;;)
  {
    /* Look for next dot */
    cp1 = (unsigned char*)strchr((char*)dname, '.');
    
    if (cp1) len = cp1 - dname;	/* More to come */
    else len = dlen;			/* Last component */
    
    *cp++ = len;				/* Write length of component */
    if (len == 0) break;
    
    /* Copy component up to (but not including) dot */
    strncpy((char *)cp, (char*)dname, len);
    cp += len;
    if (!cp1)
    {
      *cp++ = 0;			/* Last one; write null and finish */
      break;
    }
    dname += len+1;
    dlen -= len+1;
  }
  
  *(uint16_t*)&cp[0] = htons(0x0001);				/* type */
  *(uint16_t*)&cp[2] = htons(0x0001);				/* class */
  cp += sizeof(uint16_t)*2;
  
  return ((int)((uint32_t)(cp) - (uint32_t)(buf)));
}


/*
********************************************************************************
*              MAKE DNS QUERY AND PARSE THE REPLY
*
* Description : This function makes DNS query message and parses the reply from DNS server.
* Arguments   : name - is a pointer to the domain name.
* Returns     : if succeeds : 1, fails : -1
* Note        :
********************************************************************************
*/
uint8_t DNS::query(uint8_t * name)
{
  static uint32_t dns_wait_time = 0;
  struct dhdr dhp;
  uint8_t ip[4];
  uint16_t len, port;
  switch(eth->getSn_SR(s))
  {
    case SOCK_UDP:
      if ((len = eth->getSn_RX_RSR(s)) > 0)
      {
        if (len > MAX_DNS_BUF_SIZE) len = MAX_DNS_BUF_SIZE;
        len = _recvfrom(s, BUFPUB, len, ip, &port);
        if(parseMSG(&dhp, BUFPUB))
        {
          _close(s);
          return DNS_RET_SUCCESS;
        }
        else 
          dns_wait_time = DNS_RESPONSE_TIMEOUT;
      }
      else
      {
        delay_ms(1000);
        dns_wait_time++;
        //if(ConfigMsg.debug) printf("dns wait time=%d\r\n", dns_wait_time);
      }
      if(dns_wait_time >= DNS_RESPONSE_TIMEOUT)   // 3ÃÊ
      {
        _close(s);
        return DNS_RET_FAIL;
      }
      break;
    case SOCK_CLOSED:
      dns_wait_time = 0;
      _socket(s, Sn_MR_UDP, port, 0);
      len = dns_makequery(0, name, BUFPUB, MAX_DNS_BUF_SIZE);
      _sendto(s, BUFPUB, len, eth->dns, IPPORT_DOMAIN);
      _sendto(s, BUFPUB, len, eth->dns, IPPORT_DOMAIN);
      break;         
  }
  return DNS_RET_PROGRESS;
}


int DNS::begin(SOCKET p_s,uint16_t p_port)
{
	int ret = 0;
	s = p_s;
	port =p_port;
	if(eth->getSn_SR(s) != SOCK_CLOSED)
	{
		_close(s);
	}

	return ret;
}


