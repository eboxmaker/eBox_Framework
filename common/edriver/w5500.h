/**
  ******************************************************************************
  * @file    w5500.h
  * @author  shentq
  * @version V1.2
  * @date    2016/08/14
  * @brief   
  ******************************************************************************
  * @attention
  *
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

#ifndef __W5500_H
#define __W5500_H
#include "ebox.h"


typedef int8_t 			SOCKET;





#define	MAX_SOCK_NUM		8	/**< Maxmium number of socket  */

static uint8_t I_STATUS[MAX_SOCK_NUM];
static uint16_t SSIZE[MAX_SOCK_NUM]; /**< Max Tx buffer size by each channel */
static uint16_t RSIZE[MAX_SOCK_NUM]; /**< Max Rx buffer size by each channel */

class W5500
{
public:
    uint8_t mac[6];
    uint8_t ip[4];
    uint8_t	subnet[4];
    uint8_t gw[4];
    uint8_t dns[4];

public:
    W5500(Gpio *cs, Gpio *rst, Gpio *int_pin, Spi *spi)
    {
        this->cs      = cs;
        this->rst_pin = rst;
        this->int_pin = int_pin;
        this->spi     = spi;
    }
    void begin(uint8_t dev_num,uint8_t *mac);
    void begin(uint8_t dev_num, uint8_t *mac, uint8_t *ip, uint8_t *subnet, uint8_t *gateway, uint8_t *dns);
    void reset();

    void send_data_processing(SOCKET s, uint8_t *data, uint16_t len);
    void recv_data_processing(SOCKET s, uint8_t *data, uint16_t len);

    void write(uint32_t addrbsb, uint8_t data);
    uint8_t   read (uint32_t addrbsb);
    uint16_t  write(uint32_t addrbsb, uint8_t *buf, uint16_t len);
    uint16_t  read (uint32_t addrbsb, uint8_t *buf, uint16_t len);
    void sysinit( uint8_t  *tx_size, uint8_t  *rx_size  );


    uint8_t getISR(SOCKET s);
    void putISR(SOCKET s, uint8_t val);
    uint16_t getRxMAX(SOCKET s);
    uint16_t getTxMAX(SOCKET s);

    //通用寄存器
    void setSHAR(uint8_t *addr );//mac
    void setSIPR(uint8_t *addr );//ip
    void setSUBR(uint8_t *addr );//mask
    void setGAR(uint8_t *addr );	//gateway

    void getSHAR(uint8_t  *addr );//mac
    void getSIPR(uint8_t  *addr);//ip
    void getSUBR(uint8_t  *addr);//mask
    void getGWIP(uint8_t  *addr);//gateway
    void getGAR(uint8_t  *addr);//gateway

    void getMAC(uint8_t  *addr );//mac
    void getIP(uint8_t  *addr);//ip
    void getSubnet(uint8_t  *addr);//mask
    void getGateway(uint8_t  *addr);//gateway

    void setMR(uint8_t val);

    void setRTR(uint16_t timeout);
    void setRCR(uint8_t retry);

    //中断屏蔽寄存器
    void setIMR(uint8_t val);
    void setSIMR(uint8_t val);
    //清除中断标志位
    void setIR(uint8_t mask);
    void setSIR(uint8_t mask);
    //获取中断状态寄存器
    uint8_t   getIR( void );
    uint8_t   getSIR( void );

    //socket寄存器
    //中断屏蔽寄存器
    void setSn_IMR(SOCKET s, uint8_t mask);
    uint8_t   getSn_IMR(SOCKET s);
    //获取中断状态寄存器
    uint8_t   getSn_IR(SOCKET s);
    //清除中断标志位
    void setSn_IR(SOCKET s, uint8_t val);

    void setSn_MSS(SOCKET s, uint16_t Sn_MSSR);
    void setSn_TTL(SOCKET s, uint8_t ttl);

    //获取远端IP和端口
    void getSn_DIPR(SOCKET s, uint8_t *ip);
    uint16_t getSn_DPORT(SOCKET s);

    uint8_t      getSn_SR(SOCKET s);
    uint16_t     get_tx_free_size(SOCKET s);
    uint16_t     get_rx_recv_size(SOCKET s);



    void attch_interruput_event(void (*callbackFun)(void))
    {
        Exti ex(int_pin, EXTI_Trigger_Falling);
        ex.begin();
        ex.attach_interrupt(callbackFun);
        ex.interrupt(ENABLE);
    }
private:
    Gpio *cs;
    Gpio *rst_pin;
    Gpio *int_pin;
    SPI_CONFIG_TYPE spi_dev_w5500;
    Spi *spi;


};
#define SOCKET0 0
#define SOCKET1 1
#define SOCKET2 2
#define SOCKET3 3
#define SOCKET4 4
#define SOCKET5 5
#define SOCKET6 6
#define SOCKET7 7

