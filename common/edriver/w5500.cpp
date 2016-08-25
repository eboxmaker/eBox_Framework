/**
  ******************************************************************************
  * @file    w5500.cpp
  * @author  shentq
  * @version V1.2
  * @date    2016/08/14
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
#include "w5500.h"
#include "string.h"

void W5500::begin(uint8_t dev_num,uint8_t *mac)
{
    uint8_t ip_0[4]={0,};
    begin(dev_num,mac,ip_0,ip_0,ip_0,ip_0);
}

void W5500::begin(uint8_t dev_num, uint8_t *mac, uint8_t *ip, uint8_t *subnet, uint8_t *gateway, uint8_t *dns)
{
    uint8_t txsize[MAX_SOCK_NUM] = {2, 2, 2, 2, 2, 2, 2, 2};
    uint8_t rxsize[MAX_SOCK_NUM] = {2, 2, 2, 2, 2, 2, 2, 2};


    spi_dev_w5500.dev_num = dev_num;
    spi_dev_w5500.mode = SPI_MODE0;
    spi_dev_w5500.prescaler = SPI_CLOCK_DIV2;
    spi_dev_w5500.bit_order = SPI_BITODER_MSB;

    spi->begin(&spi_dev_w5500);
    cs->mode(OUTPUT_PP);
    cs->set();
    rst_pin->mode(OUTPUT_PP);
    rst_pin->set();

    reset();
    //中断屏蔽寄存器
    //setIMR(0XF0);
    //	setSIMR(0XFF);//允许所有socket产生中断。
    //	setSn_IMR(0,0x04);//允许接收中断
    //	setSn_IMR(1,0x04);//允许接收中断
    //	setSn_IMR(2,0x04);//允许接收中断
    //	setSn_IMR(3,0x04);//允许接收中断
    //	setSn_IMR(4,0x04);//允许接收中断
    //	setSn_IMR(5,0x04);//允许接收中断
    //	setSn_IMR(6,0x04);//允许接收中断
    //	setSn_IMR(7,0x04);//允许接收中断

    memcpy(this->mac, mac, 6);
    memcpy(this->ip, ip, 4);
    memcpy(this->subnet, subnet, 4);
    memcpy(this->gw, gateway, 4);
    memcpy(this->dns, dns, 4);

    setSHAR(mac);/*配置Mac地址*/
    setSIPR(ip);/*配置Ip地址*/
    setSUBR(subnet);/*配置子网掩码*/
    setGAR(gw);/*配置默认网关*/

    sysinit(txsize, rxsize); /*初始化8个socket*/
    setRTR(2000);/*设置溢出时间值*/
    setRCR(3);/*设置最大重新发送次数*/
    delay_ms(1000);//预留一定时间保证网卡获取相关网络信息

}

void W5500::reset()
{
    rst_pin->reset();
    delay_ms(1);
    rst_pin->set();
    delay_ms(500);

}

void W5500::write(uint32_t addrbsb, uint8_t data)
{

    spi->take_spi_right(&spi_dev_w5500);
    cs->reset();                              // CS=0, SPI start
    spi->write( (addrbsb & 0x00FF0000) >> 16); // Address byte 1
    spi->write( (addrbsb & 0x0000FF00) >> 8); // Address byte 2
    spi->write( (addrbsb & 0x000000F8) + 4);    // Data write command and Write data length 1
    spi->write(data);                    // Data write (write 1byte data)
    cs->set();                                 // CS=1,  SPI end
    spi->release_spi_right();

}
uint8_t  W5500::read(uint32_t addrbsb)
{
    uint8_t data = 0;
    spi->take_spi_right(&spi_dev_w5500);
    cs->reset();                          // CS=0, SPI start
    spi->write( (addrbsb & 0x00FF0000) >> 16); // Address byte 1
    spi->write( (addrbsb & 0x0000FF00) >> 8); // Address byte 2
    spi->write( (addrbsb & 0x000000F8))    ;// Data read command and Read data length 1
    spi->read(&data);                // Data read (read 1byte data)
    cs->set();                            // CS=1,  SPI end
    spi->release_spi_right();
    return data;

}
uint16_t W5500::write(uint32_t addrbsb, uint8_t *buf, uint16_t len)
{
    spi->take_spi_right(&spi_dev_w5500);
    cs->reset();                               // CS=0, SPI start
    spi->write( (addrbsb & 0x00FF0000) >> 16); // Address byte 1
    spi->write( (addrbsb & 0x0000FF00) >> 8); // Address byte 2
    spi->write( (addrbsb & 0x000000F8) + 4);    // Data write command and Write data length 1
    spi->write(buf, len);
    cs->set();                                 // CS=1, SPI end
    spi->release_spi_right();

    return len;

}
uint16_t W5500::read(uint32_t addrbsb, uint8_t *buf, uint16_t len)
{
    spi->take_spi_right(&spi_dev_w5500);
    cs->reset();                               // CS=0, SPI start
    spi->write( (addrbsb & 0x00FF0000) >> 16); // Address byte 1
    spi->write( (addrbsb & 0x0000FF00) >> 8); // Address byte 2
    spi->write( (addrbsb & 0x000000F8));    // Data write command and Write data length 1
    spi->read(buf, len);
    cs->set();                                 // CS=1, SPI end
    spi->release_spi_right();

    return len;

}

