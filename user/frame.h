#ifndef __FRAME_H
#define __FRAME_H


#include "ebox.h"
typedef enum
{
    MASTER = 0X01,
    SLAVE = 0X0A,
}MasterSlaveFlag_t;
typedef enum
{
    MODIFY_DEV_ID = 1,
    MODIFY_DEV_NUM = 2,
    QUARRY_DATA = 3,
    MODIFY_IP = 4,
    HEARTBEAT = 5,
    
}PacketCmd_t;
typedef struct {
    
    uint8_t     head;
    MasterSlaveFlag_t master_or_slave;
    uint16_t    addr;
    PacketCmd_t cmd;
    char        *para;
    uint16_t    crc;
    uint8_t     tail;
   
    
    uint16_t    str_len;//
}Packet_t;

typedef struct
{
    uint8_t ip[4];
    uint16_t port;
    char str[13];
    uint8_t str_len;
}GateIpAddrs_t;

typedef struct
{
    uint8_t num;
    char str[3];
    uint8_t str_len;
}GateDevNum_t;

typedef struct
{
    uint8_t id;
    char str[5];
    uint8_t str_len;
}GateId_t;

typedef struct
{
    uint8_t id;
    char str[5];
    uint8_t str_len;
}QuarryDevId_t;

void Packet_initializer(Packet_t *packet,uint16_t addr,PacketCmd_t cmd,char *para,uint16_t para_len);
uint16_t packet_serialize(uint8_t *buf,uint16_t len,Packet_t *packet);


uint16_t read_frame(char ch,uint8_t *buf);
uint16_t read_crc(uint8_t *buf,uint16_t len);
uint8_t check_crc(uint8_t *buf,uint16_t len);
PacketCmd_t get_buf_cmd(uint8_t *buf,uint16_t len);



void gate_ip_addrs_initializer(GateIpAddrs_t *ip,char *p,uint16_t port);
void gate_dev_num_initializer(GateDevNum_t *gate_dev_num,uint8_t num);
void gate_id_initializer(GateId_t *gate_id,uint16_t id);
void dev_id_initializer(QuarryDevId_t *quarry_data_id,uint16_t id);

void get_gate_ip_addrs(GateIpAddrs_t *ip,uint8_t *buf,uint16_t len);
void get_gate_dev_num(GateDevNum_t *dev_num,uint8_t *buf,uint16_t len);
void get_gate_id(GateId_t *dev_id,uint8_t *buf,uint16_t len);
void get_dev_id(QuarryDevId_t *dev_id,uint8_t *buf,uint16_t len);

#endif
