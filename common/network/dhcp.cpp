/**
  ******************************************************************************
  * @file    dhcp.cpp
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
#include "dhcp.h"
#include "stdio.h"
#include "string.h"

DHCP dhcp;

#define DHCP_DEBUG 1

#if DHCP_DEBUG
    #define DHCP_DBG(...) DBG(__VA_ARGS__)
#else
    #define  DHCP_DBG(...)
#endif



uint8_t host_name[]  = "iweb";

#define    DEVICE_ID      "w5500"
#define SOCK_DHCP             0
///////////////////////////////////////////////////////////////////////////////////////////////

#if !USE_HEAP
    uint8_t EXTERN_DHCPBUF[1024];
#endif

/*
*********************************************************************************************************
*              SEND DHCP DISCOVER
*
* Description: reset timeout value and retry count
* Arguments  :
* Returns    : 
* Note       : 
*********************************************************************************************************
*/
void DHCP::reset_DHCP_time(void)
{
	dhcp_time = 0;
	dhcp_tick_cnt = 0;
	next_dhcp_time = dhcp_time + DHCP_WAIT_TIME;
	dhcp_retry_count = 0;
}

/*
*********************************************************************************************************
*              SEND DHCP DISCOVER
*
* Description: This function sends DHCP DISCOVER message to DHCP server.
* Arguments  : s - is a socket number.
* Returns    : None.
* Note       : 
*********************************************************************************************************
*/
void DHCP::send_DHCP_DISCOVER(void)
{
  uint8_t ip[4] = {255,255,255,255};
  uint16_t i=0;
  //the host name modified
  uint8_t host_name[12];
  //*((uint32_t*)DHCP_SIP)=0;
  //*((uint32_t*)DHCP_REAL_SIP)=0;
  memset((void*)pRIPMSG,0,sizeof(RIP_MSG));
  
  pRIPMSG->op = DHCP_BOOTREQUEST;
  pRIPMSG->htype = DHCP_HTYPE10MB;
  pRIPMSG->hlen = DHCP_HLENETHERNET;
  pRIPMSG->hops = DHCP_HOPS;
  pRIPMSG->xid = htonl(DHCP_XID);
  pRIPMSG->secs = htons(DHCP_SECS);
  pRIPMSG->flags =htons(DHCP_FLAGSBROADCAST);
  pRIPMSG->chaddr[0] = net.mac[0];
  pRIPMSG->chaddr[1] = net.mac[1];
  pRIPMSG->chaddr[2] = net.mac[2];
  pRIPMSG->chaddr[3] = net.mac[3];
  pRIPMSG->chaddr[4] = net.mac[4];
  pRIPMSG->chaddr[5] = net.mac[5];    
  
  
  /* MAGIC_COOKIE */
  pRIPMSG->OPT[i++] = (uint8_t)((MAGIC_COOKIE >> 24)& 0xFF);
  pRIPMSG->OPT[i++] = (uint8_t)((MAGIC_COOKIE >> 16)& 0xFF);
  pRIPMSG->OPT[i++] = (uint8_t)((MAGIC_COOKIE >> 8)& 0xFF);
  pRIPMSG->OPT[i++] = (uint8_t)(MAGIC_COOKIE& 0xFF);
  
  /* Option Request Param. */
  pRIPMSG->OPT[i++] = dhcpMessageType;
  pRIPMSG->OPT[i++] = 0x01;
  pRIPMSG->OPT[i++] = DHCP_DISCOVER;
  
  // Client identifier
  pRIPMSG->OPT[i++] = dhcpClientIdentifier;
  pRIPMSG->OPT[i++] = 0x07;
  pRIPMSG->OPT[i++] = 0x01;
  pRIPMSG->OPT[i++] = net.mac[0];
  pRIPMSG->OPT[i++] = net.mac[1];
  pRIPMSG->OPT[i++] = net.mac[2];
  pRIPMSG->OPT[i++] = net.mac[3];
  pRIPMSG->OPT[i++] = net.mac[4];
  pRIPMSG->OPT[i++] = net.mac[5];
  
  // host name
  pRIPMSG->OPT[i++] = hostName;
  // set the host name
  sprintf((char*)host_name,"%.4s-%02X%02X%02X",DEVICE_ID,net.mac[3],net.mac[4],net.mac[5]);
  
  
  pRIPMSG->OPT[i++] = (uint8_t)strlen((char*)host_name);
  
  strcpy((char*)(&(pRIPMSG->OPT[i])),(char*)host_name);
 
  
  i+=(uint16_t)strlen((char*)host_name);	
 
  pRIPMSG->OPT[i++] = dhcpParamRequest;
  pRIPMSG->OPT[i++] = 0x06;
  pRIPMSG->OPT[i++] = subnetMask;
  pRIPMSG->OPT[i++] = routersOnSubnet;
  pRIPMSG->OPT[i++] = _dns;
  pRIPMSG->OPT[i++] = domainName;
  pRIPMSG->OPT[i++] = dhcpT1value;
  pRIPMSG->OPT[i++] = dhcpT2value;
  pRIPMSG->OPT[i++] = endOption;

  /* send broadcasting packet */
  //DHCP_DBG("send dhcp discover %s\r\n",EXTERN_DHCPBUF);
  //for(uint8_t i=0; i<3; i++)
  //Delay_ms(800);
  _sendto(SOCK_DHCP, (uint8_t *)pRIPMSG, sizeof(RIP_MSG), ip, DHCP_SERVER_PORT);	

	
	DHCP_DBG("send DHCP_DISCOVER\r\n");
}