/**
@brief  This function set the transmit & receive buffer size as per the channels is used
Note for TMSR and RMSR bits are as follows\n
bit 1-0 : memory size of channel #0 \n
bit 3-2 : memory size of channel #1 \n
bit 5-4 : memory size of channel #2 \n
bit 7-6 : memory size of channel #3 \n
bit 9-8 : memory size of channel #4 \n
bit 11-10 : memory size of channel #5 \n
bit 12-12 : memory size of channel #6 \n
bit 15-14 : memory size of channel #7 \n
Maximum memory size for Tx, Rx in the W5500 is 16K Bytes,\n
In the range of 16KBytes, the memory size could be allocated dynamically by each channel.\n
Be attentive to sum of memory size shouldn't exceed 8Kbytes\n
and to data transmission and receiption from non-allocated channel may cause some problems.\n
If the 16KBytes memory is already  assigned to centain channel, \n
other 3 channels couldn't be used, for there's no available memory.\n
If two 4KBytes memory are assigned to two each channels, \n
other 2 channels couldn't be used, for there's no available memory.\n
*/
void W5500::sysinit( uint8_t  *tx_size, uint8_t  *rx_size  )
{
    int16_t i = 0;
    int16_t ssum, rsum;
#ifdef __DEF_IINCHIP_DBG__
    //  printf("sysinit()\r\n");
#endif
    ssum = 0;
    rsum = 0;

    for (i = 0 ; i < MAX_SOCK_NUM; i++)       // Set the size, masking and base address of Tx & Rx memory by each channel
    {
        write( (Sn_TXMEM_SIZE(i)), tx_size[i]);
        write( (Sn_RXMEM_SIZE(i)), rx_size[i]);

#ifdef __DEF_IINCHIP_DBG__
        //         printf("tx_size[%d]: %d, Sn_TXMEM_SIZE = %d\r\n",i, tx_size[i], IINCHIP_READ(Sn_TXMEM_SIZE(i)));
        //         printf("rx_size[%d]: %d, Sn_RXMEM_SIZE = %d\r\n",i, rx_size[i], IINCHIP_READ(Sn_RXMEM_SIZE(i)));
#endif
        SSIZE[i] = (int16_t)(0);
        RSIZE[i] = (int16_t)(0);


        if (ssum <= 16384)
        {
            switch( tx_size[i] )
            {
            case 1:
                SSIZE[i] = (int16_t)(1024);
                break;
            case 2:
                SSIZE[i] = (int16_t)(2048);
                break;
            case 4:
                SSIZE[i] = (int16_t)(4096);
                break;
            case 8:
                SSIZE[i] = (int16_t)(8192);
                break;
            case 16:
                SSIZE[i] = (int16_t)(16384);
                break;
            default :
                RSIZE[i] = (int16_t)(2048);
                break;
            }
        }

        if (rsum <= 16384)
        {
            switch( rx_size[i] )
            {
            case 1:
                RSIZE[i] = (int16_t)(1024);
                break;
            case 2:
                RSIZE[i] = (int16_t)(2048);
                break;
            case 4:
                RSIZE[i] = (int16_t)(4096);
                break;
            case 8:
                RSIZE[i] = (int16_t)(8192);
                break;
            case 16:
                RSIZE[i] = (int16_t)(16384);
                break;
            default :
                RSIZE[i] = (int16_t)(2048);
                break;
            }
        }
        ssum += SSIZE[i];
        rsum += RSIZE[i];

    }
}