#define MR                          (0x000000)

/**
 @brief Gateway IP Register address
 */
#define GAR0                        (0x000100)
#define GAR1                        (0x000200)
#define GAR2                        (0x000300)
#define GAR3                        (0x000400)
/**
 @brief Subnet mask Register address
 */
#define SUBR0                       (0x000500)
#define SUBR1                       (0x000600)
#define SUBR2                       (0x000700)
#define SUBR3                       (0x000800)

/**
 @brief Source MAC Register address
 */
#define SHAR0                       (0x000900)
#define SHAR1                       (0x000A00)
#define SHAR2                       (0x000B00)
#define SHAR3                       (0x000C00)
#define SHAR4                       (0x000D00)
#define SHAR5                       (0x000E00)
/**
 @brief Source IP Register address
 */
#define SIPR0                       (0x000F00)
#define SIPR1                       (0x001000)
#define SIPR2                       (0x001100)
#define SIPR3                       (0x001200)
/**
 @brief set Interrupt low level timer register address
 */
#define INTLEVEL0                   (0x001300)
#define INTLEVEL1                   (0x001400)
/**
 @brief Interrupt Register
 */
#define IR                          (0x001500)
/**
 @brief Interrupt mask register
 */
#define IMR                         (0x001600)
/**
 @brief Socket Interrupt Register
 */
#define SIR                         (0x001700)
/**
 @brief Socket Interrupt Mask Register
 */
#define SIMR                        (0x001800)
/**
 @brief Timeout register address( 1 is 100us )
 */
#define RTR0                        (0x001900)
#define RTR1                        (0x001A00)
/**
 @brief Retry count reigster
 */
#define WIZ_RCR                         (0x001B00)
/**
 @briefPPP LCP Request Timer register  in PPPoE mode
 */
#define PTIMER                      (0x001C00)
/**
 @brief PPP LCP Magic number register  in PPPoE mode
 */
#define PMAGIC                      (0x001D00)
/**
 @brief PPP Destination MAC Register address
 */
#define PDHAR0                      (0x001E00)
#define PDHAR1                      (0x001F00)
#define PDHAR2                      (0x002000)
#define PDHAR3                      (0x002100)
#define PDHAR4                      (0x002200)
#define PDHAR5                      (0x002300)
/**
 @brief PPP Session Identification Register
 */
#define PSID0                       (0x002400)
#define PSID1                       (0x002500)
/**
 @brief PPP Maximum Segment Size(MSS) register
 */
#define PMR0                        (0x002600)
#define PMR1                        (0x002700)
/**
 @brief Unreachable IP register address in UDP mode
 */
#define UIPR0                       (0x002800)
#define UIPR1                       (0x002900)
#define UIPR2                       (0x002A00)
#define UIPR3                       (0x002B00)
/**
 @brief Unreachable Port register address in UDP mode
 */
#define UPORT0                      (0x002C00)
#define UPORT1                      (0x002D00)
/**
 @brief PHY Configuration Register
 */
#define PHYCFGR                      (0x002E00)
/**
 @brief chip version register address
 */
#define VERSIONR                    (0x003900)



/**
 @brief socket Mode register
 */
#define Sn_MR(ch)                       (0x000008 + (ch<<5))

/**
 @brief channel Sn_CR register
 */
#define Sn_CR(ch)                       (0x000108 + (ch<<5))
/**
 @brief channel interrupt register
 */
#define Sn_IR(ch)                       (0x000208 + (ch<<5))
/**
 @brief channel status register
 */
#define Sn_SR(ch)                       (0x000308 + (ch<<5))


/**
 @brief source port register
 */
#define Sn_PORT0(ch)                    (0x000408 + (ch<<5))
#define Sn_PORT1(ch)                    (0x000508 + (ch<<5))
/**
 @brief Peer MAC register address
 */
#define Sn_DHAR0(ch)                    (0x000608 + (ch<<5))
#define Sn_DHAR1(ch)                    (0x000708 + (ch<<5))
#define Sn_DHAR2(ch)                    (0x000808 + (ch<<5))
#define Sn_DHAR3(ch)                    (0x000908 + (ch<<5))
#define Sn_DHAR4(ch)                    (0x000A08 + (ch<<5))
#define Sn_DHAR5(ch)                    (0x000B08 + (ch<<5))
/**
 @brief Peer IP register address
 */