/*
*********************************************************************************************************
*              SEND DHCP REQUEST
*
* Description: This function sends DHCP REQUEST message to DHCP server.
* Arguments  : s - is a socket number.
* Returns    : None.
* Note       : 
*********************************************************************************************************
*/
void DHCP::send_DHCP_REQUEST(void)
{
  uint8_t  ip[4];
  uint16_t i = 0;
  //uint16_t len=0;
  
  uint8_t  host_name[12];

  memset((void*)pRIPMSG,0,sizeof(RIP_MSG));
  
  pRIPMSG->op = DHCP_BOOTREQUEST;
  pRIPMSG->htype = DHCP_HTYPE10MB;
  pRIPMSG->hlen = DHCP_HLENETHERNET;
  pRIPMSG->hops = DHCP_HOPS;
  pRIPMSG->xid = htonl(DHCP_XID);
  pRIPMSG->secs = htons(DHCP_SECS);
  
  if(dhcp_state < STATE_DHCP_LEASED)
    pRIPMSG->flags = htons(DHCP_FLAGSBROADCAST);
  else
  {
    pRIPMSG->flags = 0;		// For Unicast
    memcpy(pRIPMSG->ciaddr,net.ip,4);
  }		

  memcpy(pRIPMSG->chaddr,net.mac,6);
  
  /* MAGIC_COOKIE */
  pRIPMSG->OPT[i++] = (uint8_t)((MAGIC_COOKIE >> 24) & 0xFF);
  pRIPMSG->OPT[i++] = (uint8_t)((MAGIC_COOKIE >> 16) & 0xFF);
  pRIPMSG->OPT[i++] = (uint8_t)((MAGIC_COOKIE >> 8) & 0xFF);
  pRIPMSG->OPT[i++] = (uint8_t)(MAGIC_COOKIE & 0xFF);
  
  /* Option Request Param. */
  pRIPMSG->OPT[i++] = dhcpMessageType;
  pRIPMSG->OPT[i++] = 0x01;
  pRIPMSG->OPT[i++] = DHCP_REQUEST;
  
  pRIPMSG->OPT[i++] = dhcpClientIdentifier;
  pRIPMSG->OPT[i++] = 0x07;
  pRIPMSG->OPT[i++] = 0x01;
  pRIPMSG->OPT[i++] = net.mac[0];
  pRIPMSG->OPT[i++] = net.mac[1];
  pRIPMSG->OPT[i++] = net.mac[2];
  pRIPMSG->OPT[i++] = net.mac[3];
  pRIPMSG->OPT[i++] = net.mac[4];
  pRIPMSG->OPT[i++] = net.mac[5];	
  
  
  if(dhcp_state < STATE_DHCP_LEASED)
  {
    pRIPMSG->OPT[i++] = dhcpRequestedIPaddr;
    pRIPMSG->OPT[i++] = 0x04;
    pRIPMSG->OPT[i++] = net.ip[0];
    pRIPMSG->OPT[i++] = net.ip[1];
    pRIPMSG->OPT[i++] = net.ip[2];
    pRIPMSG->OPT[i++] = net.ip[3];
  
    pRIPMSG->OPT[i++] = dhcpServerIdentifier;
    pRIPMSG->OPT[i++] = 0x04;
    pRIPMSG->OPT[i++] = DHCP_SIP[0];
    pRIPMSG->OPT[i++] = DHCP_SIP[1];
    pRIPMSG->OPT[i++] = DHCP_SIP[2];
    pRIPMSG->OPT[i++] = DHCP_SIP[3];
  }
  
  // host name
  pRIPMSG->OPT[i++] = hostName;
  //set the host name
  sprintf((char*)host_name,(char*)"%.4s-%02X%02X%02X",DEVICE_ID,net.mac[3],net.mac[4],net.mac[5]);
  pRIPMSG->OPT[i++] = (uint8_t)strlen((char*)host_name);

  strcpy((char*)&(pRIPMSG->OPT[i]),(char*)host_name);

  i+=strlen((char*)host_name);
	
  
  pRIPMSG->OPT[i++] = dhcpParamRequest;
  pRIPMSG->OPT[i++] = 0x08;
  pRIPMSG->OPT[i++] = subnetMask;
  pRIPMSG->OPT[i++] = routersOnSubnet;
  pRIPMSG->OPT[i++] = _dns;
  pRIPMSG->OPT[i++] = domainName;
  pRIPMSG->OPT[i++] = dhcpT1value;
  pRIPMSG->OPT[i++] = dhcpT2value;
  pRIPMSG->OPT[i++] = performRouterDiscovery;
  pRIPMSG->OPT[i++] = staticRoute;
  pRIPMSG->OPT[i++] = endOption;
  
  /* send broadcasting packet */
  if(dhcp_state < STATE_DHCP_LEASED) memset(ip,0xFF,4);
  else 
    memcpy(ip,DHCP_SIP,4);
  //DHCP_DBG("send dhcp request\r\n");
    _sendto(SOCK_DHCP, (const uint8_t *)pRIPMSG, sizeof(RIP_MSG), ip, DHCP_SERVER_PORT);
  //if(len==0)
    //DHCP_DBG("send request error\r\n");

	DHCP_DBG("sent DHCP_REQUEST\r\n");
}


