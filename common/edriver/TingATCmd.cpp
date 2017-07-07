/**
  ******************************************************************************
  * @file    tingATCmd.cpp
  * @author  shentq
  * @version V0.3
  * @date    2017/06/25
  * @brief   
  ******************************************************************************
  * @attention
  *
  * No part of this software may be used for any commercial activities by any form 
  * or means, without the prior written consent of shentq. This specification is 
  * preliminary and is subject to change at any time without notice. shentq assumes
  * no responsibility for any errors contained herein.
  * <h2><center>&copy; Copyright 2015 shentq. All Rights Reserved.</center></h2>
  ******************************************************************************
  */
  

/* Includes ------------------------------------------------------------------*/
#include "tingatcmd.h"
#if 1
#define TING_DEBUG(...) uart1.printf("[Ting]"),uart1.printf(__VA_ARGS__)
#else
#define  TING_DEBUG(...)
#endif

DATA_STATE_T Ting::data_process(char c)
{

    switch((uint8_t)data_state)
    {
  
        case NEED_DOT1:
            if(c == ',')
            {
                data_count = 0;//初始化计数器
                data_state = NEED_SADDR_DATA;
            }
            else
                data_state = STATE_ERROR;
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
                data_state = STATE_ERROR;
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
                data_state = STATE_ERROR;
            break;            
            
        case NEED_DATA:
            data_count++;
            if(data_count <= data_len )
            {
                pDataBuf.write(c);
            }
            if(data_count >= (data_len + 2))//接收模组多发送的\r\n的结束符号
            {
                data_state = NEED_DOT1;
            }
            break;  
        default:
            uart1.write('x');
            data_state = STATE_ERROR;
            break;
    }
    return data_state;
}
CMD_MODE_T cmd_mode = MODE_IDLE;
void Ting::rx_evend()
{
    uint8_t c;
    static uint8_t atHead[2];
    uint8_t temp;
    c = uart->read();
    last_rx_event_time = millis();
    
    switch((uint8_t)cmd_mode)
    {
    case MODE_IDLE:
        atHead[0] = atHead[1];
        atHead[1] = c;
        if((memcmp(atHead, "AT", 2) == 0))
        {
            cmd_mode = MODE_AT;
            atHead[1] = 0x00;
        }
        if((memcmp(atHead, "LR", 2) == 0))
        {
            cmd_mode = MODE_DATA;
            atHead[1] = 0x00;
        }
        break;
    case MODE_AT:
        rx_cmd_buf[rx_count] = c;
        rx_count++;
        if(c == '\n')
        {
            if(rx_signal_on == true)
                if(memcmp(rx_cmd_buf, ",TimeOut", 8) == 0)
                {
                    rx_timeoutflag = 1;
                    clear_rx_cdm_buffer();
                    cmd_mode = MODE_IDLE;
                    break;
                }
            cmd_mode = MODE_AT_END;
        }
        if(rx_count > CMD_BUFFER_SIZE)
        {
            cmd_mode = MODE_AT_TOO_LONG;
        }
        break;
    case MODE_DATA:
        temp = data_process(c);
        if((temp == NEED_DOT1) || (temp == STATE_ERROR))
        {
            cmd_mode = MODE_IDLE;
        }
        break;
    }

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

    hard_reset();
    clear_rx_cdm_buffer();
    return true;
}
void Ting::hard_reset()
{
    rst->reset();
    delay_ms(10);
    rst->set();
    delay_ms(1000);
}
CMD_ERR_T Ting::soft_reset()
{
    uart->printf("AT+RST\r\n");
    if(_wait_ack(1000) == MODE_AT_END)
    {
        update_cmd_err();
        debug_cmd_err("RST");
    }
    clear_rx_cdm_buffer();
    return cmd_err;
}
CMD_ERR_T Ting::test()
{
    uart->printf("AT\r\n");
    if(_wait_ack(1000) == MODE_AT_END)
    {
        update_cmd_err();
        debug_cmd_err("TEST");
    }
    cmd_mode = MODE_IDLE;
    clear_rx_cdm_buffer();
    return cmd_err;

}