uint8_t W5500::getISR(SOCKET s)
{
    return I_STATUS[s];
}
void W5500::putISR(SOCKET s, uint8_t val)
{
    I_STATUS[s] = val;
}

uint16_t W5500::getRxMAX(SOCKET s)
{
    return RSIZE[s];
}
uint16_t W5500::getTxMAX(SOCKET s)
{
    return SSIZE[s];
}




/**
@brief  This function sets up MAC address.
*/
void W5500::setSHAR(
    uint8_t  *addr  /**< a pointer to a 6 -byte array responsible to set the MAC address. */
)
{
    write(SHAR0, addr, 6);
}
/**
@brief  This function sets up Source IP address.
*/
void W5500::setSIPR(
    uint8_t  *addr  /**< a pointer to a 4 -byte array responsible to set the Source IP address. */
)
{
    write(SIPR0, addr, 4);
}
/**
@brief  It sets up SubnetMask address
*/
void W5500::setSUBR(uint8_t  *addr)
{
    write(SUBR0, addr, 4);
}
/**
@brief  This function sets up gateway IP address.
*/
void W5500::setGAR(
    uint8_t  *addr  /**< a pointer to a 4 -byte array responsible to set the Gateway IP address. */
)
{
    write(GAR0, addr, 4);
}





void W5500::getMAC(uint8_t  *addr )//mac
{
    read(SHAR0, addr, 6);
}
void  W5500::getIP(uint8_t  *addr)//ip
{
    read(SIPR0, addr, 4);
}
void  W5500::getSubnet(uint8_t  *addr)//mask
{
    read(SUBR0, addr, 4);
}
void W5500::getGateway(uint8_t  *addr)//gateway
{
    read(GAR0, addr, 4);
}


void W5500::getSHAR(uint8_t  *addr )//mac
{
    read(SHAR0, addr, 6);
}
void  W5500::getSIPR(uint8_t  *addr)//ip
{
    read(SIPR0, addr, 4);
}
void  W5500::getSUBR(uint8_t  *addr)//mask
{
    read(SUBR0, addr, 4);
}
void W5500::getGWIP(uint8_t  *addr)//gateway
{
    read(GAR0, addr, 4);
}
void  W5500::getGAR(uint8_t  *addr)//gateway
{
    read(GAR0, addr, 4);
}


void W5500::setMR(uint8_t val)
{
    write(MR, val);
}
void W5500::setIMR(uint8_t val)
{
    write(IMR, val);
}
void W5500::setSIMR(uint8_t val)
{
    write(SIMR, val);

}

/**
@brief  This function gets Interrupt register in common register.
 */
uint8_t W5500::getIR( void )
{
    return read(IR);
}
uint8_t W5500::getSIR( void )
{
    return read(SIR);
}

/**
@brief  This function sets up Retransmission time.

If there is no response from the peer or delay in response then retransmission
will be there as per RTR (Retry Time-value Register)setting
*/
void W5500::setRTR(uint16_t timeout)
{
    write(RTR0, (uint8_t)((timeout & 0xff00) >> 8));
    write(RTR1, (uint8_t)(timeout & 0x00ff));
}

/**
@brief  This function set the number of Retransmission.

If there is no response from the peer or delay in response then recorded time
as per RTR & RCR register seeting then time out will occur.
*/
void W5500::setRCR(uint8_t retry)
{
    write(WIZ_RCR, retry);
}

/**
@brief  This function set the interrupt mask Enable/Disable appropriate Interrupt. ('1' : interrupt enable)

If any bit in IMR is set as '0' then there is not interrupt signal though the bit is
set in IR register.
*/
void W5500::setIR(uint8_t val)
{
    write(IR, val ); // must be setted 0x10.
}
void W5500::setSIR(uint8_t val)
{
    write(SIR, val ); // must be setted 0x10.
}

/////////////////////////////////////////////////////////////////////////////////////

void W5500::setSn_IMR(SOCKET s, uint8_t mask)
{
    write( Sn_IMR(s), mask);
}
uint8_t W5500::getSn_IMR(SOCKET s)
{
    return read(Sn_IMR(s));
}

/**
@brief  This sets the maximum segment size of TCP in Active Mode), while in Passive Mode this is set by peer
*/
void W5500::setSn_MSS(SOCKET s, uint16_t Sn_MSSR)
{
    write( Sn_MSSR0(s), (uint8_t)((Sn_MSSR & 0xff00) >> 8));
    write( Sn_MSSR1(s), (uint8_t)(Sn_MSSR & 0x00ff));
}

