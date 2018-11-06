#include "ebox.h"
#include "enc28j60.h"
#include "stdio.h"

#include "eth_port.h"
Enc28j60 eth(&spi1, &PA4, &PE1);


void ebox_printf(const char *fmt, ...)
{

    uart1.printf(fmt);
}
void ENC28J60_Init(uint8_t *mac_addr)
{
    eth.begin(2);
    eth.init(mac_addr);
}
void ENC28J60_InitSend(uint16_t len)
{
    eth.init_send(len);
}
void    ENC28J60_WriteBuf(uint8_t *p, uint16_t len)
{
    eth.write_buf(p, len);
}

void    ENC28J60_BeginSend()
{
    eth.begin_send();
}

uint16_t    ENC28J60_GetPacketLength()
{
    return eth.get_packet_length();

}

void    ENC28J60_ReadBuf(uint8_t *p, uint16_t len)
{
    eth.read_buf(p, len);

}
void    ENC28J60_EndReceive(void)
{
    eth.end_receive();
}

