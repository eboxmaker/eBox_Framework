/*
 modified 12 Aug 2013
 by Soohwan Kim (suhwan@wiznet.co.kr)

- 10 Apr. 2015
 Added support for Arduino Ethernet Shield 2
 by Arduino.org team

 */

#include "Ethernet3.h"
#include "Dhcp.h"
extern W5500Class W5500;
// XXX: don't make assumptions about the value of MAX_SOCK_NUM.
uint8_t EthernetClass::_state[MAX_SOCK_NUM] = { 0, };
uint16_t EthernetClass::_server_port[MAX_SOCK_NUM] = { 0, };

//void EthernetClass::setRstPin(uint8_t pinRST) {
////  _pinRST = pinRST;
////  pinMode(_pinRST, OUTPUT);
////  digitalWrite(_pinRST, HIGH);
//  }
//void EthernetClass::setCsPin(uint8_t pinCS) {
//  _pinCS = pinCS;
//  }

//void EthernetClass::init(uint8_t maxSockNum) {
//  _maxSockNum = maxSockNum;
//  }

uint8_t EthernetClass::softreset()
{
    return w5500.softReset();
}

void EthernetClass::hardreset()
{
    if(_pinRST != 0)
    {
        //    digitalWrite(_pinRST, LOW);
        //    delay(1);
        //    digitalWrite(_pinRST, HIGH);
        //    delay(150);
    }
}

#if defined(WIZ550io_WITH_MACADDRESS)

int EthernetClass::begin(void)
{
    uint8_t mac_address[6] = {0,};
    _dhcp = new DhcpClass();

    // Initialise the basic info
    w5500.init(_maxSockNum);
    w5500.setIPAddress(IPAddress(0, 0, 0, 0).raw_address());
    w5500.getMACAddress(mac_address);

    if (strlen(_customHostname) != 0)
    {
        _dhcp->setCustomHostname(_customHostname);
    }

    // Now try to get our config info from a DHCP server
    int ret = _dhcp->beginWithDHCP(mac_address);
    if(ret == 1)
    {
        // We've successfully found a DHCP server and got our configuration info, so set things
        // accordingly
        w5500.setIPAddress(_dhcp->getLocalIp().raw_address());
        w5500.setGatewayIp(_dhcp->getGatewayIp().raw_address());
        w5500.setSubnetMask(_dhcp->getSubnetMask().raw_address());
        _dnsServerAddress = _dhcp->getDnsServerIp();
    }
    return ret;
}

void EthernetClass::begin(IPAddress local_ip)
{
    IPAddress subnet(255, 255, 255, 0);
    begin(local_ip, subnet);
}

void EthernetClass::begin(IPAddress local_ip, IPAddress subnet)
{
    // Assume the gateway will be the machine on the same network as the local IP
    // but with last octet being '1'
    IPAddress gateway = local_ip;
    gateway[3] = 1;
    begin(local_ip, subnet, gateway);
}

void EthernetClass::begin(IPAddress local_ip, IPAddress subnet, IPAddress gateway)
{
    // Assume the DNS server will be the machine on the same network as the local IP
    // but with last octet being '1'
    IPAddress dns_server = local_ip;
    dns_server[3] = 1;
    begin(local_ip, subnet, gateway, dns_server);
}

void EthernetClass::begin(IPAddress local_ip, IPAddress subnet, IPAddress gateway, IPAddress dns_server)
{
    w5500.init(_maxSockNum);
    w5500.setIPAddress(local_ip.raw_address());
    w5500.setGatewayIp(gateway.raw_address());
    w5500.setSubnetMask(subnet.raw_address());
    _dnsServerAddress = dns_server;
}

#else
int EthernetClass::begin(uint8_t *mac_address)
{
    _dhcp = new DhcpClass();
    // Initialise the basic info
    w5500.init(_maxSockNum);
    w5500.setMACAddress(mac_address);
    w5500.setIPAddress(IPAddress(0, 0, 0, 0).raw_address());

    if (strlen(_customHostname) != 0)
    {
        _dhcp->setCustomHostname(_customHostname);
    }

    // Now try to get our config info from a DHCP server
    int ret = _dhcp->beginWithDHCP(mac_address);
    if(ret == 1)
    {
        // We've successfully found a DHCP server and got our configuration info, so set things
        // accordingly
        w5500.setIPAddress(_dhcp->getLocalIp().raw_address());
        w5500.setGatewayIp(_dhcp->getGatewayIp().raw_address());
        w5500.setSubnetMask(_dhcp->getSubnetMask().raw_address());
        _dnsServerAddress = _dhcp->getDnsServerIp();
    }

    return ret;
}