/*
*********************************************************************************************************
*              SEND DHCP RELEASE
*
* Description: This function sends DHCP RELEASE message to DHCP server.
* Arguments  : s - is a socket number.
*              msgtype - 0 : RELEASE, Not Zero : DECLINE
* Returns    : None.
* Note       : 
*********************************************************************************************************
*/
void DHCP::send_DHCP_RELEASE_DECLINE(char msgtype)
{
  uint16_t i =0;
  uint8_t  ip[4];
  
  memset((void*)pRIPMSG,0,sizeof(RIP_MSG));
  
  pRIPMSG->op = DHCP_BOOTREQUEST;
  pRIPMSG->htype = DHCP_HTYPE10MB;
  pRIPMSG->hlen = DHCP_HLENETHERNET;
  pRIPMSG->hops = DHCP_HOPS;
  pRIPMSG->xid = htonl(DHCP_XID);
  pRIPMSG->secs = htons(DHCP_SECS);
  pRIPMSG->flags =0;// DHCP_FLAGSBROADCAST;
  
  memcpy(pRIPMSG->chaddr,net.mac,6);
  
  /* MAGIC_COOKIE */
  pRIPMSG->OPT[i++] = (uint8_t)((MAGIC_COOKIE >> 24) & 0xFF);
  pRIPMSG->OPT[i++] = (uint8_t)((MAGIC_COOKIE >> 16) & 0xFF);
  pRIPMSG->OPT[i++] = (uint8_t)((MAGIC_COOKIE >> 8) & 0xFF);
  pRIPMSG->OPT[i++] = (uint8_t)(MAGIC_COOKIE & 0xFF);
  
  /* Option Request Param. */
  pRIPMSG->OPT[i++] = dhcpMessageType;
  pRIPMSG->OPT[i++] = 0x01;
  pRIPMSG->OPT[i++] = ((!msgtype) ? DHCP_RELEASE : DHCP_DECLINE);
  
  pRIPMSG->OPT[i++] = dhcpClientIdentifier;
  pRIPMSG->OPT[i++] = 0x07;
  pRIPMSG->OPT[i++] = 0x01;
  pRIPMSG->OPT[i++] = net.mac[0];
  pRIPMSG->OPT[i++] = net.mac[1];
  pRIPMSG->OPT[i++] = net.mac[2];
  pRIPMSG->OPT[i++] = net.mac[3];
  pRIPMSG->OPT[i++] = net.mac[4];
  pRIPMSG->OPT[i++] = net.mac[5];	
  
  pRIPMSG->OPT[i++] = dhcpServerIdentifier;
  pRIPMSG->OPT[i++] = 0x04;
  pRIPMSG->OPT[i++] = DHCP_SIP[0];
  pRIPMSG->OPT[i++] = DHCP_SIP[1];
  pRIPMSG->OPT[i++] = DHCP_SIP[2];
  pRIPMSG->OPT[i++] = DHCP_SIP[3];
  
  if(msgtype)
  {
    pRIPMSG->OPT[i++] = dhcpRequestedIPaddr;
    pRIPMSG->OPT[i++] = 0x04;
    pRIPMSG->OPT[i++] = net.ip[0];
    pRIPMSG->OPT[i++] = net.ip[1];
    pRIPMSG->OPT[i++] = net.ip[2];
    pRIPMSG->OPT[i++] = net.ip[3];		
  		
    DHCP_DBG("sent DHCP_DECLINE\r\n");
  }
  	
  else
  {
    DHCP_DBG("sent DHCP_RELEASE\r\n");
  }
  
  pRIPMSG->OPT[i++] = endOption;	
  
  
  if(msgtype) memset(ip,0xFF,4);
  else 
    memcpy(ip,DHCP_SIP,4);
  //DHCP_DBG("send dhcp decline\r\n");
  _sendto(SOCK_DHCP, (uint8_t *)pRIPMSG, sizeof(RIP_MSG), ip, DHCP_SERVER_PORT);
}

