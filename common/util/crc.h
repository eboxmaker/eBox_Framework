#ifndef __CRC_H
#define __CRC_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "ebox_core.h"

uint16_t crc16( uint8_t * pucFrame, uint16_t usLen );
uint8_t  crc8 (uint8_t *p,uint16_t n);
     
#ifdef __cplusplus
}
#endif

#endif