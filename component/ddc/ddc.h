#ifndef __DDC_H
#define __DDC_H


#ifdef __cplusplus
 extern "C" {
#endif
     
     
#include "stdio.h"
#include "mcu_core.h"
#include "ddc_port.h"
#include "ddc_list.h"
     
typedef void (*DdcCallBack_t)(uint8_t *ptr, uint16_t len );

typedef enum 
{
    UNLOCK,
    LOCK
}DdcLock_t;

typedef enum 
{
    DDC_NoAck = 0,
    DDC_Ack,
}DdcAck_t;


typedef struct
{
    uint8_t     head[2];
    DataU16_t   id;
    uint8_t     ch;
    uint8_t     ack;
    DataU16_t   payload_len;
    uint8_t     payload[DDC_MAX_PAYLOAD_LENGTH];
    DataU16_t   crc;
}DdcFrame_t;

#define PAY2FRAME_LEN(x) (x + 10)

//�û�API�ӿ�
void        ddc_loop(void);
uint16_t    ddc_make_frame(uint8_t *dst,uint8_t *data,uint16_t data_len,DdcAck_t ack,uint8_t ch );
uint16_t    ddc_make_ack_frame(uint8_t *dst,uint16_t id);
void        ddc_add_to_list(uint8_t *buf);
uint16_t    ddc_frame_to_buf(uint8_t *dst,DdcFrame_t *frame);
uint16_t    ddc_buf_to_frame(DdcFrame_t *frame,uint8_t *src);
void        ddc_attach_chx(uint8_t ch, DdcCallBack_t callback);
void        ddc_send_list(DdcNode_t *p);




//�ڲ��ӿ�
void        ddc_recv_process(void);
void        ddc_retry(DdcNode_t *p);
void        ddc_analyze_frame(DdcFrame_t *frame);
uint16_t    get_frame_payload_len(uint8_t *frame);
uint16_t    get_frame_id(uint8_t *frame);
uint8_t     get_frame_ack(uint8_t *frame);
uint8_t     get_frame_ch(uint8_t *frame);
void        print_frame(uint8_t *frame);
void        print_list(DdcNode_t *list);
void        ddc_check_ack_list(void);

#ifdef __cplusplus
    }
#endif

#endif 


 