/*
*********************************************************************************************************
*              PARSE REPLY MSG
*
* Description: This function parses the reply message from DHCP server.
* Arguments  : s      - is a socket number.
*              length - is a size data to receive.
* Returns    : success - return type, fail - 0
* Note       : 
*********************************************************************************************************
*/
uint8_t DHCP::parseDHCPMSG(uint16_t length)
{
  uint8_t  svr_addr[6];
  uint16_t svr_port;
  uint16_t len;
  uint8_t  * p;
  uint8_t  * e;
  uint8_t  type;
  uint8_t  opt_len = 0;
  
  len = _recvfrom(SOCK_DHCP, (uint8_t *)pRIPMSG, length, svr_addr, &svr_port);
  
  
    DHCP_DBG("DHCP_SIP:%u.%u.%u.%u\r\n",DHCP_SIP[0],DHCP_SIP[1],DHCP_SIP[2],DHCP_SIP[3]);
    DHCP_DBG("DHCP_RIP:%u.%u.%u.%u\r\n",DHCP_REAL_SIP[0],DHCP_REAL_SIP[1],DHCP_REAL_SIP[2],DHCP_REAL_SIP[3]);
    DHCP_DBG("svr_addr:%u.%u.%u.%u\r\n",svr_addr[0],svr_addr[1],svr_addr[2],svr_addr[3]);
  
  if(pRIPMSG->op != DHCP_BOOTREPLY || svr_port != DHCP_SERVER_PORT)
  {
    	   
      DHCP_DBG("DHCP : NO DHCP MSG\r\n");
    return 0;
  }
  if(memcmp(pRIPMSG->chaddr,net.mac,6) != 0 || pRIPMSG->xid != htonl(DHCP_XID))
  {
    
      DHCP_DBG("No My DHCP Message. This message is ignored.\r\n");
      DHCP_DBG("\tSRC_MAC_ADDR(%02X.%02X.%02X.",net.mac[0],net.mac[1],net.mac[2]);
      DHCP_DBG("%02X.%02X.%02X)",net.mac[3],net.mac[4],net.mac[5]);
      DHCP_DBG(", pRIPMSG->chaddr(%02X.%02X.%02X.",(char)pRIPMSG->chaddr[0],(char)pRIPMSG->chaddr[1],(char)pRIPMSG->chaddr[2]);
      DHCP_DBG("%02X.%02X.%02X)",pRIPMSG->chaddr[3],pRIPMSG->chaddr[4],pRIPMSG->chaddr[5]);
      DHCP_DBG("\tpRIPMSG->xid(%08X), DHCP_XID(%08X)",pRIPMSG->xid,(DHCP_XID));
      DHCP_DBG("\tpRIMPMSG->yiaddr:%d.%d.%d.%d\r\n",pRIPMSG->yiaddr[0],pRIPMSG->yiaddr[1],pRIPMSG->yiaddr[2],pRIPMSG->yiaddr[3]);
    return 0;
  }
  
  if( *((uint32_t*)DHCP_SIP) != 0x00000000 )
  {
    if( *((uint32_t*)DHCP_REAL_SIP) != *((uint32_t*)svr_addr) && 
            *((uint32_t*)DHCP_SIP) != *((uint32_t*)svr_addr) ) 
    {
		
            DHCP_DBG("Another DHCP sever send a response message. This is ignored.\r\n");
            DHCP_DBG("\tIP:%u.%u.%u.%u\r\n",svr_addr[0],svr_addr[1],svr_addr[2],svr_addr[3]);
      return 0;								
    }
  }
  
  memcpy(net.ip,pRIPMSG->yiaddr,4);
  
  	
    DHCP_DBG("DHCP MSG received\r\n");
    DHCP_DBG("yiaddr : %u.%u.%u.%u\r\n",net.ip[0],net.ip[1],net.ip[2],net.ip[3]);
  
  type = 0;
  p = (uint8_t *)(&pRIPMSG->op);
  p = p + 240;
  e = p + (len - 240);
  
   DHCP_DBG("p : %08X  e : %08X  len : %d\r\n", (uint32_t)p, (uint32_t)e, len);
  while ( p < e ) 
  {
    switch ( *p++ ) 
    {
    case endOption :
      return type;
              
      case padOption :
      break;
    case dhcpMessageType :
      opt_len = *p++;
      type = *p;			
      DHCP_DBG("dhcpMessageType : %02X\r\n", type);
      break;
    case subnetMask :
      opt_len =* p++;
      memcpy(net.subnet,p,4);

      DHCP_DBG("subnetMask : ");
      DHCP_DBG("%u.%u.%u.%u\r\n",net.subnet[0],net.subnet[1],net.subnet[2],net.subnet[3]);
      break;
    case routersOnSubnet :
      opt_len = *p++;
      memcpy(net.gw,p,4);

      DHCP_DBG("routersOnSubnet : ");
      DHCP_DBG("%u.%u.%u.%u\r\n",net.gw[0],net.gw[1],net.gw[2],net.gw[3]);
      break;
    case _dns :
      opt_len = *p++;
      memcpy(net.dns,p,4);
      break;
    case dhcpIPaddrLeaseTime :
      opt_len = *p++;
      dhcp_lease_time = ntohl(*((uint32_t*)p));
			
      DHCP_DBG("dhcpIPaddrLeaseTime : %d\r\n", dhcp_lease_time);
      break;
    case dhcpServerIdentifier :
      opt_len = *p++;
						
      DHCP_DBG("DHCP_SIP : %u.%u.%u.%u\r\n", DHCP_SIP[0], DHCP_SIP[1], DHCP_SIP[2], DHCP_SIP[3]);
      if( *((uint32_t*)DHCP_SIP) == 0 || 
          *((uint32_t*)DHCP_REAL_SIP) == *((uint32_t*)svr_addr) || 
          *((uint32_t*)DHCP_SIP) == *((uint32_t*)svr_addr) )
      {
        memcpy(DHCP_SIP,p,4);
        memcpy(DHCP_REAL_SIP,svr_addr,4);	// Copy the real ip address of my DHCP server
						
        DHCP_DBG("My dhcpServerIdentifier : ");
        DHCP_DBG("%u.%u.%u.%u\r\n", DHCP_SIP[0], DHCP_SIP[1], DHCP_SIP[2], DHCP_SIP[3]);
        DHCP_DBG("My DHCP server real IP address : ");
        DHCP_DBG("%u.%u.%u.%u\r\n", DHCP_REAL_SIP[0], DHCP_REAL_SIP[1], DHCP_REAL_SIP[2], DHCP_REAL_SIP[3]);
      }
      else
      {
			   
        DHCP_DBG("Another dhcpServerIdentifier : \r\n");
        DHCP_DBG("\tMY(%u.%u.%u.%u) ", DHCP_SIP[0], DHCP_SIP[1], DHCP_SIP[2], DHCP_SIP[3]);
        DHCP_DBG("Another(%u.%u.%u.%u)\r\n", svr_addr[0], svr_addr[1], svr_addr[2], svr_addr[3]);
      }
      break;
    default :
      opt_len = *p++;
			
      DHCP_DBG("opt_len : %u\r\n", opt_len);
      break;
    } // switch
    p+=opt_len;
  } // while
  
  return 0;
}

