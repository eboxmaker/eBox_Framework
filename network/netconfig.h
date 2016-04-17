#ifndef __NETCONFIG_H
#define __NETCONFIG_H
#include "stm32f10x.h"

class IPCONFIG
{
    public:
        uint8_t mac[6];
        uint8_t ip[4];
        uint8_t subnet[4];
        uint8_t gw[4];
        uint8_t dns[4];    
};

#endif