CMD_ERR_T Ting::config(sLoRaSettings *settings)
{
    uart->printf("AT+CFG=%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\r\n",
    settings->RFFrequency,
    settings->Power,
    settings->SignalBw,
    settings->SpreadingFactor,
    settings->ErrorCoding,
    settings->CrcOn,
    settings->ImplicitHeaderOn,
    settings->RxSingleOn,
    settings->FreqHopOn,
    settings->HopPeriod,
    settings->RxPacketTimeout,
    settings->PayloadLength,
    settings->PreambleLength
    );
    rx_signal_on = settings->RxSingleOn;

    if(_wait_ack(1000) == MODE_AT_END)
    {
        update_cmd_err();
        debug_cmd_err("CONFIG");
    }
    cmd_mode = MODE_IDLE;
    clear_rx_cdm_buffer();
    return cmd_err;
}



CMD_ERR_T Ting::get_version(char *msg,uint8_t *len)
{
    uart->printf("AT+VER?\r\n");
    if(_wait_ack(1000) == MODE_AT_END)
    {
        *len = get_str(rx_cmd_buf, ",", 1, ",OK\r\n", 1, msg);    
        update_cmd_err();
        debug_cmd_err("VER");
    }
    cmd_mode = MODE_IDLE;
    clear_rx_cdm_buffer();
    return cmd_err;
}
CMD_ERR_T Ting::idle()
{
    uart->printf("AT+IDLE\r\n");
    if(_wait_ack(1000) == MODE_AT_END)
    {
        update_cmd_err();
        debug_cmd_err("IDLE");
    }
    cmd_mode = MODE_IDLE;
    clear_rx_cdm_buffer();
    return cmd_err;
}
CMD_ERR_T Ting::sleep()
{
    uart->printf("AT+SLEEP=1\r\n");
    if(_wait_ack(1000) == MODE_AT_END)
    {
        update_cmd_err();
        debug_cmd_err("SLEEP");
    }
    cmd_mode = MODE_IDLE;
    clear_rx_cdm_buffer();
    return cmd_err;
}
CMD_ERR_T Ting::wakeup()
{
    _wakeup->reset();
    delay_ms(1  );
    _wakeup->set();
    if(_wait_ack(1000) == MODE_AT_END)
    {
        update_cmd_err();
        debug_cmd_err("WAKEUP");
    }
    cmd_mode = MODE_IDLE;
    clear_rx_cdm_buffer();
    return cmd_err;

}
CMD_ERR_T Ting::rx()
{
    uart->printf("AT+RX\r\n");
    if(_wait_ack(1000) == MODE_AT_END)
    {
        update_cmd_err();
        debug_cmd_err("RX");
        rx_timeoutflag = 0;
    }
    cmd_mode = MODE_IDLE;
    clear_rx_cdm_buffer();
    return cmd_err;
}
void Ting::timeout_process()
{
    update_cmd_err();
    debug_cmd_err("TimeOutxx\r\n");
}

CMD_ERR_T Ting::rssi(char *msg,uint8_t *len)
{
    uart->printf("AT+RSSI?\r\n");
    if(_wait_ack(1000) == MODE_AT_END)
    {
        *len = get_str(rx_cmd_buf, ",", 1, ",OK\r\n", 1, msg);    
        update_cmd_err();
        debug_cmd_err("RSSI");
    }
    cmd_mode = MODE_IDLE;
    clear_rx_cdm_buffer();
    return cmd_err;
   
}

