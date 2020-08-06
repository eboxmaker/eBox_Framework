/*
    ModbusIP.h - Header for Modbus IP Library
    Copyright (C) 2015 André Sarmento Barbosa
*/
#include <Modbus.h>
#include "Ethernet3.h"

#ifndef MODBUSIP_H
#define MODBUSIP_H

#define MODBUSIP_PORT 	  502
#define MODBUSIP_MAXFRAME 200

#define TCP_KEEP_ALIVE 1
typedef void (*MBcallbackPredeal)(byte *_frame);

class ModbusIP : public Modbus
{
private:
    EthernetServer _server;
    byte _MBAP[7];

public:
    ModbusIP();
    void config(uint8_t *mac);
    void config(uint8_t *mac, IPAddress ip);
    void config(uint8_t *mac, IPAddress ip, IPAddress dns);
    void config(uint8_t *mac, IPAddress ip, IPAddress dns, IPAddress gateway);
    void config(uint8_t *mac, IPAddress ip, IPAddress dns, IPAddress gateway, IPAddress subnet);
    bool task();
    void attach(MBcallbackPredeal cb){ cb_pre_deal = cb;}
       
public:
    bool data_com;

private:
    MBcallbackPredeal cb_pre_deal;
//    EthernetClient mclient ;
};

#endif //MODBUSIP_H

