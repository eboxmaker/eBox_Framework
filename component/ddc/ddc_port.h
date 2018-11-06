#ifndef __DDC_PORT_H
#define __DDC_PORT_H

#ifdef __cplusplus
extern "C" {
#endif

//#include "stdio.h"
#include "ebox_cfun.h"

#define DDC_MAX_FRAME_LENGTH   128
#define DDC_MAX_PAYLOAD_LENGTH  (DDC_MAX_FRAME_LENGTH - 10)


extern struct ebox_fifo *fifo_recv;


void ddc_init(void);
void ddc_get_char(uint8_t ch);
uint16_t ddc_send_char(uint8_t ch);
uint16_t ddc_send_buf(uint8_t *buf, uint16_t buflen);

#ifdef __cplusplus
}
#endif

#endif

