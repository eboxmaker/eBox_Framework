#include "lwip_process.h"

struct ip4_addr ipaddr, netmask, gw;
struct netif enc28j60;




void lwip_init_app()
{
    lwip_init();
    IP4_ADDR(&ipaddr, 192, 168, 1, 220); // IP��ַ
    IP4_ADDR(&netmask, 255, 255, 255, 0); // ��������
    IP4_ADDR(&gw, 192, 168, 1, 1); // ����

    netif_add(&enc28j60, &ipaddr, &netmask, &gw, NULL, ethernetif_init, ethernet_input);
    netif_set_default(&enc28j60); // ��ΪĬ������
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
    // ����ʱ�����
    if (millis() - TCPTimer > TCP_TMR_INTERVAL)
    {
        TCPTimer = millis();
        tcp_tmr(); // TCP��ʱ����
    }

    if (millis() - ARPTimer > TCP_TMR_INTERVAL)
    {
        ARPTimer = millis();
        etharp_tmr(); // TCP��ʱ����
    }
#if LWIP_DHCP //���ʹ��DHCP�Ļ�

    if (millis() - DHCPfineTimer > TCP_TMR_INTERVAL)
    {
        DHCPfineTimer = millis();

        dhcp_fine_tmr(); // TCP��ʱ����
        if ((lwipdev.dhcpstatus != 2) && (lwipdev.dhcpstatus != 0XFF))
        {
            lwip_dhcp_process_handle();  //DHCP����
        }
    }
#endif
}

