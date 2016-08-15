/**
  ******************************************************************************
  * @file    dhcp.h
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
#ifndef __DHCP_H
#define __DHCP_H
#include "socket.h"

#define USE_HEAP 1

#ifdef USE_HEAP
    #include "heap.h"
#endif





#define DHCP_RET_NONE      0
#define DHCP_RET_ERR       1
#define DHCP_RET_TIMEOUT   2
#define DHCP_RET_UPDATE    3
#define DHCP_RET_CONFLICT  4




/* DHCP state machine. */
#define  STATE_DHCP_READY        0
#define  STATE_DHCP_DISCOVER     1
#define  STATE_DHCP_REQUEST      2
#define  STATE_DHCP_LEASED       3
#define  STATE_DHCP_REREQUEST    4
#define  STATE_DHCP_RELEASE      5
         
#define  MAX_DHCP_RETRY          3
#define  DHCP_WAIT_TIME          5
         
#define  DHCP_FLAGSBROADCAST     0x8000

/* UDP port numbers for DHCP */
#define  DHCP_SERVER_PORT        67	/* from server to client */
#define  DHCP_CLIENT_PORT        68	/* from client to server */

/* DHCP message OP code */
#define  DHCP_BOOTREQUEST        1
#define  DHCP_BOOTREPLY          2

/* DHCP message type */
#define  DHCP_DISCOVER           1
#define  DHCP_OFFER              2
#define  DHCP_REQUEST            3
#define  DHCP_DECLINE            4
#define  DHCP_ACK                5
#define  DHCP_NAK                6
#define  DHCP_RELEASE            7
#define  DHCP_INFORM             8

#define DHCP_HTYPE10MB           1
#define DHCP_HTYPE100MB          2

#define DHCP_HLENETHERNET        6
#define DHCP_HOPS                0
#define DHCP_SECS                0

#define MAGIC_COOKIE		         0x63825363
#define DEFAULT_XID              0x12345678

#define DEFAULT_LEASETIME        0xffffffff	/* infinite lease time */

/* DHCP option and value (cf. RFC1533) */
enum
{
   padOption               = 0,
   subnetMask              = 1,
   timerOffset             = 2,
   routersOnSubnet         = 3,
   timeServer              = 4,
   nameServer              = 5,
// dns                     = 6,
   _dns                    = 6,
   logServer               = 7,
   cookieServer            = 8,
   lprServer               = 9,
   impressServer           = 10,
   resourceLocationServer  = 11,
   hostName                = 12,
   bootFileSize            = 13,
   meritDumpFile           = 14,
   domainName              = 15,
   swapServer              = 16,
   rootPath                = 17,
   extentionsPath          = 18,
   IPforwarding            = 19,
   nonLocalSourceRouting   = 20,
   policyFilter            = 21,
   maxDgramReasmSize       = 22,
   defaultIPTTL            = 23,
   pathMTUagingTimeout     = 24,
   pathMTUplateauTable     = 25,
   ifMTU                   = 26,
   allSubnetsLocal         = 27,
   broadcastAddr           = 28,
   performMaskDiscovery    = 29,
   maskSupplier            = 30,
   performRouterDiscovery  = 31,
   routerSolicitationAddr  = 32,
   staticRoute             = 33,
   trailerEncapsulation    = 34,
   arpCacheTimeout         = 35,
   ethernetEncapsulation   = 36,
   tcpDefaultTTL           = 37,
   tcpKeepaliveInterval    = 38,
   tcpKeepaliveGarbage     = 39,
   nisDomainName           = 40,
   nisServers              = 41,
   ntpServers              = 42,
   vendorSpecificInfo      = 43,
   netBIOSnameServer       = 44,
   netBIOSdgramDistServer  = 45,
   netBIOSnodeType         = 46,
   netBIOSscope            = 47,
   xFontServer             = 48,
   xDisplayManager         = 49,
   dhcpRequestedIPaddr     = 50,
   dhcpIPaddrLeaseTime     = 51,
   dhcpOptionOverload      = 52,
   dhcpMessageType         = 53,
   dhcpServerIdentifier    = 54,
   dhcpParamRequest        = 55,
   dhcpMsg                 = 56,
   dhcpMaxMsgSize          = 57,
   dhcpT1value             = 58,
   dhcpT2value             = 59,
   dhcpClassIdentifier     = 60,
   dhcpClientIdentifier    = 61,
   endOption               = 255
};

typedef struct _RIP_MSG
{
   uint8_t  op; 
   uint8_t  htype; 
   uint8_t  hlen;
   uint8_t  hops;
   uint32_t xid;
   uint16_t secs;
   uint16_t flags;
   uint8_t  ciaddr[4];
   uint8_t  yiaddr[4];
   uint8_t  siaddr[4];
   uint8_t  giaddr[4];
   uint8_t  chaddr[16];
   uint8_t  sname[64];
   uint8_t  file[128];
   uint8_t  OPT[312];
}RIP_MSG;

#define MAX_DHCP_OPT	16

void  init_dhcp_client(void);
//uint8 getIP_DHCPS(void);      // Get the network configuration from the DHCP server

class DHCP
{
    public:
        int begin(uint8_t mac[]);
        uint8_t check_DHCP_state(SOCKET s); // Check the DHCP state
    
        IPCONFIG    net;
    private:
        void  send_DHCP_DISCOVER(void);// Send the discovery message to the DHCP server
        void  send_DHCP_REQUEST(void);// Send the request message to the DHCP server
        void  send_DHCP_RELEASE_DECLINE(char msgtype);// send the release message to the DHCP server
        uint8_t parseDHCPMSG(uint16_t length);// Receive the message from DHCP server and parse it.
        void  reset_DHCP_time(void);        // Initialize DHCP Timer
        uint8_t check_leasedIP(void);         // Check the leased IP address
        void  DHCP_timer_handler(void);     // DHCP Timer handler
        void  check_DHCP_Timeout(void);     // Check DHCP Timeout 
    private:
        int param_init(void);


        uint8_t*   DHCP_HOST_NAME ;   // HOST NAME
        uint8_t    DHCP_SIP[4] ;      // DNS server ip address is discovered
        uint8_t    DHCP_REAL_SIP[4]; // For extract my DHCP server in a few DHCP servers
        uint8_t    OLD_SIP[4];        // Previous local ip address received from DHCP server



        RIP_MSG*  pRIPMSG;
        uint32_t  DHCP_XID;		
        uint8_t  dhcp_state;// DHCP client status
        uint8_t  dhcp_retry_count ;    // retry count
        uint8_t  DHCP_timeout     ;        // DHCP Timeout flag
        uint32_t  dhcp_lease_time;    // Leased time
        uint32_t  dhcp_time       ;
        uint32_t  next_dhcp_time  ;      // DHCP Time 1s
        uint32_t  dhcp_tick_cnt   ;                    // 1ms

        uint8_t  DHCP_timer;

};
extern DHCP dhcp;

#endif