/*
*********************************************************************************************************
*              CHECK DHCP STATE
*
* Description: This function checks the state of DHCP.
* Arguments  : None.
* Returns    : None.
* Note       : 
*********************************************************************************************************
*/
uint8_t DHCP::check_DHCP_state(SOCKET s) 
{
  uint16_t len;
  uint8_t  type;
  
  type = 0;
  if(socket_status(s)!=SOCK_CLOSED)
  {
    if ((len = recv_available(s)) > 0)
    {
      type = parseDHCPMSG(len);
    }
  }
  else
  {
    if(dhcp_state == STATE_DHCP_READY)
    {
       //init_dhcp_client();
     DHCP_DBG("state : STATE_DHCP_READY\r\n");
    }
    
    
    if(!_socket(SOCK_DHCP,Sn_MR_UDP,DHCP_CLIENT_PORT,0x00))
    {
        DHCP_DBG("Fail to create the DHCPC_SOCK(%u)\r\n",SOCK_DHCP);
        return DHCP_RET_ERR;
    }
    
  }
  
  switch ( dhcp_state )
  {
    case STATE_DHCP_READY:
      DHCP_timeout = 0;
      reset_DHCP_time();
      send_DHCP_DISCOVER();
      
      DHCP_timer = 0;//set_timer0(DHCP_timer_handler);   	
      dhcp_state = STATE_DHCP_DISCOVER;
      break;	   
    case STATE_DHCP_DISCOVER :
      if (type == DHCP_OFFER) 
      {
        reset_DHCP_time();
        send_DHCP_REQUEST();
        dhcp_state = STATE_DHCP_REQUEST;
        DHCP_DBG("state : STATE_DHCP_REQUEST\r\n");
      }
      else 
        check_DHCP_Timeout();
      break;
    
    case STATE_DHCP_REQUEST :
      if (type == DHCP_ACK) 
      {
        reset_DHCP_time();
        if (check_leasedIP()) 
        {
          DHCP_DBG("state : STATE_DHCP_LEASED\r\n");
          dhcp_state = STATE_DHCP_LEASED;
          return DHCP_RET_UPDATE;
        } 
        else 
        {
          DHCP_DBG("state : STATE_DHCP_DISCOVER\r\n");
          dhcp_state = STATE_DHCP_DISCOVER;
          return DHCP_RET_CONFLICT;
        }
      }
      else if (type == DHCP_NAK) 
      {
        reset_DHCP_time();
        dhcp_state = STATE_DHCP_DISCOVER;
        DHCP_DBG("state : STATE_DHCP_DISCOVER\r\n");
      }
      else 
        check_DHCP_Timeout();
      break;
    
    case STATE_DHCP_LEASED :
      if ((dhcp_lease_time != 0xffffffff) && (dhcp_time>(dhcp_lease_time/2))) 
      {
        type = 0;
        memcpy(OLD_SIP,net.ip,4);
        DHCP_XID++;
        send_DHCP_REQUEST();
        dhcp_state = STATE_DHCP_REREQUEST;
        DHCP_DBG("state : STATE_DHCP_REREQUEST\r\n");
        reset_DHCP_time();
      }
      break;
    
    case STATE_DHCP_REREQUEST :
      if (type == DHCP_ACK) 
      {
        if(memcmp(OLD_SIP,net.ip,4)!=0)	
        {
          DHCP_DBG("The IP address from the DHCP server is updated.\r\n");
          DHCP_DBG("OLD_SIP=%u.%u.%u.%u",OLD_SIP[0],OLD_SIP[1],OLD_SIP[2],OLD_SIP[3]);
          DHCP_DBG("net.ip=%u.%u.%u.%u\r\n",net.ip[0],net.ip[1],net.ip[2],net.ip[3]);
          return DHCP_RET_UPDATE;
        }
        else
        {
          DHCP_DBG("state : STATE_DHCP_LEASED : same IP\r\n");
        }
        reset_DHCP_time();
        dhcp_state = STATE_DHCP_LEASED;
      } 
      else if (type == DHCP_NAK) 
      {
        reset_DHCP_time();
        dhcp_state = STATE_DHCP_DISCOVER;
        DHCP_DBG("state : STATE_DHCP_DISCOVER\r\n");
      } 
      else 
        check_DHCP_Timeout();
      break;
    default :
    dhcp_state = STATE_DHCP_READY;
    break;
  }
  
  if (DHCP_timeout == 1)
  {
    //kill_timer0(DHCP_timer);
    dhcp_state = STATE_DHCP_READY;
    return DHCP_RET_TIMEOUT;
  }
  return DHCP_RET_NONE;
}