void EthernetClass::begin(uint8_t *mac_address, IPAddress local_ip)
{
    IPAddress subnet(255, 255, 255, 0);
    begin(mac_address, local_ip, subnet);

}

void EthernetClass::begin(uint8_t *mac_address, IPAddress local_ip, IPAddress subnet)
{
    // Assume the gateway will be the machine on the same network as the local IP
    // but with last octet being '1'
    IPAddress gateway = local_ip;
    gateway[3] = 1;
    begin(mac_address, local_ip, subnet, gateway);
}

void EthernetClass::begin(uint8_t *mac_address, IPAddress local_ip, IPAddress subnet, IPAddress gateway)
{
    // Assume the DNS server will be the machine on the same network as the local IP
    // but with last octet being '1'
    IPAddress dns_server = local_ip;
    dns_server[3] = 1;
    begin(mac_address, local_ip, subnet, gateway, dns_server);
}

void EthernetClass::begin(uint8_t *mac, IPAddress local_ip, IPAddress subnet, IPAddress gateway, IPAddress dns_server)
{
    uint8_t buf[20];
    w5500.init(_maxSockNum);
    w5500.setMACAddress(mac);
    w5500.setIPAddress(local_ip.raw_address());
    w5500.setGatewayIp(gateway.raw_address());
    w5500.setSubnetMask(subnet.raw_address());
    _dnsServerAddress = dns_server;

    w5500.getMACAddress(buf);
    //    ebox_printf("mac : %x.%x.%x.%x.%x.%x\r\n", buf[0], buf[1], buf[2], buf[3], buf[4], buf[5]);

    w5500.getIPAddress(buf);
    //    ebox_printf("IP : %d.%d.%d.%d\r\n", buf[0], buf[1], buf[2], buf[3]);

    w5500.getGatewayIp(buf);
    //    ebox_printf("GW : %d.%d.%d.%d\r\n", buf[0], buf[1], buf[2], buf[3]);

    w5500.getSubnetMask(buf);
    //    ebox_printf("sub : %d.%d.%d.%d\r\n", buf[0], buf[1], buf[2], buf[3]);
}

#endif

int EthernetClass::maintain()
{
    int rc = DHCP_CHECK_NONE;
    if(_dhcp != NULL)
    {
        //we have a pointer to dhcp, use it
        rc = _dhcp->checkLease();
        switch ( rc )
        {
        case DHCP_CHECK_NONE:
            //nothing done
            break;
        case DHCP_CHECK_RENEW_OK:
        case DHCP_CHECK_REBIND_OK:
            //we might have got a new IP.
            w5500.setIPAddress(_dhcp->getLocalIp().raw_address());
            w5500.setGatewayIp(_dhcp->getGatewayIp().raw_address());
            w5500.setSubnetMask(_dhcp->getSubnetMask().raw_address());
            _dnsServerAddress = _dhcp->getDnsServerIp();
            break;
        default:
            //this is actually a error, it will retry though
            break;
        }
    }
    return rc;
}

void EthernetClass::WoL(bool wol)
{
    uint8_t val = w5500.readMR();
    bit_write(val, 5, wol);
    w5500.writeMR(val);
}

bool EthernetClass::WoL()
{
    uint8_t val = w5500.readMR();
    return bit_read(val, 5);
}

