#ifndef __LWIPOPTS_H__
#define __LWIPOPTS_H__

/**
* SYS_LIGHTWEIGHT_PROT==1: if you want inter-task protection for certain
* critical regions during buffer allocation, deallocation and memory
* allocation and deallocation.
*/
#define SYS_LIGHTWEIGHT_PROT 0

/**
* NO_SYS==1: Provides VERY minimal functionality. Otherwise,
* use lwIP facilities.
*/
#define NO_SYS 1 // 无操作系统
/**
* NO_SYS_NO_TIMERS==1: Drop support for sys_timeout when NO_SYS==1
* Mainly for compatibility to old versions.
*/
#define NO_SYS_NO_TIMERS 0 // 这样就不需要定义sys_now函数

/* ---------- Memory options ---------- */
/* MEM_ALIGNMENT: should be set to the alignment of the CPU for which
lwIP is compiled. 4 byte alignment -> define MEM_ALIGNMENT to 4, 2
byte alignment -> define MEM_ALIGNMENT to 2. */
#define MEM_ALIGNMENT 4

/* MEM_SIZE: the size of the heap memory. If the application will send
a lot of data that needs to be copied, this should be set high. */
#define MEM_SIZE (5 * 1024)

/* MEMP_NUM_PBUF: the number of memp struct pbufs. If the application
sends a lot of data out of ROM (or other static memory), this
should be set high. */
#define MEMP_NUM_PBUF 10
/* MEMP_NUM_UDP_PCB: the number of UDP protocol control blocks. One
per active UDP "connection". */
#define MEMP_NUM_UDP_PCB 6
/* MEMP_NUM_TCP_PCB: the number of simulatenously active TCP
connections. */
#define MEMP_NUM_TCP_PCB 10
/* MEMP_NUM_TCP_PCB_LISTEN: the number of listening TCP
connections. */
#define MEMP_NUM_TCP_PCB_LISTEN 6
/* MEMP_NUM_TCP_SEG: the number of simultaneously queued TCP
segments. */
#define MEMP_NUM_TCP_SEG 12
/* MEMP_NUM_SYS_TIMEOUT: the number of simulateously active
timeouts. */
#define MEMP_NUM_SYS_TIMEOUT 3

/* ---------- Pbuf options ---------- */
/* PBUF_POOL_SIZE: the number of buffers in the pbuf pool. */
#define PBUF_POOL_SIZE 10

/* PBUF_POOL_BUFSIZE: the size of each pbuf in the pbuf pool. */
#define PBUF_POOL_BUFSIZE 1500

/* ---------- TCP options ---------- */
#define LWIP_TCP 1
#define TCP_TTL 255

/* Controls if TCP should queue segments that arrive out of
order. Define to 0 if your device is low on memory. */
#define TCP_QUEUE_OOSEQ 0

/* TCP Maximum segment size. */
#define TCP_MSS (1500 - 40) /* TCP_MSS = (Ethernet MTU - IP header size - TCP header size) */

/* TCP sender buffer space (bytes). */
#define TCP_SND_BUF (2 * TCP_MSS)

/* TCP sender buffer space (pbufs). This must be at least = 2 *
TCP_SND_BUF/TCP_MSS for things to work. */
#define TCP_SND_QUEUELEN (6 * TCP_SND_BUF) / TCP_MSS

/* TCP receive window. */
#define TCP_WND (2 * TCP_MSS)

/* ---------- ICMP options ---------- */
#define LWIP_ICMP 1

/* ---------- DHCP options ---------- */
/* Define LWIP_DHCP to 1 if you want DHCP configuration of
interfaces. DHCP is not implemented in lwIP 0.5.1, however, so
turning this on does currently not work. */
#define LWIP_DHCP 0

/* ---------- UDP options ---------- */
#define LWIP_UDP 1
#define UDP_TTL 255

/* ---------- Statistics options ---------- */
#define LWIP_STATS 0
#define LWIP_PROVIDE_ERRNO 1

// LWIP_NETCONN==1: Enable Netconn API (require to use api_lib.c)
#define LWIP_NETCONN 0

// LWIP_SOCKET==1: Enable Socket API (require to use sockets.c)
#define LWIP_SOCKET 0

// 这个必须要定义, 否则在执行lwip_init函数时会在串口中提示:
// Assertion "Struct packing not implemented correctly. Check your lwIP port." failed at line 345 in Library\lwip\core\init.c
#define PACK_STRUCT_BEGIN __packed
/*******************************************************************************************************************/
//#define LWIP_DEBUG 1
#define LWIP_DBG_TYPES_ON LWIP_DBG_OFF

/**
* ETHARP_DEBUG: Enable debugging in etharp.c.
*/
#define ETHARP_DEBUG LWIP_DBG_OFF

/**
* NETIF_DEBUG: Enable debugging in netif.c.
*/
#define NETIF_DEBUG LWIP_DBG_OFF