void W5500::setSn_TTL(SOCKET s, uint8_t ttl)
{
    write( Sn_TTL(s) , ttl);
}
/**
@brief  get socket interrupt status

These below functions are used to read the Interrupt & Soket Status register
*/
uint8_t  W5500::getSn_IR(SOCKET s)
{
    return read(Sn_IR(s));
}

/**
@brief   get socket status
*/
uint8_t  W5500::getSn_SR(SOCKET s)
{
    return read(Sn_SR(s));
}
void W5500::getSn_DIPR(SOCKET s, uint8_t *ip)
{
    ip[0] = read(Sn_DIPR0(s));
    ip[1] = read(Sn_DIPR1(s));
    ip[2] = read(Sn_DIPR2(s));
    ip[3] = read(Sn_DIPR3(s));
}
uint16_t W5500::getSn_DPORT(SOCKET s)
{
    uint16_t tmp;
    tmp = (read(Sn_DPORT0(s)) << 8) + read(Sn_DPORT1(s));
    return tmp;
}

/**
@brief  get socket TX free buf size

This gives free buffer size of transmit buffer. This is the data size that user can transmit.
User shuold check this value first and control the size of transmitting data
*/
uint16_t W5500::get_tx_free_size(SOCKET s)
{
    uint16_t val = 0, val1 = 0;
    do
    {
        val1 = read(Sn_TX_FSR0(s));
        val1 = (val1 << 8) + read(Sn_TX_FSR1(s));
        if (val1 != 0)
        {
            val = read(Sn_TX_FSR0(s));
            val = (val << 8) + read(Sn_TX_FSR1(s));
        }
    }
    while (val != val1);
    return val;
}
/**
@brief   get socket RX recv buf size

This gives size of received data in receive buffer.
*/
uint16_t W5500::get_rx_recv_size(SOCKET s)
{
    uint16_t val = 0, val1 = 0;
    do
    {
        val1 = read(Sn_RX_RSR0(s));
        val1 = (val1 << 8) + read(Sn_RX_RSR1(s));
        if(val1 != 0)
        {
            val = read(Sn_RX_RSR0(s));
            val = (val << 8) + read(Sn_RX_RSR1(s));
        }
    }
    while (val != val1);
    return val;
}

/**
@brief   This function is being called by send() and sendto() function also.

This function read the Tx write pointer register and after copy the data in buffer update the Tx write pointer
register. User should read upper byte first and lower byte later to get proper value.
*/
void W5500::send_data_processing(SOCKET s, uint8_t *data, uint16_t len)
{
    uint16_t ptr = 0;
    uint32_t addrbsb = 0;
    if(len == 0)
    {
        //    printf("CH: %d Unexpected1 length 0\r\n", s);
        return;
    }


    ptr = read( Sn_TX_WR0(s) );
    ptr = ((ptr & 0x00ff) << 8) + read(Sn_TX_WR1(s));

    addrbsb = ((uint32_t)ptr << 8) + (s << 5) + 0x10;
    write(addrbsb, data, len);

    ptr += len;
    write( Sn_TX_WR0(s) , (uint8_t)((ptr & 0xff00) >> 8));
    write( Sn_TX_WR1(s), (uint8_t)(ptr & 0x00ff));
}


/**
@brief  This function is being called by recv() also.

This function read the Rx read pointer register
and after copy the data from receive buffer update the Rx write pointer register.
User should read upper byte first and lower byte later to get proper value.
*/
void W5500::recv_data_processing(SOCKET s, uint8_t *data, uint16_t len)
{
    uint16_t ptr = 0;
    uint32_t addrbsb = 0;

    if(len == 0)
    {
        //    printf("CH: %d Unexpected2 length 0\r\n", s);
        return;
    }

    ptr = read( Sn_RX_RD0(s) );
    ptr = ((ptr & 0x00ff) << 8) + read( Sn_RX_RD1(s) );

    addrbsb = ((uint32_t)ptr << 8) + (s << 5) + 0x18;
    read(addrbsb, data, len);
    ptr += len;

    write( Sn_RX_RD0(s), (uint8_t)((ptr & 0xff00) >> 8));
    write( Sn_RX_RD1(s), (uint8_t)(ptr & 0x00ff));
}

void W5500::setSn_IR(SOCKET s, uint8_t val)
{
    write(Sn_IR(s), val);
}





