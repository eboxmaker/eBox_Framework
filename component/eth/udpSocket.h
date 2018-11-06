#ifndef __UDPSocket_H
#define __UDPSocket_H
#include "ebox.h"
#include "../eth/ether.h"

class UDPSocket
{
public:

    /**
     * Construct a UDP socket
     * The local port number will be sent to a random port number
     *
     * @param ether The Ethernet interface to attach the socket to
     */
    UDPSocket(Ether &ether) {};
};

#endif
