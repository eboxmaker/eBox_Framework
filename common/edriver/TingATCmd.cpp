#include "tingatcmd.h"
#include "util.h"
#if 1
#define TING_DEBUG(...) uart1.printf("[Ting]"),uart1.printf(__VA_ARGS__)
#else
#define  TING_DEBUG(...)
#endif
DATA_STATE_T data_state;
uint8_t data_count;
void Ting::data_process(char c)
{

    switch((uint8_t)data_state)
    {
        case NEED_PLUS:
            if(c == '+')
                data_state = NEED_L;
            else
                data_state = NEED_PLUS;
            break;
        case NEED_L:
            if(c == 'L')
                data_state = NEED_R;
            else
                data_state = NEED_PLUS;
            break;            
        case NEED_R:
            if(c == 'R')
                data_state = NEED_DOT1;
            else
                data_state = NEED_PLUS;
            break;        
        case NEED_DOT1:
            if(c == ',')
            {
                data_count = 0;//初始化计数器
                data_state = NEED_SADDR_DATA;
            }
            else
                data_state = NEED_PLUS;
            break;            
        case NEED_SADDR_DATA:
            source_addr_buf[data_count] = c;
            data_count++;
            if(data_count ==4)
            {
                source_addr_buf[4] = '\0';
                source_addr = ATOI32(source_addr_buf,16);

                data_state = NEED_DOT2;
            }
            else
                data_state = NEED_SADDR_DATA;
            break;            
        case NEED_DOT2:
            if(c == ',')
            {
                data_count = 0;//初始化计数器
                data_state = NEED_LEN_DATA;
            }
            else
                data_state = NEED_PLUS;
            break;            
        case NEED_LEN_DATA:
            len_buf[data_count] = c;
            data_count++;
            if(data_count == 2)
            {
                len_buf[2] = '\0';
                data_len = ATOI32(len_buf,16);
                data_state = NEED_DOT3;

            }
            else
            {
                data_state = NEED_LEN_DATA;

            }
            break;            
        case NEED_DOT3:
            if(c == ',')
            {
                data_count = 0;//初始化计数器
                data_state = NEED_DATA;
            }
            else
                data_state = NEED_PLUS;
            break;            
            
        case NEED_DATA:
            if(data_count < data_len)
            {
                pDataBuf.write(c);
                data_count++;
            }
            else
            {
                data_state = NEED_PLUS;
            }
            break;  
        default:
            uart1.write('x');
            data_state = NEED_PLUS;
            break;
    }
}
void Ting::rx_evend()
{
    uint8_t c;
    c = uart->read();
    last_rx_event_time = millis();
    PB8.toggle();
    //uart1.write(c);
    
    rx_cmd_buf[rx_count] = c;
    if(rx_count++ > CMD_BUFFER_SIZE)
        rx_count = 0;
    cmd_state = RECEIVING;
    data_process(c);

}
bool Ting::begin(Gpio *rst,Gpio *wakeup, Uart *uart, uint32_t baud)
{

    this->uart = uart;
    this->rst = rst;
    this->_wakeup = wakeup;
    this->rst->mode(OUTPUT_PP);
    this->_wakeup->mode(OUTPUT_PP);
    this->rst->set();
    this->_wakeup->set();
    pDataBuf.begin(data_buf, DATA_BUFFER_SIZE); //初始化环形缓冲区


    this->uart->begin(baud);
    this->uart->attach(this,&Ting::rx_evend,RxIrq);
    clear_rx_cdm_buffer();

    hard_reset();
    return true;
}
void Ting::hard_reset()
{
    rst->reset();
    delay_ms(10);
    rst->set();
    delay_ms(1000);
}

CMD_ERR_T Ting::config(sLoRaSettings *settings)
{

}

