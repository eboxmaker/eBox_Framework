

#ifndef __MAC_LINK_H__
#define __MAC_LINK_H__

#include "PN532.h"

class MACLink {
public:
    MACLink(PN532 &pn532);
    
    int8_t activateAsTarget();
    int8_t writePDU(const uint8_t *buf, uint16_t len);
    int8_t readPDU(uint8_t *buf, uint16_t len);
    
private:
    PN532 *_pn532;
};

#endif
