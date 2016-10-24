#include "bigiot_port.h"


bool BigIotPort::connect(const char *remote_ip, uint32_t remote_port, uint16_t local_port)
{
    return WIFI_TCP::connect((char *)remote_ip,remote_port,local_port);
}
bool BigIotPort::connected()
{
    return WIFI_TCP::connected();
}
uint16_t BigIotPort::available()
{
    return WIFI_TCP::available();
}

uint16_t BigIotPort::read_until(uint8_t *buf,char ch)
{
    uint16_t len=0;
    if(available()){
        len = WIFI_TCP::read_until(buf,ch);
    }
    return len;
}
uint16_t BigIotPort::send(uint8_t *buf,uint16_t length)
{
    return WIFI_TCP::send(buf,length);

}
