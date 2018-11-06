/*
 modified 12 Aug 2013
 by Soohwan Kim (suhwan@wiznet.co.kr)

 - 10 Apr. 2015
 Added support for Arduino Ethernet Shield 2
 by Arduino.org team

 */
#ifndef ethernet3_h
#define ethernet3_h

#include <inttypes.h>
#include "ebox_core.h"
#include "utility/w5500.h"
#include "EthernetClient.h"
#include "EthernetServer.h"
#include "Dhcp.h"

enum phyMode_t
{
    HALF_DUPLEX_10,
    FULL_DUPLEX_10,
    HALF_DUPLEX_100,
    FULL_DUPLEX_100,
    FULL_DUPLEX_100_AUTONEG,
    POWER_DOWN,
    ALL_AUTONEG
};

class EthernetClass
{
private:
    IPAddress _dnsServerAddress;
    DhcpClass *_dhcp;
    char _customHostname[32];
public:
    uint8_t _maxSockNum;
    uint8_t _pinCS;
    uint8_t _pinRST;

    static uint8_t _state[MAX_SOCK_NUM];
    static uint16_t _server_port[MAX_SOCK_NUM];

    EthernetClass()
    {
        _dhcp = NULL;
        _pinCS = 10;
        _maxSockNum = 8;
    }

    void setRstPin(uint8_t pinRST = 9); // for WIZ550io or USR-ES1, must set befor Ethernet.begin
    void setCsPin(uint8_t pinCS = 10); // must set befor Ethernet.begin

    // Initialize with less sockets but more RX/TX Buffer
    // maxSockNum = 1 Socket 0 -> RX/TX Buffer 16k
    // maxSockNum = 2 Socket 0, 1 -> RX/TX Buffer 8k
    // maxSockNum = 4 Socket 0...3 -> RX/TX Buffer 4k
    // maxSockNum = 8 (Standard) all sockets -> RX/TX Buffer 2k
    // be carefull of the MAX_SOCK_NUM, because in the moment it can't dynamicly changed
    void init(uint8_t maxSockNum = 8);

    uint8_t softreset(); // can set only after Ethernet.begin
    void hardreset(); // You need to set the Rst pin

#if defined(WIZ550io_WITH_MACADDRESS)

    // Initialize function when use the ioShield serise (included WIZ550io)
    // WIZ550io has a MAC address which is written after reset.
    // Default IP, Gateway and subnet address are also writen.
    // so, It needs some initial time. please refer WIZ550io Datasheet in details.
    int begin(void);
    void begin(IPAddress local_ip);
    void begin(IPAddress local_ip, IPAddress subnet);
    void begin(IPAddress local_ip, IPAddress subnet, IPAddress gateway);
    void begin(IPAddress local_ip, IPAddress subnet, IPAddress gateway, IPAddress dns_server);
#else
    // Initialize the Ethernet shield to use the provided MAC address and gain the rest of the
    // configuration through DHCP.
    // Returns 0 if the DHCP configuration failed, and 1 if it succeeded
    int begin(uint8_t *mac_address);
    void begin(uint8_t *mac_address, IPAddress local_ip);
    void begin(uint8_t *mac_address, IPAddress local_ip, IPAddress subnet);
    void begin(uint8_t *mac_address, IPAddress local_ip, IPAddress subnet, IPAddress gateway);
    void begin(uint8_t *mac_address, IPAddress local_ip, IPAddress subnet, IPAddress gateway, IPAddress dns_server);
#endif

    int maintain();
    void WoL(bool wol); // set Wake on LAN
    bool WoL(); // get the WoL state
    void phyMode(phyMode_t mode); // set PHYCFGR
    uint8_t phyState(); // returns the PHYCFGR
    uint8_t link(); // returns the linkstate, 1 = linked, 0 = no link
    const char *linkReport(); // returns the linkstate as a string
    uint8_t speed(); // returns speed in MB/s
    const char *speedReport(); // returns speed as a string
    uint8_t duplex(); // returns duplex mode 0 = no link, 1 = Half Duplex, 2 = Full Duplex
    const char *duplexReport(); // returns duplex mode as a string

    void setRtTimeOut(uint16_t timeout = 2000); // set the retransmission timout *100us
    uint16_t getRtTimeOut(); // get the retransmission timout
    void setRtCount(uint8_t count = 8); // set the retransmission count
    uint8_t getRtCount(); // get the retransmission count

    void macAddress(uint8_t mac[]); // get the MAC Address
    const char *macAddressReport(); // returns the the MAC Address as a string

    void setHostname(char *hostname);

    IPAddress localIP();
    IPAddress subnetMask();
    IPAddress gatewayIP();
    IPAddress dnsServerIP();

    friend class EthernetClient;
    friend class EthernetServer;
};

extern EthernetClass Ethernet;

#endif