CMD_ERR_T Ting::set_addr(uint16_t addr)
{
    uart->printf("AT+ADDR=%x\r\n",addr);
    if(_wait_ack(1000) == MODE_AT_END)
    {
        update_cmd_err();
        debug_cmd_err("set addr");
    }
    cmd_mode = MODE_IDLE;
    clear_rx_cdm_buffer();
    return cmd_err;
}
CMD_ERR_T Ting::get_addr(uint16_t *addr)
{
    char buf[5];

    uart->printf("AT+ADDR?\r\n");
    if(_wait_ack(1000) == MODE_AT_END)
    {
        get_str(rx_cmd_buf, ",", 1, ",OK\r\n", 1, buf);    
        *addr = ATOI32(buf,16);
        update_cmd_err();
        debug_cmd_err("get addr");
    }
    cmd_mode = MODE_IDLE;
    clear_rx_cdm_buffer();
    return cmd_err;    
}
CMD_ERR_T Ting::set_dest(uint16_t addr)
{
    uart->printf("AT+DEST=%x\r\n",addr);
    if(_wait_ack(1000) == MODE_AT_END)
    {
        update_cmd_err();
        debug_cmd_err("set dest");
    }
    cmd_mode = MODE_IDLE;
    clear_rx_cdm_buffer();
    return cmd_err;
}
CMD_ERR_T Ting::get_dest(uint16_t *addr)
{
    char buf[5];

    uart->printf("AT+DEST?\r\n");
    if(_wait_ack(1000) == MODE_AT_END)
    {
        get_str(rx_cmd_buf, ",", 1, ",OK\r\n", 1, buf);    
        *addr = ATOI32(buf,16);
        update_cmd_err();
        debug_cmd_err("get dest");
    }
    cmd_mode = MODE_IDLE;
    clear_rx_cdm_buffer();
    return cmd_err; 
}
CMD_ERR_T Ting::save()
{
    uart->printf("AT+SAVE\r\n");
    if(_wait_ack(1000) == MODE_AT_END)
    {
        update_cmd_err();
        debug_cmd_err("save");
    }
    cmd_mode = MODE_IDLE;
    clear_rx_cdm_buffer();
    return cmd_err;
}

CMD_ERR_T Ting::send(uint8_t *ptr,uint8_t len)
{
    uart->printf("AT+SEND=%d\r\n",len);
    if(_wait_ack(2000) == MODE_AT_END)
    {
        update_cmd_err();
        debug_cmd_err("send");
        cmd_mode = MODE_IDLE;
        clear_rx_cdm_buffer();
        uart->write(ptr,len);
        if(_wait_ack(1000) == MODE_AT_END)
        {
            update_cmd_err();
            debug_cmd_err("sendING");
            cmd_mode = MODE_IDLE;
            clear_rx_cdm_buffer();
        }
        if(_wait_ack(1000) == MODE_AT_END)
        {
            update_cmd_err();
            debug_cmd_err("sended");
            clear_rx_cdm_buffer();
        }
    }
    else if(cmd_mode == MODE_AT_TIMEOUT)
    {
        cmd_err = ERR_TIMEOUT;
    }
    cmd_mode = MODE_IDLE;
    clear_rx_cdm_buffer();
    return cmd_err;
}


CMD_ERR_T Ting::set_pb0()
{
    uart->printf("AT+PB0=1\r\n");
    if(_wait_ack(1000) == MODE_AT_END)
    {
        update_cmd_err();
        debug_cmd_err("set pb0");
    }
    cmd_mode = MODE_IDLE;
    clear_rx_cdm_buffer();
    return cmd_err;

}
CMD_ERR_T Ting::clear_pb0()
{
    uart->printf("AT+PB0=0\r\n");
    if(_wait_ack(1000) == MODE_AT_END)
    {
        update_cmd_err();
        debug_cmd_err("clr pb0");
    }
    cmd_mode = MODE_IDLE;
    clear_rx_cdm_buffer();
    return cmd_err;
}
CMD_ERR_T  Ting::read_pb0()
{
    uart->printf("AT+PB0?\r\n");
    if(_wait_ack(1000) == MODE_AT_END)
    {
        update_cmd_err();
        debug_cmd_err("read pb0");
    }
    cmd_mode = MODE_IDLE;
    clear_rx_cdm_buffer();
    return cmd_err;
}

