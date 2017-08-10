#include "USR_GM3.h"

#if 1
#define GM3_DEBUG(...) uart1.printf("\r\n[GM3]"),uart1.printf(__VA_ARGS__)
#else
#define  GM3_DEBUG(...)
#endif

uint16_t cmd_count = 0;
uint16_t net_count = 0;
void UsrGm3::rx_event()
{
    char c;
    c = uart->read();
   // uart1.write(c);

    last_rx_event_time = millis();
    switch((uint8_t)gm3_mode)
    {
        case INIT:
        case CMD:
            cmd_buf[cmd_count] = c;
            cmd_count++;
            break;
        case NET:
            pNetBuf.write(c);
            break;

    }
}
void UsrGm3::begin(uint32_t baud_rate)
{
    uart->begin(baud_rate);
    uart->attach(this,&UsrGm3::rx_event,RxIrq);
    rst->mode(OUTPUT_PP);
    hard_reset();
    memset(cmd_buf,0,256);
    cmd_count = 0;
    if(wait("USR-GM3 V2.1",5000) == 1)
        GM3_DEBUG("init ok\r\n");
    clear_cmd_buf();
    pNetBuf.begin(net_buf,NET_BUF_SIZE);
}

void UsrGm3::hard_reset()
{
    rst->reset();
    delay_ms(10);
    rst->set();
}
void UsrGm3::soft_reset()
{

}

void UsrGm3::at_cmd_go()
{
    gm3_mode = CMD;
    at_3plus(); 
}
uint8_t UsrGm3::at_3plus()
{
    uint8_t ret;
    delay_ms(201);
    uart->write("+++");
    if(wait("a",1000) == 1)
    {
        clear_cmd_buf();
        uart->write("a");
        if(wait("+ok",1000) == 1)
        {
            clear_cmd_buf();
            GM3_DEBUG("enter CMD\r\n");
            ret = 1;
        }
        else
            ret = 0;
    }
    else
        ret = 0;
    return ret;

}

void UsrGm3::set_wkmod(GM3Mode_t mode)
{

    switch((uint8_t)mode)
    {
        case CMD:
            uart->write("AT+WKMOD=\"CMD\"\r\n");
            break;
        case NET:
            uart->write("AT+WKMOD=\"NET\"\r\n");
            break;
        case SMS:
            uart->write("AT+WKMOD=\"SMS\"\r\n");
            break;
        case HTTPD:
            uart->write("AT+WKMOD=\"HTTPD\"\r\n");
            break;
    }
    gm3_new_mode  = mode;
    if(wait(1000) == 1)
    {
        GM3_DEBUG("WKMODE SET\r\n");
        GM3_DEBUG(cmd_buf);
    }
    clear_cmd_buf();
}

void UsrGm3::q_version()
{
    uart->write("AT+VER?\r\n");
    if(wait(1000) == 1)
    {
        GM3_DEBUG(cmd_buf);
    }
    clear_cmd_buf();
}
void UsrGm3::q_e()
{
    uart->write("AT+E?\r\n");
    if(wait(1000) == 1)
    {
        GM3_DEBUG(cmd_buf);
    }
    clear_cmd_buf();
}

void UsrGm3::set_sockaen(uint8_t enable)
{
    if(enable == 1)
        uart->write("AT+SOCKAEN=\"on\"\r\n");
    else
        uart->write("AT+SOCKAEN=\"off\"\r\n");
    if(wait(1000) == 1)
    {
        GM3_DEBUG("SA ENABLE\r\n");
        GM3_DEBUG(cmd_buf);
    }
    clear_cmd_buf();
}
void UsrGm3::set_socka(char* type,char *host,char *port)
{
    uart->printf("AT+SOCKA=\"TCP\",\"%s\",%s\r\n",host,port);
    if(wait(1000) == 1)
    {
        GM3_DEBUG("TCP\r\n");
        GM3_DEBUG(cmd_buf);
    }
    clear_cmd_buf();
}
void UsrGm3::save()
{
    uart->write("AT+S\r\n");
    if(wait(1000) == 1)
    {
        gm3_mode = gm3_new_mode;
        GM3_DEBUG("SAVE\r\n");
        GM3_DEBUG(cmd_buf);

    }
    clear_cmd_buf();
}