CMD_ERR_T Ting::test()
{
    //sys_reset();
    uart->printf("AT\r\n");
    wait_ack(1000);
    debug_cmd_err("TEST");
    clear_rx_cdm_buffer();
    return cmd_err;

}
CMD_ERR_T Ting::soft_rst()
{
    uart->printf("AT+RST\r\n");
    wait_ack(1000);
    debug_cmd_err("RST");
    clear_rx_cdm_buffer();
    //delay_ms(500);
    return cmd_err;
}
CMD_ERR_T Ting::get_version(char *msg,uint8_t *len)
{
    uart->printf("AT+VER?\r\n");
    wait_ack(1000);
    *len = get_str(rx_cmd_buf, "OK", 1, msg);    
    debug_cmd_err("VERSION");
    clear_rx_cdm_buffer();
    return cmd_err;
}
CMD_ERR_T Ting::idle()
{
    uart->printf("AT+IDLE\r\n");
    wait_ack(1000);
    debug_cmd_err("IDLE");
    clear_rx_cdm_buffer();
    return cmd_err;
}
CMD_ERR_T Ting::sleep()
{
    uart->printf("AT+SLEEP=1\r\n");
    wait_ack(1000);
    debug_cmd_err("SLEEP");
    clear_rx_cdm_buffer();
    return cmd_err;
}
CMD_ERR_T Ting::wakeup()
{
    _wakeup->reset();
    delay_ms(1);
    wait_ack(1000);
    _wakeup->set();
    debug_cmd_err("WAKEUP");
    clear_rx_cdm_buffer();

    return cmd_err;
}
CMD_ERR_T Ting::rx()
{
    uart->printf("AT+RX\r\n");
    wait_ack(1000);
    debug_cmd_err("RX");
    clear_rx_cdm_buffer();

    return cmd_err;
}
CMD_ERR_T Ting::rssi(char *msg,uint8_t *len)
{
    uart->printf("AT+RSSI?\r\n");
    wait_ack(1000);
    *len = get_str(rx_cmd_buf, "OK", 1, msg);    
    debug_cmd_err("RSSI");
    clear_rx_cdm_buffer();
    return cmd_err;
}

CMD_ERR_T Ting::set_addr(uint16_t addr)
{
    uart->printf("AT+ADDR=%x\r\n",addr);
    wait_ack(1000);
    debug_cmd_err("SET ADDR");
    clear_rx_cdm_buffer();

    return cmd_err;
}
CMD_ERR_T Ting::get_addr(uint16_t *addr)
{
    char buf[5];

    uart->printf("AT+ADDR?\r\n");
    wait_ack(1000);
    
    buf[0] = rx_cmd_buf[0];
    buf[1] = rx_cmd_buf[1];
    buf[2] = rx_cmd_buf[2];
    buf[3] = rx_cmd_buf[3];
    buf[4] = '\0';
    *addr = ATOI32(buf,16);
    
    debug_cmd_err("GET ADDR");
    clear_rx_cdm_buffer();
    return cmd_err;
}
CMD_ERR_T Ting::set_dest(uint16_t addr)
{
    uart->printf("AT+DEST=%x\r\n",addr);
    wait_ack(1000);
    debug_cmd_err("SET DEST");
    clear_rx_cdm_buffer();

    return cmd_err;
}
CMD_ERR_T Ting::get_dest(uint16_t *addr)
{
    char buf[5];

    uart->printf("AT+DEST?\r\n");
    wait_ack(1000);
    
    buf[0] = rx_cmd_buf[0];
    buf[1] = rx_cmd_buf[1];
    buf[2] = rx_cmd_buf[2];
    buf[3] = rx_cmd_buf[3];
    buf[4] = '\0';
    *addr = ATOI32(buf,16);
    
    debug_cmd_err("GET DEST");
    clear_rx_cdm_buffer();
    return cmd_err;
}

