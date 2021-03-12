
#include "mac_link.h"

MACLink::MACLink(PN532 &pn532)
{
    _pn532 = &pn532;
}

int8_t MACLink::activateAsTarget()
{
    return _pn532->tgInitAsTarget();
}

int8_t MACLink::writePDU(const uint8_t *buf, uint16_t len)
{
    if (!_pn532->tgSetData(buf, len)) {
        return -1;
    } else {
        return 0;
    }
}

int8_t MACLink::readPDU(uint8_t *buf, uint16_t len)
{
    int16_t status = _pn532->tgGetData(buf, len);
    if (status < 0) {
        return status;
    }
    
    uint16_t length = status;
    if (length < 2) {
        return -6;
    }
    
    return 0;  // return length instread ?
}