#define Sn_DIPR0(ch)                    (0x000C08 + (ch<<5))
#define Sn_DIPR1(ch)                    (0x000D08 + (ch<<5))
#define Sn_DIPR2(ch)                    (0x000E08 + (ch<<5))
#define Sn_DIPR3(ch)                    (0x000F08 + (ch<<5))
/**
 @brief Peer port register address
 */
#define Sn_DPORT0(ch)                   (0x001008 + (ch<<5))
#define Sn_DPORT1(ch)                   (0x001108 + (ch<<5))
/**
 @brief Maximum Segment Size(Sn_MSSR0) register address
 */
#define Sn_MSSR0(ch)                    (0x001208 + (ch<<5))
#define Sn_MSSR1(ch)                    (0x001308 + (ch<<5))
/* *
 @brief IP Type of Service(TOS) Register
 */
#define Sn_TOS(ch)                      (0x001508 + (ch<<5))
/**
 @brief IP Time to live(TTL) Register
 */
#define Sn_TTL(ch)                      (0x001608 + (ch<<5))
/**
 @brief Receive memory size reigster
 */
#define Sn_RXMEM_SIZE(ch)               (0x001E08 + (ch<<5))
/**
 @brief Transmit memory size reigster
 */
#define Sn_TXMEM_SIZE(ch)               (0x001F08 + (ch<<5))
/**
 @brief Transmit free memory size register
 */
#define Sn_TX_FSR0(ch)                  (0x002008 + (ch<<5))
#define Sn_TX_FSR1(ch)                  (0x002108 + (ch<<5))
/**
 @brief Transmit memory read pointer register address
 */
#define Sn_TX_RD0(ch)                   (0x002208 + (ch<<5))
#define Sn_TX_RD1(ch)                   (0x002308 + (ch<<5))
/**
 @brief Transmit memory write pointer register address
 */
#define Sn_TX_WR0(ch)                   (0x002408 + (ch<<5))
#define Sn_TX_WR1(ch)                   (0x002508 + (ch<<5))
/**
 @brief Received data size register
 */
#define Sn_RX_RSR0(ch)                  (0x002608 + (ch<<5))
#define Sn_RX_RSR1(ch)                  (0x002708 + (ch<<5))
/**
 @brief Read point of Receive memory
 */
#define Sn_RX_RD0(ch)                   (0x002808 + (ch<<5))
#define Sn_RX_RD1(ch)                   (0x002908 + (ch<<5))
/**
 @brief Write point of Receive memory
 */
#define Sn_RX_WR0(ch)                   (0x002A08 + (ch<<5))
#define Sn_RX_WR1(ch)                   (0x002B08 + (ch<<5))
/**
 @brief socket interrupt mask register
 */
#define Sn_IMR(ch)                      (0x002C08 + (ch<<5))
/**
 @brief frag field value in IP header register
 */
#define Sn_FRAG(ch)                     (0x002D08 + (ch<<5))
/**
 @brief Keep Timer register
 */
#define Sn_KPALVTR(ch)                  (0x002F08 + (ch<<5))

/*MODE register values */
#define MR_RST                       0x80 /**< reset */
#define MR_WOL                       0x20 /**< Wake on Lan */
#define MR_PB                        0x10 /**< ping block */
#define MR_PPPOE                     0x08 /**< enable pppoe */
#define MR_UDP_FARP                  0x02 /**< enbale FORCE ARP */


/*IR register values */
#define IR_CONFLICT                  0x80 /**< check ip confict */
#define IR_UNREACH                   0x40 /**< get the destination unreachable message in UDP sending */
#define IR_PPPoE                     0x20 /**< get the PPPoE close message */
#define IR_MAGIC                     0x10 /**< get the magic packet interrupt */
/*SIR register values */
#define SIR_0                  0x01 /**< check ip confict */
#define SIR_1                  0x02 /**< check ip confict */
#define SIR_2                  0x04 /**< check ip confict */
#define SIR_3                  0x08 /**< check ip confict */
#define SIR_4                  0x10 /**< check ip confict */
#define SIR_5                  0x20 /**< check ip confict */
#define SIR_6                  0x40 /**< check ip confict */
#define SIR_7                  0x80 /**< check ip confict */


