#include "frame.h"

static void write_direct(unsigned char **pptr,unsigned char data)
{
    **pptr = data;
    (*pptr)++;
}
static void write_direct_nbytes(unsigned char **pptr,char *data,uint16_t len)
{
    for(int i = 0; i < len; i++)
    {
        **pptr = *data++;
        (*pptr)++;
    }
}
static void write_u8(unsigned char **pptr,unsigned char data)
{
    char temp[3];
    sprintf(temp,"%02X",data);
    **pptr = temp[0];
    (*pptr)++;
    **pptr = temp[1];
    (*pptr)++;
}
static void write_u16(unsigned char **pptr,uint16_t data)
{
    char temp[5];
    sprintf(temp,"%04X",data);
    **pptr = temp[0];
    (*pptr)++;
    **pptr = temp[1];
    (*pptr)++;
    **pptr = temp[2];
    (*pptr)++;
    **pptr = temp[3];
    (*pptr)++;
}





void Packet_initializer(Packet_t *packet,uint16_t addr,PacketCmd_t cmd,char *para,uint16_t para_len)
{
    packet->head = '%';
    packet->master_or_slave = SLAVE;
    packet->addr = addr;
    packet->cmd = cmd;
    packet->para = para;
    packet->tail = '*';
    packet->str_len = para_len + 14;
}

uint16_t packet_serialize(uint8_t *buf,uint16_t len,Packet_t *packet)
{
    uint8_t *ptr = buf;
    write_direct(&ptr,packet->head);
    write_u8(&ptr,packet->master_or_slave);
    write_u16(&ptr,packet->addr);
    write_u8(&ptr,packet->cmd);
    if(packet->para != 0)
    {
        write_direct_nbytes(&ptr,packet->para,packet->str_len - 14);
    }
    packet->crc = crc16(buf,packet->str_len-5);
    write_u16(&ptr,packet->crc);
    write_direct(&ptr,packet->tail);

}
typedef enum
{
     NEED_HEAD = 0,
     NEED_DATA = 1,
     NEED_TAIL = 2,
}FrameState_t;
static FrameState_t state;
uint16_t read_frame(char ch,uint8_t *buf)
{
    static uint16_t counter = 0;
    uint8_t ret = 0;
    uart1.print(ch);
    switch((uint8_t)state)
    {
        case NEED_HEAD:
            if(ch == '%')
            {
                counter = 0;
                memset(buf,0,100);
                buf[counter] = ch;
                counter++;
                state = NEED_DATA;
            }
            break;
        case NEED_DATA:
            buf[counter] = ch;
            counter++;
            if(ch == '*')
            {
                state = NEED_HEAD;
                ret = counter;
            }
            break;
        default :
            ret = 0;
            break;
    }
    return ret;
}



                
                
uint16_t read_crc(uint8_t *buf,uint16_t len)
{
    uint8_t temp[5];
    uint16_t crc;
    temp[0] = buf[len - 5];
    temp[1] = buf[len - 4];
    temp[2] = buf[len - 3];
    temp[3] = buf[len - 2];
    temp[4] = 0;
    crc = ATOI((char *)temp,16);
    return crc;
}

uint8_t check_crc(uint8_t *buf,uint16_t len)
{
    if(crc16(buf,len-5) == read_crc(buf,len))
    {
        return 1;
    }
    else
        return 0;
}
PacketCmd_t get_buf_cmd(uint8_t *buf,uint16_t len)
{
    uint8_t temp[4];
    uint8_t cmd;
    temp[0] = buf[7];
    temp[1] = buf[8];
    temp[2] = 0;
    cmd = ATOI((char *)temp,16);
    return (PacketCmd_t)cmd;
}



void gate_ip_addrs_initializer(GateIpAddrs_t *ip,char *p,uint16_t port)
{
    memcpy(ip->ip,p,4);
    sprintf(ip->str,"%02X%02X%02X%02X%04X",p[0],p[1],p[2],p[3],port);
    ip->str_len = 12;
}
void gate_dev_num_initializer(GateDevNum_t *dev_num,uint8_t num)
{
    dev_num->num = num;
    sprintf(dev_num->str,"%02X",num);
    dev_num->str_len = 2;
}
void gate_id_initializer(GateId_t *dev_id,uint16_t id)
{
    dev_id->id = id;
    sprintf(dev_id->str,"%04X",id);
    dev_id->str_len = 4;
}

void get_gate_ip_addrs(GateIpAddrs_t *gate_ip,uint8_t *buf,uint16_t len)
{
    for(int i=0; i < 12; i++)
    {
        gate_ip->str[i] = buf[i + 9];
    }
    
    char temp[5];
    
    for(int i=0; i < 4; i++)
    {
        temp[0] = gate_ip->str[i*2+0];
        temp[1] = gate_ip->str[i*2+1];
        gate_ip->ip[i] = ATOI(temp,16);
    }
    temp[0] = gate_ip->str[8];
    temp[1] = gate_ip->str[9]; 
    temp[2] = gate_ip->str[10];
    temp[3] = gate_ip->str[11];
    gate_ip->port = ATOI(temp,16); 
    
    gate_ip->str_len = 12;
}
void get_gate_dev_num(GateDevNum_t *gate_dev_num,uint8_t *buf,uint16_t len)
{
    char temp[3];
    for(int i=0; i < 2; i++)
    {
        gate_dev_num->str[i] = buf[i + 9];
        temp[i] = buf[i + 9];
    }
    temp[2] = 0;
    gate_dev_num->num = ATOI(temp,16);
    gate_dev_num->str_len = 2;
}
void get_gate_id(GateId_t *gate_id,uint8_t *buf,uint16_t len)
{
    char temp[5];
    for(int i=0; i < 4; i++)
    {
        gate_id->str[i] = buf[i + 9];
        temp[i] = buf[i + 9];
    }
    temp[4] = 0;

    gate_id->id = ATOI(temp,16);
    gate_id->str_len = 4;
}
void get_dev_id(QuarryDevId_t *dev_id,uint8_t *buf,uint16_t len)
{
    char temp[5];
    for(int i=0; i < 4; i++)
    {
        dev_id->str[i] = buf[i + 9];
        temp[i] = buf[i + 9];
    }
    temp[4] = 0;

    dev_id->id = ATOI(temp,16);
    dev_id->str_len = 4;
}