//心跳包指令
void UsrGm3::set_hearten(uint8_t enable)
{
    if(enable == 1)
        uart->write("AT+HEARTEN=\"on\"\r\n");
    else
        uart->write("AT+HEARTEN=\"off\"\r\n");

    if(wait(1000) == 1)
    {
        GM3_DEBUG("HEARTEN set\r\n");
        GM3_DEBUG(cmd_buf);
    }
    clear_cmd_buf();
}
void UsrGm3::set_heartdt(char *data)
{
    char temp[41];
    char *p = temp;
    while(*data != '\0')
    {
        sprintf(p,"%x",*data++);
        p++;
        p++;        
    }
    uart->printf("AT+HEARTDT=\"%s\"\r\n",temp);

    if(wait(1000) == 1)
    {
        GM3_DEBUG("HEARTdt set\r\n");
        GM3_DEBUG(cmd_buf);
    }
    clear_cmd_buf();
}
void UsrGm3::set_hearttp(HEARTTP_t type)
{
    if(type == H_COM)
        uart->write("AT+HEARTTP=\"COM\"\r\n");
    else
        uart->write("AT+HEARTTP=\"NET\"\r\n");

    if(wait(1000) == 1)
    {
        GM3_DEBUG("HEARTTP set\r\n");
        GM3_DEBUG(cmd_buf);
    }
    clear_cmd_buf();
}
void UsrGm3::set_hearttm(uint8_t time)
{

    uart->printf("AT+HEARTTM=%d\r\n",time);

    if(wait(1000) == 1)
    {
        GM3_DEBUG("HEARTTM set\r\n");
        GM3_DEBUG(cmd_buf);
    }
    clear_cmd_buf();
}
uint8_t UsrGm3::q_hearten()
{
    uint8_t ret = 0;
    char temp[5];
    uart->write("AT+HEARTEN?\r\n");
    if(wait(1000) == 1)
    {
        GM3_DEBUG("hearten  Q\r\n");
        GM3_DEBUG(cmd_buf);
        get_str(cmd_buf,"\"",1,"\"",2,temp);
        if(memcmp(temp,"on",2) == 0)
        {
            ret = 1;
        }
        else
            ret  = 0;
    }
    clear_cmd_buf(); 
    return ret;    
}
void UsrGm3::q_heartdt(char *p)
{
    uart->write("AT+HEARTDT?\r\n");
    if(wait(1000) == 1)
    {
        GM3_DEBUG("HEARTDT Q\r\n");
        GM3_DEBUG(cmd_buf);
        get_str(cmd_buf,"\"",1,"\"",2,p);

    }
    clear_cmd_buf();   
}
HEARTTP_t UsrGm3::q_hearttp()
{
    HEARTTP_t ret;
    uart->write("AT+HEARTTP?\r\n");
    if(wait(1000) == 1)
    {
        GM3_DEBUG("HEARTDT Q\r\n");
        GM3_DEBUG(cmd_buf);
    }
    clear_cmd_buf();   
    return ret;
}
uint8_t UsrGm3::q_hearttm()
{
    uart->write("AT+HEARTTM?\r\n");
    if(wait(1000) == 1)
    {
        GM3_DEBUG("HEARTTM Q\r\n");
        GM3_DEBUG(cmd_buf);
    }
    clear_cmd_buf();   
}


uint16_t UsrGm3::wait(const char *s, uint16_t timeout)
{
    uint16_t index;
    uint16_t len;
    uint8_t ret;
    uint64_t time = millis();
    while(1)
    {
        if(cmd_count != 0)
        {
            if(millis() - last_rx_event_time > 3)//如果开始接收字符串，且中断时间超过3ms，认为接收字符串结束
            {
                len = find_str((uint8_t *)cmd_buf,(uint8_t *)s,1,index);
                if(len == 0)
                    ret = 0;
                else
                    ret = 1;
                break;
            }
        }
        
        if(millis() - time > timeout)
        {
            ret = 0;
            break;
        }
    }
    return ret;
}
uint16_t UsrGm3::wait( uint16_t timeout)
{
    uint16_t index;
    uint16_t len;
    uint8_t ret;
    uint64_t time = millis();
    while(1)
    {
        if(cmd_count != 0)
        {
            if(millis() - last_rx_event_time > 5)
            {
                len = find_str((uint8_t *)cmd_buf,(uint8_t *)"OK",1,index);
                if(len == 0)
                    ret = 0;
                else
                    ret = 1;
                break;
            }
        }
        
        if(millis() - time > timeout)
        {
            ret = 0;
            break;
        }
    }
    return ret;
}
void UsrGm3::clear_cmd_buf()
{
    cmd_count = 0 ;
    memset(cmd_buf,0,256);
}