CMD_ERR_T Ting::send(uint8_t *ptr,uint8_t len)
{
    uart->printf("AT+SEND=%d\r\n",len);
    if(wait_ack(2000) == ERR_OK)
    {
        clear_rx_cdm_buffer();
        uart->write(ptr,len);
        wait_ack(10000);
        uart1.printf("SEND OK\r\n");
        clear_rx_cdm_buffer();
        
    }
    else
        clear_rx_cdm_buffer();

    return cmd_err;

}


CMD_ERR_T Ting::set_pb0()
{
    uart->printf("AT+PB0=1\r\n");
    wait_ack(1000);
    debug_cmd_err("SET PB0");
    clear_rx_cdm_buffer();
    return cmd_err;

}
CMD_ERR_T Ting::clear_pb0()
{
    uart->printf("AT+PB0=0\r\n");
    wait_ack(1000);
    debug_cmd_err("CLR PB0");
    clear_rx_cdm_buffer();
    return cmd_err;
}
CMD_ERR_T  Ting::read_pb0()
{
    uart->printf("AT+PB0?\r\n");
    wait_ack(1000);
    debug_cmd_err("READ PB0");
    clear_rx_cdm_buffer();
    return cmd_err;
}

CMD_ERR_T Ting::set_pd0()
{
    uart->printf("AT+PD0=1\r\n");
    wait_ack(1000);
    debug_cmd_err("SET PD0");
    clear_rx_cdm_buffer();
    return cmd_err;

}
CMD_ERR_T Ting::clear_pd0()
{
    uart->printf("AT+PD0=0\r\n");
    wait_ack(1000);
    debug_cmd_err("CLR PD0");
    clear_rx_cdm_buffer();
    return cmd_err;
}
CMD_ERR_T  Ting::read_pd0()
{
    uart->printf("AT+PD0?\r\n");
    wait_ack(1000);
    debug_cmd_err("READ PD0");
    clear_rx_cdm_buffer();
    return cmd_err;
}

CMD_ERR_T Ting::pwm1(uint8_t prescaler,uint16_t period,uint16_t pulse)
{
    uart->printf("AT+PWM1=%d,%d,%d\r\n",prescaler,period,pulse);
    wait_ack(1000);
    debug_cmd_err("PWM1");
    clear_rx_cdm_buffer();
    return cmd_err;
}
CMD_ERR_T Ting::pwm2(uint8_t prescaler,uint16_t period,uint16_t pulse)
{
    uart->printf("AT+PWM2=%d,%d,%d\r\n",prescaler,period,pulse);
    wait_ack(1000);
    debug_cmd_err("PWM2");
    clear_rx_cdm_buffer();
    return cmd_err;
}
uint8_t Ting::read(uint8_t *buf)
{
    uint16_t len = pDataBuf.available();
    if(len > 0)
    {
        for(int i = 0; i < len; i++)
        {
            buf[i] = pDataBuf.read();
        }
    }
    else
    {
        buf[0] = 0;
        len = 0;
    }
    return len;
}

CMD_ERR_T Ting::wait_ack(uint16_t timeout)
{
    uint32_t time = millis();
    cmd_state = WAITING;

    while(1)
    {
        if(millis() - time > timeout)
        {
            cmd_state = TIMEOUT;
            TING_DEBUG("cmd TIMEOUT\r\n");
            break;
        }
        if(update_cmd_err() != ERR_NO_ACK)
        {
            cmd_state = RECEIVED;
            break;
        }
    }
    return cmd_err;
}

