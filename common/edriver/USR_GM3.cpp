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
    link->mode(INPUT);
    linka->mode(INPUT);
    linkb->mode(INPUT);
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
LinkState_t UsrGm3::link_state()
{
    return (LinkState_t)link->read();
}
LinkState_t UsrGm3::linka_state()
{
    return (LinkState_t)linka->read();

}
LinkState_t UsrGm3::linkb_state()
{
    return (LinkState_t)linkb->read();

}
void UsrGm3::send(char *buf,uint16_t len)
{
    uart->write(buf,len);


}

void UsrGm3::at_cmd_go()
{
    gm3_mode = CMD;
    at_3plus(); 
}

uint16_t UsrGm3::at_cmd_send(char *cmd,char *result)
{
    uint16_t result_len;
    gm3_mode = CMD;
    
    uart->printf("%s",cmd);
    result_len = wait_end(2000);
    if(result_len != 0)
    {
        memcpy(result,cmd_buf,cmd_count+1);//+1,保证字符串结束符被正确拷贝
    }
    clear_cmd_buf();
        gm3_mode = NET;

    return cmd_count;

}
uint16_t UsrGm3::at_cmd_send(const char *cmd,char *result)
{
    return at_cmd_send((char *)cmd,result);
}
uint8_t UsrGm3::at_3plus()
{
    uint8_t ret;
    gm3_mode = CMD;
    delay_ms(301);
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
//////////////////////////管理指令///////////////////////////////////////////
void UsrGm3::h(){}
void UsrGm3::z(){}
    
void UsrGm3::q_e()
{
    char temp[64];
    uint16_t len;
    len = at_cmd_send("AT+E?\r\n",temp);
    GM3_DEBUG(temp);
}

void UsrGm3::entm()
{
    char result[64];
    uint16_t len;
    len = at_cmd_send((char*)"AT+ENTM\r\n",result);
    gm3_mode = NET;
    GM3_DEBUG("entm\r\n");
    GM3_DEBUG(result);    

}
void UsrGm3::q_wkmod(){}
void UsrGm3::q_calen(){}
void UsrGm3::q_naten(){}
void UsrGm3::q_uaten(){}
void UsrGm3::q_cmdpw(){}
void UsrGm3::q_cachen(){}
void UsrGm3::q_stmsg(){}
uint16_t UsrGm3::q_rstim()
{
    char result[64];
    uint16_t ret;
    at_cmd_send("AT+RSTIM?\r\n",result);
    GM3_DEBUG(result);
    get_str(result,":",1,"\r",2,result);
    ret = ATOI(result,10);
    return ret;
}
void UsrGm3::q_sleep()
{
    
}
uint16_t UsrGm3::q_sleeptim()
{
    char result[64];
    uint16_t ret;
    at_cmd_send("AT+SLEEPTIM?\r\n",result);
    GM3_DEBUG(result);
    get_str(result,":",1,"\r",2,result);
    ret = ATOI(result,10);
    return ret;
} 
    
void UsrGm3::set_e(uint8_t enable)
{

}
    
void UsrGm3::set_wkmod(GM3Mode_t mode)
{
    char result[64];
    switch((uint8_t)mode)
    {
        case CMD:
            at_cmd_send("AT+WKMOD=\"CMD\"\r\n",result);
            break;
        case NET:
            at_cmd_send("AT+WKMOD=\"NET\"\r\n",result);
            break;
        case SMS:
            at_cmd_send("AT+WKMOD=\"SMS\"\r\n",result);
            break;
        case HTTPD:
            at_cmd_send("AT+WKMOD=\"HTTPD\"\r\n",result);
            break;
    }
    gm3_mode = mode;
    gm3_new_mode  = mode;
    GM3_DEBUG("WKMODE SET\r\n");
    GM3_DEBUG(result);
}

void UsrGm3::set_calen(uint8_t enable){}
void UsrGm3::set_naten(uint8_t enable){}
void UsrGm3::set_uaten(uint8_t enable){}
void UsrGm3::set_cmdpw(char *pwd){}
void UsrGm3::set_cachen(uint8_t enable){}
void UsrGm3::set_stmsg(char *msg){}
void UsrGm3::set_rstim(uint16_t time)
{
    char result[64];
    uint16_t ret;
    sprintf(result,"AT+RSTIM=%d\r\n",time);
    at_cmd_send(result,result);
    GM3_DEBUG(result);
}
void UsrGm3::set_sleep(uint8_t enable)
{
    char result[64];
    uint16_t ret;
    if(enable == 1)
        at_cmd_send("AT+SLEEP=\"on\"\r\n",result);
    else
        at_cmd_send("AT+SLEEP=\"off\"\r\n",result);
    GM3_DEBUG(result);
}
void UsrGm3::set_sleeptim(uint16_t time)
{
    char result[64];
    uint16_t ret;
    sprintf(result,"AT+SLEEPTIM=%d\r\n",time);
    at_cmd_send(result,result);
    GM3_DEBUG(result);
}     
/////////////////////////////////////////////////////////////////////
    
    
//////////////////配置参数指令///////////////////////////////////////        
void UsrGm3::save()
{
    char result[64];
    uint16_t len;
    len = at_cmd_send((char*)"AT+S\r\n",result);
    gm3_mode = gm3_new_mode;
    GM3_DEBUG("save\r\n");
    GM3_DEBUG(result);
}
void UsrGm3::reld(){}
void UsrGm3::clear(){}
void UsrGm3::cfgtf(){}
/////////////////////////////////////////////////////////////////////

//////////////////信息查询指令///////////////////////////////////////        
void UsrGm3::q_version()
{
    char temp[64];
    uint16_t len;
    len = at_cmd_send("AT+VER?\r\n",temp);
    GM3_DEBUG(temp);
}
void UsrGm3::q_sn(){}
void UsrGm3::q_iccid(){}
void UsrGm3::q_imei(){}
void UsrGm3::q_cnum(){}
/////////////////////////////////////////////////////////////////////
    
//////////////////串口参数指令///////////////////////////////////////        
void UsrGm3::q_uart(){}
void UsrGm3::q_rfcen(){}
void UsrGm3::set_uart(uint32_t baud,uint8_t parity,uint8_t data,uint8_t bits,uint8_t flow_ctr){}
void UsrGm3::set_rfcen(uint8_t enable){}
/////////////////////////////////////////////////////////////////////

//////////////////网络指令///////////////////////////////////////        
           
void UsrGm3::q_apn(){}
void UsrGm3::q_socka(){}
void UsrGm3::q_sockb(){}
void UsrGm3::q_sockaen(){}
void UsrGm3::q_sockben(){}
void UsrGm3::q_sockaals(){}
void UsrGm3::q_sockbsl(){}
void UsrGm3::q_sockalk()
{
    char result[64];
    uint8_t ret = 0;
    char temp[5];
    at_cmd_send("AT+SOCKALK?\r\n",result);
    GM3_DEBUG("SOCKALK  Q\r\n");
    GM3_DEBUG(result); 
    //get_str(result,"\"",1,"\"",2,temp);
//    if(memcmp(temp,"on",2) == 0)
//    {
//        ret = 1;
//    }
//    else
//        ret  = 0;
//    return ret;  
}
void UsrGm3::q_sockblk(){}
void UsrGm3::q_sockiden(){}
void UsrGm3::set_apn(){}
    
void UsrGm3::set_socka(char* type,char *host,char *port)
{
    char result[64];
    sprintf(result,"AT+SOCKA=\"%s\",\"%s\",%s\r\n",type,host,port);
    at_cmd_send(result,result);
    GM3_DEBUG("TCP\r\n");
    GM3_DEBUG(result);
}

void UsrGm3::set_sockb(char* type, char *host,char *port){}

void UsrGm3::set_sockaen(uint8_t enable)
{
    char result[64];
    if(enable == 1)
        at_cmd_send("AT+SOCKAEN=\"on\"\r\n",result);
    else
        at_cmd_send("AT+SOCKAEN=\"off\"\r\n",result);
    GM3_DEBUG("SA ENABLE\r\n");
    GM3_DEBUG(result);

}
void UsrGm3::set_sockben(uint8_t enable){}
void UsrGm3::set_sockaals(){}
void UsrGm3::set_sockbsl(){}
void UsrGm3::set_sockiden(){}
/////////////////////////////////////////////////////////////////////



//////////////////////////心跳包指令///////////////////////////////////////////
void UsrGm3::set_hearten(uint8_t enable)
{
    
    char result[64];
    uint16_t len;
    if(enable == 1)
        len = at_cmd_send((char*)"AT+HEARTEN=\"on\"\r\n",result);
    else
        len = at_cmd_send((char*)"AT+HEARTEN=\"off\"\r\n",result);
    GM3_DEBUG("hearten  set\r\n");
    GM3_DEBUG(result);    
}
void UsrGm3::set_heartdt(char *data)
{
    char result[64];
    char temp[64];
    char *p = temp;
    while(*data != '\0')
    {
        sprintf(p,"%x",*data++);
        p++;
        p++;        
    }
    sprintf(result,"AT+HEARTDT=\"%s\"\r\n",temp);
    at_cmd_send(result,result);
    GM3_DEBUG("heartdt  set\r\n");
    GM3_DEBUG(result);    

}
void UsrGm3::set_hearttp(HEARTTP_t type)
{
    char result[64];
    if(type == H_COM)
        at_cmd_send("AT+HEARTTP=\"COM\"\r\n",result);
    else
        at_cmd_send("AT+HEARTTP=\"NET\"\r\n",result);
    GM3_DEBUG("hearttp  set\r\n");
    GM3_DEBUG(result);    

}
void UsrGm3::set_hearttm(uint8_t time)
{
    char result[64];
    char temp[64];
    sprintf(temp,"AT+HEARTTM=%d\r\n",time);
    at_cmd_send(temp,result);
    GM3_DEBUG("hearttm  set\r\n");
    GM3_DEBUG(result); 

}
uint8_t UsrGm3::q_hearten()
{
    char result[64];
    uint8_t ret = 0;
    char temp[5];
    at_cmd_send("AT+HEARTEN?\r\n",result);
    GM3_DEBUG("hearten  Q\r\n");
    GM3_DEBUG(result); 
    get_str(result,"\"",1,"\"",2,temp);
    if(memcmp(temp,"on",2) == 0)
    {
        ret = 1;
    }
    else
        ret  = 0;
    return ret;    
}
void UsrGm3::q_heartdt(char *p)
{
    char result[64];
    at_cmd_send("AT+HEARTDT?\r\n",result);
    GM3_DEBUG("HEARTDT Q\r\n");
    GM3_DEBUG(result);
    get_str(result,"\"",1,"\"",2,p);
}
HEARTTP_t UsrGm3::q_hearttp()
{
    char result[64];
    HEARTTP_t ret;
    at_cmd_send("AT+HEARTTP?\r\n",result);
    GM3_DEBUG("HEARTDT Q\r\n");
    GM3_DEBUG(result);
    return ret;
}
uint8_t UsrGm3::q_hearttm()
{
    char result[64];
    at_cmd_send("AT+HEARTTM?\r\n",result);
    GM3_DEBUG("HEARTTM Q\r\n");
    GM3_DEBUG(result);
}
//////////////////////////////////////////////////////////////////////

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
uint16_t UsrGm3::wait_end( uint16_t timeout)
{
    uint16_t ret;
    uint64_t time = millis();
    while(1)
    {
        if(cmd_count != 0)
        {
            if(millis() - last_rx_event_time > 5)
            {
                ret = cmd_count;
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


