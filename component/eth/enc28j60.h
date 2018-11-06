#ifndef __EtherEnc28j60_H
#define __EtherEnc28j60_H
#include "ebox.h"

#include "ether.h"

class EtherEnc28j60 : public Ether
{
public:
    /**
     * Constructor that uses the default hardware SPI pins
     * @param cs the Arduino Chip Select / Slave Select pin (default 10 on Uno)
     */
    EtherEnc28j60(int8_t id)
    {};

};

#endif
