#ifndef __GATEWAY_H
#define __GATEWAY_H

#include "ebox.h"
typedef struct
{
    uint16_t id;
    uint8_t  host_ip[4];
    uint16_t host_port;
    uint8_t  dev_num;

}GateWay_t;
typedef enum{
    DEHUMIDIFIER    = 1,
    THERMOSTAT      = 2,
    DISPLAYER       = 3,
}DevType_t;

typedef struct
{
    DevType_t type;
    uint8_t     humidity;
    int8_t      temperature1;
    int8_t      temperature2;
    struct
    {
        uint8_t fan:1;
        uint8_t cooler:1;
        uint8_t by_hand:1;
        uint8_t smoke:1;
        uint8_t bit3:1;
        uint8_t bit2:1;
        uint8_t bit1:1;
        uint8_t bit0:1;
    }mode;
    uint8_t alert_hum;
    uint8_t alert_temp;
}Dev_t;

typedef struct
{

}Dehumidifier_t;

typedef struct
{

}Thermostat_t;

typedef struct
{

}Displayer_t;
#endif

