#include "lwip_process.h"

struct ip4_addr ipaddr, netmask, gw;
struct netif enc28j60;




void lwip_init_app()
{
    lwip_init();
    IP4_ADDR(&ipaddr, 192, 168, 1, 220); // IP地址
    IP4_ADDR(&netmask, 255, 255, 255, 0); // 子网掩码
    IP4_ADDR(&gw, 192, 168, 1, 1); // 网关

    netif_add(&enc28j60, &ipaddr, &netmask, &gw, NULL, ethernetif_init, ethernet_input);
    netif_set_default(&enc28j60); // 设为默认网卡
    netif_set_up(&enc28j60);

}


void lwip_process()
{

    static uint32_t TCPTimer = 0;
    static uint32_t ARPTimer = 0;
    static uint32_t DHCPfineTimer = 0;

    if (eth.get_packet_num() != 0)
    {
        ethernetif_input(&enc28j60);
    }
    // 若定时器溢出
    if (millis() - TCPTimer > TCP_TMR_INTERVAL)
    {
        TCPTimer = millis();
        tcp_tmr(); // TCP定时处理
    }

    if (millis() - ARPTimer > TCP_TMR_INTERVAL)
    {
        ARPTimer = millis();
        etharp_tmr(); // TCP定时处理
    }
#if LWIP_DHCP //如果使用DHCP的话

    if (millis() - DHCPfineTimer > TCP_TMR_INTERVAL)
    {
        DHCPfineTimer = millis();

        dhcp_fine_tmr(); // TCP定时处理
        if ((lwipdev.dhcpstatus != 2) && (lwipdev.dhcpstatus != 0XFF))
        {
            lwip_dhcp_process_handle();  //DHCP处理
        }
    }
#endif
}