CMD_ERR_T Ting::update_cmd_err()
{
    if(search_str(rx_cmd_buf, "OK") != -1)
    {
        while(millis() - last_rx_event_time < 2);//等待所有字节接收完成
        cmd_err = ERR_OK;
    }
    else if(search_str(rx_cmd_buf, "ERR:CMD") != -1)
    {
        while(millis() - last_rx_event_time < 2);//等待所有字节接收完成
        cmd_err = ERR_CMD;
    }
    else if(search_str(rx_cmd_buf, "ERR:CPU_BUSY") != -1)
    {
        while(millis() - last_rx_event_time < 2);//等待所有字节接收完成
        cmd_err = ERR_CPU_BUSY;
    }
    else if(search_str(rx_cmd_buf, "ERR:RF_BUSY") != -1)
    {
        while(millis() - last_rx_event_time < 2);//等待所有字节接收完成
        cmd_err = ERR_RF_BUSY;
    }
    else if(search_str(rx_cmd_buf, "ERR:SYMBLE") != -1)
    {
        while(millis() - last_rx_event_time < 2);//等待所有字节接收完成
        cmd_err = ERR_SYMBLE;
    }
    else if(search_str(rx_cmd_buf, "ERR:PARA") != -1)
    {
        while(millis() - last_rx_event_time < 2);//等待所有字节接收完成
        cmd_err = ERR_PARA;
    }
    else if(search_str(rx_cmd_buf, "ERR:NONE") != -1)
    {
        while(millis() - last_rx_event_time < 2);//等待所有字节接收完成
        cmd_err = ERR_NONE;
    }
    else if(search_str(rx_cmd_buf, "WakeUp") != -1)
    {
        while(millis() - last_rx_event_time < 2);//等待所有字节接收完成
        cmd_err = ERR_OK;
    }
    else 
    {
        cmd_err = ERR_NO_ACK;
    }
    return cmd_err;
}
void Ting::debug_cmd_err(const char *str)
{
    
    switch(cmd_err)
    {
    case ERR_OK:
        TING_DEBUG("%s:OK\r\n",str);
       break;
    case ERR_CMD:
        TING_DEBUG("%s:ERR:CMD\r\n",str);
       break;
    case ERR_CPU_BUSY:
        TING_DEBUG("%s:ERR:CPU_BUSY\r\n",str);
       break;
    case ERR_RF_BUSY:
        TING_DEBUG("%s:ERR:RF_BUSY\r\n",str);
       break;
    case ERR_SYMBLE:
        TING_DEBUG("%s:ERR:SYMBLE\r\n",str);
       break;
    case ERR_PARA:
        TING_DEBUG("%s:ERR:PARA\r\n",str);
       break;
    case ERR_NONE:
        TING_DEBUG("%s:ERR:NONE\r\n",str);
       break;
    default :
        TING_DEBUG("%s:default\r\n");
       break;
    }
}

void Ting::clear_rx_cdm_buffer(void) //清空AT命令缓冲区
{
    uint16_t i;
    for(i = 0; i < CMD_BUFFER_SIZE; i++)
    {
        rx_cmd_buf[i] = 0;
    }
    rx_count = 0;
}

uint16_t Ting::find_str(uint8_t *s_str, uint8_t *p_str, uint16_t count, uint16_t &seek)
{
    uint16_t _count = 1;
    uint16_t len = 0;
    seek = 0;
    uint8_t *temp_str = NULL;
    uint8_t *temp_ptr = NULL;
    uint8_t *temp_char = NULL;
    if(0 == s_str || 0 == p_str)
        return 0;
    for(temp_str = s_str; *temp_str != '\0'; temp_str++)	 //依次查找字符串
    {
        temp_char = temp_str; //指向当前字符串
        //比较
        for(temp_ptr = p_str; *temp_ptr != '\0'; temp_ptr++)
        {
            if(*temp_ptr != *temp_char)
            {
                len = 0;
                break;
            }
            temp_char++;
            len++;
        }
        if(*temp_ptr == '\0') //出现了所要查找的字符，退出
        {
            if(_count == count)
                return len;
            else
            {
                _count++;
                len = 0;
            }
        }
        seek++;  //当前偏移量加1
    }
    return 0;
}
int Ting::search_str(char *source, const char *target)
{
    uint16_t seek = 0;
    uint16_t len;
    len = this->find_str((uint8_t *)source, (uint8_t *)target, 1, seek);
    if(len == 0)
        return -1;
    else
        return len;

}

