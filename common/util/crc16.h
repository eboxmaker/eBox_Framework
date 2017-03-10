#ifndef __CRC16_H
#define __CRC16_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "ebox_core.h"

uint16_t crc16( uint8_t * pucFrame, uint16_t usLen );
     
#ifdef __cplusplus
}
#endif

#endif