CMD_ERR_T Ting::set_pd0()
{
    uart->printf("AT+PD0=1\r\n");
    if(_wait_ack(1000) == MODE_AT_END)
    {
        update_cmd_err();
        debug_cmd_err("set pd0");
    }
    cmd_mode = MODE_IDLE;
    clear_rx_cdm_buffer();
    return cmd_err;

}
CMD_ERR_T Ting::clear_pd0()
{
    uart->printf("AT+PD0=0\r\n");
    if(_wait_ack(1000) == MODE_AT_END)
    {
        update_cmd_err();
        debug_cmd_err("clr pd0");
    }
    cmd_mode = MODE_IDLE;
    clear_rx_cdm_buffer();
    return cmd_err;
}
CMD_ERR_T  Ting::read_pd0()
{
    uart->printf("AT+PD0?\r\n");
    if(_wait_ack(1000) == MODE_AT_END)
    {
        update_cmd_err();
        debug_cmd_err("read dp0");
    }
    cmd_mode = MODE_IDLE;
    clear_rx_cdm_buffer();
    return cmd_err;
}

CMD_ERR_T Ting::pwm1(uint8_t prescaler,uint16_t period,uint16_t pulse)
{
    uart->printf("AT+PWM1=%d,%d,%d\r\n",prescaler,period,pulse);
    if(_wait_ack(1000) == MODE_AT_END)
    {
        update_cmd_err();
        debug_cmd_err("pwm1");
    }
    cmd_mode = MODE_IDLE;
    clear_rx_cdm_buffer();
    return cmd_err;
}
CMD_ERR_T Ting::pwm2(uint8_t prescaler,uint16_t period,uint16_t pulse)
{
    uart->printf("AT+PWM2=%d,%d,%d\r\n",prescaler,period,pulse);
    if(_wait_ack(1000) == MODE_AT_END)
    {
        update_cmd_err();
        debug_cmd_err("pwm2");
    }
    cmd_mode = MODE_IDLE;
    clear_rx_cdm_buffer();
    return cmd_err;
}
uint8_t Ting::available()
{
    return pDataBuf.available();
}

uint8_t Ting::read(uint8_t *buf,uint8_t len)
{
    if( len > pDataBuf.available())
    {
        len = pDataBuf.available();
    }
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
uint8_t Ting::is_rx_timeout()
{
    return rx_timeoutflag;
}

CMD_MODE_T Ting::_wait_ack(uint16_t timeout)//等待命令回复
{
    uint64_t time = millis();
    while(cmd_mode != MODE_AT_END)
    {
        if(millis() - time > timeout)
        {
            cmd_mode = MODE_AT_TIMEOUT;
            break;
        }
    }
    return cmd_mode;

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
    else if(search_str(rx_cmd_buf, "WakeUp") != -1)
    {
        while(millis() - last_rx_event_time < 2);//等待所有字节接收完成
        cmd_err = ERR_OK;
    }
    else if(search_str(rx_cmd_buf, "SENDING") != -1)
    {
        while(millis() - last_rx_event_time < 2);//等待所有字节接收完成
        cmd_err = ERR_OK;
    }
    else if(search_str(rx_cmd_buf, "SENDED") != -1)
    {
        while(millis() - last_rx_event_time < 2);//等待所有字节接收完成
        cmd_err = ERR_OK;
    }
    else if(search_str(rx_cmd_buf, "TimeOut") != -1)
    {
        while(millis() - last_rx_event_time < 2);//等待所有字节接收完成
        cmd_err = ERR_TIMEOUT;
    }
    else 
    {
        cmd_err = ERR_OTHER;
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

int Ting::search_str(char *source, const char *target)
{
    uint16_t seek = 0;
    uint16_t len;
    len = find_str((uint8_t *)source, (uint8_t *)target, 1, seek);
    if(len == 0)
        return -1;
    else
        return len;

}

