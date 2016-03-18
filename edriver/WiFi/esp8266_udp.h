
/*
 * file   : esp8266_udp.h
 * author : shentq&link
 * version: V1.0
 * date   : 2016/03/18
 *
 * esp8266 library for eBOX.
 *
 * Copyright Notice
 *    No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq and link.
 * Disclaimer
 *    This specification is preliminary and is subject to change at any time without notice. shentq and link assumes no responsibility for any errors contained herein.
 *
 *
 * Modification History:
 * -Link&shentq             - Version 0.1 (2016/3/18)
 */



#ifndef __ESP8266_H
#define __ESP8266_H
#include "esp8266.h"

class WIFI_UDP
{
public:
    WIFI_UDP(ESP8266 *wifi);
    bool        begin();

    bool        connect(char *remote_ip, uint32_t remote_port, uint32_t local_port);
    uint16_t    send(uint8_t *buf, uint16_t len);
    uint16_t    available();
    uint16_t    read(uint8_t *buf);

    bool        connect(uint8_t mux_id, char *remote_ip, uint32_t remote_port, uint32_t local_port);
    uint16_t    send(uint8_t mux_id, uint8_t *buf, uint16_t len);
    uint16_t    read(uint8_t *mux_id, uint8_t *buf);

    uint8_t     read_onebyte(void);

private:
    ESP8266 *wifi;




};



#endif
