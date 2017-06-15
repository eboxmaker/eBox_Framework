#include "tingatcmd.h"
#include "util.h"
#if 1
#define TING_DEBUG(...) uart1.printf("[Ting]"),uart1.printf(__VA_ARGS__)
#else
#define  TING_DEBUG(...)
#endif

void Ting::rx_evend()
{
    uint8_t c;
    c = uart->read();
    last_rx_event_time = millis();
    PB8.toggle();
    //uart1.write(c);
    
    rx_cmd_buf[rx_count] = c;
    if(rx_count++ > RX_BUFFER_SIZE)
        rx_count = 0;
    cmd_state = RECEIVING;

}
bool Ting::begin(Gpio *rst, Uart *uart, uint32_t baud)
{

    this->uart = uart;
    this->rst = rst;
    this->rst->mode(OUTPUT_PP);
    sys_reset();

    this->uart->begin(baud);
    this->uart->attach(this,&Ting::rx_evend,RxIrq);
    clear_rx_cdm_buffer();

    return true;
}
void Ting::sys_reset()
{
    rst->reset();
    delay_ms(10);
    rst->set();
    delay_ms(500);
}

void Ting::test()
{
    //sys_reset();
    uart->printf("AT+VER?\r\n");
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
CMD_ERR_T Ting::sleep()
{
    uart->printf("AT+DEST=1\r\n");
    wait_ack(1000);
    debug_cmd_err("SLEEP");
    clear_rx_cdm_buffer();

    return cmd_err;
}
CMD_ERR_T Ting::wakeup()
{
    uart->printf("AT+DEST=0\r\n");
    wait_ack(1000);
    debug_cmd_err("WAKEUP");
    clear_rx_cdm_buffer();

    return cmd_err;
}
CMD_ERR_T Ting::send(uint8_t *ptr,uint8_t len)
{
    uart->printf("AT+SEND=%d\r\n",len);
    wait_ack(2000);
    debug_cmd_err("SEND");
    clear_rx_cdm_buffer();

    uart->write(ptr,len);
    debug_cmd_err("DATA");
    wait_ack(2000);

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

CMD_STATE_T Ting::wait_ack(uint16_t timeout)
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
        update_cmd_err();
        if(cmd_err != ERR_NO_ACK)
        {
            cmd_state = RECEIVED;
            break;
        }
    }

    return cmd_state;

}
void Ting::update_cmd_err()
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
    else 
    {
        cmd_err = ERR_NO_ACK;
    }
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
    for(i = 0; i < RX_BUFFER_SIZE; i++)
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