/*
*********************************************************************************************************
*              CHECK TIMEOUT
*
* Description: This function checks the timeout of DHCP in each state.
* Arguments  : None.
* Returns    : None.
* Note       : 
*********************************************************************************************************
*/
void DHCP::check_DHCP_Timeout(void)
{
  if (dhcp_retry_count < MAX_DHCP_RETRY) 
  {
    if (dhcp_time > next_dhcp_time) 
    {
      dhcp_time = 0;
      next_dhcp_time = dhcp_time + DHCP_WAIT_TIME;
      dhcp_retry_count++;
      switch ( dhcp_state ) 
      {
      case STATE_DHCP_DISCOVER :
  			   
        DHCP_DBG("<<timeout>> state : STATE_DHCP_DISCOVER\r\n");
        send_DHCP_DISCOVER();
        break;
      case STATE_DHCP_REQUEST :
  			   
        DHCP_DBG("<<timeout>> state : STATE_DHCP_REQUEST\r\n");
        send_DHCP_REQUEST();
        break;

      case STATE_DHCP_REREQUEST :
  			   
        DHCP_DBG("<<timeout>> state : STATE_DHCP_REREQUEST\r\n");
        send_DHCP_REQUEST();
        break;
      default :
        break;
      }
    }
  } 
  else 
  {
    reset_DHCP_time();
    DHCP_timeout = 1;
    
    send_DHCP_DISCOVER();
    dhcp_state = STATE_DHCP_DISCOVER;
		
    DHCP_DBG("timeout : state : STATE_DHCP_DISCOVER\r\n");
  }
}