void EthernetClass::phyMode(phyMode_t mode)
{
    uint8_t val = w5500.getPHYCFGR();
    bit_write(val, 6, 1);
    if (mode == HALF_DUPLEX_10)
    {
        bit_write(val, 3, 0);
        bit_write(val, 4, 0);
        bit_write(val, 5, 0);
        w5500.setPHYCFGR(val);
    }
    else if (mode == FULL_DUPLEX_10)
    {
        bit_write(val, 3, 1);
        bit_write(val, 4, 0);
        bit_write(val, 5, 0);
        w5500.setPHYCFGR(val);
    }
    else if (mode == HALF_DUPLEX_100)
    {
        bit_write(val, 3, 0);
        bit_write(val, 4, 1);
        bit_write(val, 5, 0);
        w5500.setPHYCFGR(val);
    }
    else if (mode == FULL_DUPLEX_100)
    {
        bit_write(val, 3, 1);
        bit_write(val, 4, 1);
        bit_write(val, 5, 0);
        w5500.setPHYCFGR(val);
    }
    else if (mode == FULL_DUPLEX_100_AUTONEG)
    {
        bit_write(val, 3, 0);
        bit_write(val, 4, 0);
        bit_write(val, 5, 1);
        w5500.setPHYCFGR(val);
    }
    else if (mode == POWER_DOWN)
    {
        bit_write(val, 3, 0);
        bit_write(val, 4, 1);
        bit_write(val, 5, 1);
        w5500.setPHYCFGR(val);
    }
    else if (mode == ALL_AUTONEG)
    {
        bit_write(val, 3, 1);
        bit_write(val, 4, 1);
        bit_write(val, 5, 1);
        w5500.setPHYCFGR(val);
    }
    bit_write(val, 7, 0);
    w5500.setPHYCFGR(val);
    bit_write(val, 7, 1);
    w5500.setPHYCFGR(val);
}

void EthernetClass::setHostname(char *hostname)
{
    memset(_customHostname, 0, 32);
    memcpy((void *)_customHostname, (void *)hostname, strlen(hostname) >= 31 ? 31 : strlen(hostname));
}

uint8_t EthernetClass::phyState()
{
    return w5500.getPHYCFGR();
}

uint8_t EthernetClass::link()
{
    return bit_read(w5500.getPHYCFGR(), 0);
}

const char *EthernetClass::linkReport()
{
    if(bit_read(w5500.getPHYCFGR(), 0) == 1)
        return "LINK";
    return "NO LINK";
}

uint8_t EthernetClass::speed()
{
    if(bit_read(w5500.getPHYCFGR(), 0) == 1)
    {
        if(bit_read(w5500.getPHYCFGR(), 1) == 1) return 100;
        if(bit_read(w5500.getPHYCFGR(), 1) == 0) return 10;
    }
    return 0;
}

const char *EthernetClass::speedReport()
{
    if(bit_read(w5500.getPHYCFGR(), 0) == 1)
    {
        if(bit_read(w5500.getPHYCFGR(), 1) == 1) return "100 MB";
        if(bit_read(w5500.getPHYCFGR(), 1) == 0) return "10 MB";
    }
    return "NO LINK";
}

uint8_t EthernetClass::duplex()
{
    if(bit_read(w5500.getPHYCFGR(), 0) == 1)
    {
        if(bit_read(w5500.getPHYCFGR(), 2) == 1) return 2;
        if(bit_read(w5500.getPHYCFGR(), 2) == 0) return 1;
    }

    return 0;
}

const char *EthernetClass::duplexReport()
{
    if(bit_read(w5500.getPHYCFGR(), 0) == 1)
    {
        if(bit_read(w5500.getPHYCFGR(), 2) == 1) return "FULL DUPLEX";
        if(bit_read(w5500.getPHYCFGR(), 2) == 0) return "HALF DUPLEX";
    }
    
    return "NO LINK";
}

void EthernetClass::setRtTimeOut(uint16_t timeout)
{
    w5500.setRetransmissionTime(timeout);
}

uint16_t EthernetClass::getRtTimeOut()
{
    return w5500.getRetransmissionTime();
}

void EthernetClass::setRtCount(uint8_t count)
{
    w5500.setRetransmissionCount(count);
}

uint8_t EthernetClass::getRtCount()
{
    return w5500.getRetransmissionCount();
}

void EthernetClass::macAddress(uint8_t mac[])
{
    w5500.getMACAddress(mac);
}

const char *EthernetClass::macAddressReport()
{
    uint8_t mac[6];
    static char str[18];
    w5500.getMACAddress(mac);
    ebox_sprintf(str, "%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    return str;
}

IPAddress EthernetClass::localIP()
{
    IPAddress ret;
    w5500.getIPAddress(ret.raw_address());
    return ret;
}

IPAddress EthernetClass::subnetMask()
{
    IPAddress ret;
    w5500.getSubnetMask(ret.raw_address());
    return ret;
}

IPAddress EthernetClass::gatewayIP()
{
    IPAddress ret;
    w5500.getGatewayIp(ret.raw_address());
    return ret;
}

IPAddress EthernetClass::dnsServerIP()
{
    return _dnsServerAddress;
}

EthernetClass Ethernet;