/**
* PBUF_DEBUG: Enable debugging in pbuf.c.
*/
#define PBUF_DEBUG LWIP_DBG_OFF

/**
* API_LIB_DEBUG: Enable debugging in api_lib.c.
*/
#define API_LIB_DEBUG LWIP_DBG_OFF

/**
* API_MSG_DEBUG: Enable debugging in api_msg.c.
*/
#define API_MSG_DEBUG LWIP_DBG_OFF

/**
* SOCKETS_DEBUG: Enable debugging in sockets.c.
*/
#define SOCKETS_DEBUG LWIP_DBG_OFF

/**
* ICMP_DEBUG: Enable debugging in icmp.c.
*/
#define ICMP_DEBUG LWIP_DBG_OFF

/**
* IGMP_DEBUG: Enable debugging in igmp.c.
*/
#define IGMP_DEBUG LWIP_DBG_OFF

/**
* INET_DEBUG: Enable debugging in inet.c.
*/
#define INET_DEBUG LWIP_DBG_OFF

/**
* IP_DEBUG: Enable debugging for IP.
*/
#define IP_DEBUG LWIP_DBG_OFF

/**
* IP_REASS_DEBUG: Enable debugging in ip_frag.c for both frag & reass.
*/
#define IP_REASS_DEBUG LWIP_DBG_OFF
/**
* RAW_DEBUG: Enable debugging in raw.c.
*/
#define RAW_DEBUG LWIP_DBG_OFF

/**
* MEM_DEBUG: Enable debugging in mem.c.
*/
#define MEM_DEBUG LWIP_DBG_OFF

/**
* MEMP_DEBUG: Enable debugging in memp.c.
*/
#define MEMP_DEBUG LWIP_DBG_OFF

/**
* SYS_DEBUG: Enable debugging in sys.c.
*/
#define SYS_DEBUG LWIP_DBG_OFF

/**
* TCP_DEBUG: Enable debugging for TCP.
*/
#define TCP_DEBUG LWIP_DBG_OFF

/**
* TCP_INPUT_DEBUG: Enable debugging in tcp_in.c for incoming debug.
*/
#define TCP_INPUT_DEBUG LWIP_DBG_OFF
/**
* TCP_FR_DEBUG: Enable debugging in tcp_in.c for fast retransmit.
*/
#define TCP_FR_DEBUG LWIP_DBG_OFF

/**
* TCP_RTO_DEBUG: Enable debugging in TCP for retransmit
* timeout.
*/
#define TCP_RTO_DEBUG LWIP_DBG_OFF

/**
* TCP_CWND_DEBUG: Enable debugging for TCP congestion window.
*/
#define TCP_CWND_DEBUG LWIP_DBG_OFF

/**
* TCP_WND_DEBUG: Enable debugging in tcp_in.c for window updating.
*/
#define TCP_WND_DEBUG LWIP_DBG_OFF

/**
* TCP_OUTPUT_DEBUG: Enable debugging in tcp_out.c output functions.
*/
#define TCP_OUTPUT_DEBUG LWIP_DBG_OFF
/**
* TCP_RST_DEBUG: Enable debugging for TCP with the RST message.
*/
#define TCP_RST_DEBUG LWIP_DBG_OFF

/**
* TCP_QLEN_DEBUG: Enable debugging for TCP queue lengths.
*/
#define TCP_QLEN_DEBUG LWIP_DBG_OFF

/**
* UDP_DEBUG: Enable debugging in UDP.
*/
#define UDP_DEBUG LWIP_DBG_OFF

/**
* TCPIP_DEBUG: Enable debugging in tcpip.c.
*/
#define TCPIP_DEBUG LWIP_DBG_OFF

/**
* PPP_DEBUG: Enable debugging for PPP.
*/
#define PPP_DEBUG LWIP_DBG_OFF

/**
* SLIP_DEBUG: Enable debugging in slipif.c.
*/
#define SLIP_DEBUG LWIP_DBG_OFF

/**
* DHCP_DEBUG: Enable debugging in dhcp.c.
*/
#define DHCP_DEBUG LWIP_DBG_OFF

/**
* AUTOIP_DEBUG: Enable debugging in autoip.c.
*/
#define AUTOIP_DEBUG LWIP_DBG_OFF

/**
* SNMP_MSG_DEBUG: Enable debugging for SNMP messages.
*/
#define SNMP_MSG_DEBUG LWIP_DBG_OFF

/**
* SNMP_MIB_DEBUG: Enable debugging for SNMP MIBs.
*/
#define SNMP_MIB_DEBUG LWIP_DBG_OFF

/**
* DNS_DEBUG: Enable debugging for DNS.
*/
#define DNS_DEBUG LWIP_DBG_OFF
#endif /* __LWIPOPTS_H__ */