/*
*********************************************************************************************************
* Description: check if a leased IP is valid
* Arguments  : None.
* Returns    : None.
* Note       : 
*********************************************************************************************************
*/
uint8_t DHCP::check_leasedIP(void)
{
  
  
  DHCP_DBG("<Check the IP Conflict %d.%d.%d.%d: ",net.ip[0],net.ip[1],net.ip[2],net.ip[3]);
  // sendto is complete. that means there is a node which has a same IP.
  
  if ( _sendto(SOCK_DHCP, (const uint8_t*)"CHECK_IP_CONFLICT", 17, net.ip, 5000))
  {
  
    DHCP_DBG(" Conflict>\r\n");
    send_DHCP_RELEASE_DECLINE(1);
    return 0;
  }
  
    DHCP_DBG(" No Conflict>\r\n");
  return 1;

}	

/*
*********************************************************************************************************
*              
*
* Description: timer interrupt handler(For checking dhcp lease time).
* Arguments  : irq - interrupt offset
*              p   - pointer to parameter
* Returns    : None.
* Note       : Increase 'my_time' each one second.
*********************************************************************************************************
*/
void DHCP::DHCP_timer_handler(void)
{
  if(dhcp_tick_cnt++ > 1000)
  {
    dhcp_tick_cnt = 0;
    dhcp_time++;
  }
}