/*Sn_MR values */
#define Sn_MR_CLOSE                  0x00     /**< unused socket */
#define Sn_MR_TCP                    0x01     /**< TCP */
#define Sn_MR_UDP                    0x02     /**< UDP */
#define Sn_MR_IPRAW                  0x03      /**< IP LAYER RAW SOCK */
#define Sn_MR_MACRAW                 0x04      /**< MAC LAYER RAW SOCK */
#define Sn_MR_PPPOE                  0x05     /**< PPPoE */
#define Sn_MR_UCASTB                 0x10     /**< Unicast Block in UDP Multicating*/
#define Sn_MR_ND                     0x20     /**< No Delayed Ack(TCP) flag */
#define Sn_MR_MC                     0x20     /**< Multicast IGMP (UDP) flag */
#define Sn_MR_BCASTB                 0x40     /**< Broadcast blcok in UDP Multicating */
#define Sn_MR_MULTI                  0x80     /**< support UDP Multicating */

/*Sn_MR values on MACRAW MODE */
#define Sn_MR_MIP6N                  0x10     /**< IPv6 packet Block */
#define Sn_MR_MMB                    0x20     /**< IPv4 Multicasting Block */
//#define Sn_MR_BCASTB                 0x40     /**< Broadcast blcok */
#define Sn_MR_MFEN                   0x80     /**< support MAC filter enable */


/*Sn_CR values */
#define Sn_CR_OPEN                   0x01     /**< initialize or open socket */
#define Sn_CR_LISTEN                 0x02     /**< wait connection request in tcp mode(Server mode) */
#define Sn_CR_CONNECT                0x04     /**< send connection request in tcp mode(Client mode) */
#define Sn_CR_DISCON                 0x08     /**< send closing reqeuset in tcp mode */
#define Sn_CR_CLOSE                  0x10     /**< close socket */
#define Sn_CR_SEND                   0x20     /**< update txbuf pointer, send data */
#define Sn_CR_SEND_MAC               0x21     /**< send data with MAC address, so without ARP process */
#define Sn_CR_SEND_KEEP              0x22     /**<  send keep alive message */
#define Sn_CR_RECV                   0x40     /**< update rxbuf pointer, recv data */

#ifdef __DEF_IINCHIP_PPP__
#define Sn_CR_PCON                0x23
#define Sn_CR_PDISCON             0x24
#define Sn_CR_PCR                 0x25
#define Sn_CR_PCN                 0x26
#define Sn_CR_PCJ                 0x27
#endif

/*Sn_IR values */
#ifdef __DEF_IINCHIP_PPP__
#define Sn_IR_PRECV               0x80
#define Sn_IR_PFAIL               0x40
#define Sn_IR_PNEXT               0x20
#endif

#define Sn_IR_SEND_OK                0x10     /**< complete sending */
#define Sn_IR_TIMEOUT                0x08     /**< assert timeout */
#define Sn_IR_RECV                   0x04     /**< receiving data */
#define Sn_IR_DISCON                 0x02     /**< closed socket */
#define Sn_IR_CON                    0x01     /**< established connection */

/*Sn_SR values */
#define SOCK_CLOSED                  0x00     /**< closed */
#define SOCK_INIT                    0x13     /**< init state */
#define SOCK_LISTEN                  0x14     /**< listen state */
#define SOCK_SYNSENT                 0x15     /**< connection state */
#define SOCK_SYNRECV                 0x16     /**< connection state */
#define SOCK_ESTABLISHED             0x17     /**< success to connect */
#define SOCK_FIN_WAIT                0x18     /**< closing state */
#define SOCK_CLOSING                 0x1A     /**< closing state */
#define SOCK_TIME_WAIT               0x1B     /**< closing state */
#define SOCK_CLOSE_WAIT              0x1C     /**< closing state */
#define SOCK_LAST_ACK                0x1D     /**< closing state */
#define SOCK_UDP                     0x22     /**< udp socket */
#define SOCK_IPRAW                   0x32     /**< ip raw mode socket */
#define SOCK_MACRAW                  0x42     /**< mac raw mode socket */
#define SOCK_PPPOE                   0x5F     /**< pppoe socket */

/*IP PROTOCOL */
#define IPPROTO_IP                   0        /**< Dummy for IP */
#define IPPROTO_ICMP                 1        /**< Control message protocol */
#define IPPROTO_IGMP                 2        /**< Internet group management protocol */
#define IPPROTO_GGP                  3        /**< Gateway^2 (deprecated) */
#define IPPROTO_TCP                  6        /**< TCP */
#define IPPROTO_PUP                  12       /**< PUP */
#define IPPROTO_UDP                  17       /**< UDP */
#define IPPROTO_IDP                  22       /**< XNS idp */
#define IPPROTO_ND                   77       /**< UNOFFICIAL net disk protocol */
#define IPPROTO_RAW                  255      /**< Raw IP packet */

#endif