/*
*********************************************************************************************************
*              
*
* Description: Initialize the DHCP client
* Arguments  : s - Socket number for the DHCP client
               ip_update - handler called when the leased IP address is updated
               ip_conflict - handler called when the leased IP address is conflict
*              p   - pointer to parameter
* Returns    : None.
* Note       : 
*********************************************************************************************************
*/
int DHCP::param_init(void)
{
    DHCP_HOST_NAME = host_name;   // HOST NAME
    memset(OLD_SIP,0,sizeof(OLD_SIP));
    memset(DHCP_SIP,0,sizeof(DHCP_SIP));
    memset(DHCP_REAL_SIP,0,sizeof(net.subnet));

#if USE_HEAP
    pRIPMSG     = (RIP_MSG*)mem_malloc(1024,1);         // Pointer for the DHCP message
    if(pRIPMSG == NULL)
        return EPARA;
#else
    pRIPMSG     = (RIP_MSG*)EXTERN_DHCPBUF;         // Pointer for the DHCP message
#endif
    DHCP_XID    = DEFAULT_XID;				
    dhcp_state  = STATE_DHCP_READY;          // DHCP client status
    dhcp_retry_count = 0;    // retry count
    DHCP_timeout     = 0;        // DHCP Timeout flag
    dhcp_time       = 0;
    next_dhcp_time  = 0;      // DHCP Time 1s
    dhcp_tick_cnt   = 0;                    // 1ms
    
    
    dhcp_state = STATE_DHCP_READY;
    return EOK;
}

int DHCP::begin(uint8_t mac[])
{
    int ret = EOTHER;
    memcpy(net.mac,mac,6);
    ret = param_init();
    if(ret != EOK)return ret;
    
   DHCP_DBG("init_dhcp_client:%d\r\n",SOCK_DHCP);
    
   uint8_t dhcpret=0;
     while(1)
     {
        dhcpret = check_DHCP_state(0);
        switch(dhcpret)
        {
          case DHCP_RET_NONE:
              delay_ms(1);
              ret = EWAIT;
            break;
              
          case DHCP_RET_TIMEOUT:
                ret = ETIMEOUT;
          
          case DHCP_RET_UPDATE:
                ret = EOK;
            break;
          
          case DHCP_RET_CONFLICT:
              ret = EOTHER;
            break;
          
          default:
            break;
        }
        DHCP_timer_handler();
        if(ret == ETIMEOUT || ret == EOK)
            break;
    }
    #if USE_HEAP
   DHCP_DBG("--mem:%u\r\n",mem_get_free_size());
        mem_free(pRIPMSG,1);
   DHCP_DBG("--mem:%d\r\n",mem_get_free_size());
    #endif
        
    return ret;
}

