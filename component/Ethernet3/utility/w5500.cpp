/*
 * Copyright (c) 2010 by WIZnet <support@wiznet.co.kr>
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 *
 * - 10 Apr. 2015
 * Added support for Arduino Ethernet Shield 2
 * by Arduino.org team
 */

#include <stdio.h>
#include <string.h>

#include "w5500.h"

// W5500 controller instance
//W5500Class w5500;

// SPI details
//SPISettings wiznet_SPI_settings(8000000, MSBFIRST, SPI_MODE0);
//uint8_t SPI_CS;

void W5500Class::init(uint8_t socketNumbers)
{
    //  SPI_CS = ss_pin;

    if(initialized == 0)
    {
        config.dev_num = cs->id;
        config.mode = Spi::MODE0;
        config.prescaler = Spi::DIV2;
        config.bit_order = Spi::MSB;
        initialized = 1;
    }
    spi->begin(&config);
    cs->mode(OUTPUT_PP);
    cs->set();
    rst_pin->mode(OUTPUT_PP);
    rst_pin->set();

    reset();
    //    delay_ms(1000);

    if(socketNumbers == 1)
    {
        for (int i = 1; i < MAX_SOCK_NUM; i++)
        {
            uint8_t cntl_byte = (0x0C + (i << 5));
            write( 0x1E, cntl_byte, 0); //0x1E - Sn_RXBUF_SIZE
            write( 0x1F, cntl_byte, 0); //0x1F - Sn_TXBUF_SIZE
        }
        uint8_t cntl_byte = (0x0C + (0 << 5));
        write( 0x1E, cntl_byte, 16); //0x1E - Sn_RXBUF_SIZE
        write( 0x1F, cntl_byte, 16); //0x1F - Sn_TXBUF_SIZE
    }

    else if(socketNumbers == 2)
    {
        for (int i = 2; i < MAX_SOCK_NUM; i++)
        {
            uint8_t cntl_byte = (0x0C + (i << 5));
            write( 0x1E, cntl_byte, 0); //0x1E - Sn_RXBUF_SIZE
            write( 0x1F, cntl_byte, 0); //0x1F - Sn_TXBUF_SIZE
        }
        for (int i = 0; i < 2; i++)
        {
            uint8_t cntl_byte = (0x0C + (i << 5));
            write( 0x1E, cntl_byte, 8); //0x1E - Sn_RXBUF_SIZE
            write( 0x1F, cntl_byte, 8); //0x1F - Sn_TXBUF_SIZE
        }
    }

    else if(socketNumbers == 4)
    {
        for (int i = 4; i < MAX_SOCK_NUM; i++)
        {
            uint8_t cntl_byte = (0x0C + (i << 5));
            write( 0x1E, cntl_byte, 0); //0x1E - Sn_RXBUF_SIZE
            write( 0x1F, cntl_byte, 0); //0x1F - Sn_TXBUF_SIZE
        }
        for (int i = 0; i < 4; i++)
        {
            uint8_t cntl_byte = (0x0C + (i << 5));
            write( 0x1E, cntl_byte, 4); //0x1E - Sn_RXBUF_SIZE
            write( 0x1F, cntl_byte, 4); //0x1F - Sn_TXBUF_SIZE
        }
    }

    else
    {
        for (int i = 0; i < MAX_SOCK_NUM; i++)
        {
            uint8_t cntl_byte = (0x0C + (i << 5));
            write( 0x1E, cntl_byte, 2); //0x1E - Sn_RXBUF_SIZE
            write( 0x1F, cntl_byte, 2); //0x1F - Sn_TXBUF_SIZE
        }
    }
}
void W5500Class::reset()
{
    rst_pin->reset();
    delay_ms(1);
    rst_pin->set();
    delay_ms(500);

}
uint16_t W5500Class::getTXFreeSize(SOCKET s)
{
    uint16_t val = 0, val1 = 0;
    do
    {
        val1 = readSnTX_FSR(s);
        if (val1 != 0)
            val = readSnTX_FSR(s);
    }
    while (val != val1);
    return val;

}

uint16_t W5500Class::getRXReceivedSize(SOCKET s)
{
    uint16_t val = 0, val1 = 0;
    do
    {
        val1 = readSnRX_RSR(s);
        if (val1 != 0)
            val = readSnRX_RSR(s);
    }
    while (val != val1);
    return val;
}

void W5500Class::send_data_processing(SOCKET s, const uint8_t *data, uint16_t len)
{
    // This is same as having no offset in a call to send_data_processing_offset
    send_data_processing_offset(s, 0, data, len);

}

void W5500Class::send_data_processing_offset(SOCKET s, uint16_t data_offset, const uint8_t *data, uint16_t len)
{
    uint16_t ptr = readSnTX_WR(s);
    uint8_t cntl_byte = (0x14 + (s << 5));
    ptr += data_offset;
    write(ptr, cntl_byte, data, len);
    ptr += len;
    writeSnTX_WR(s, ptr);
}

void W5500Class::recv_data_processing(SOCKET s, uint8_t *data, uint16_t len, uint8_t peek)
{
    uint16_t ptr;
    ptr = readSnRX_RD(s);

    read_data(s, ptr, data, len);
    if (!peek)
    {
        ptr += len;
        writeSnRX_RD(s, ptr);
    }
}

void W5500Class::read_data(SOCKET s, volatile uint16_t src, volatile uint8_t *dst, uint16_t len)
{
    uint8_t cntl_byte = (0x18 + (s << 5));
    read((uint16_t)src, cntl_byte, (uint8_t *)dst, len);
}

uint8_t W5500Class::write(uint16_t _addr, uint8_t _cb, uint8_t _data)
{

    //    SPI.beginTransaction(wiznet_SPI_settings);
    //    setSS();
    //    SPI.transfer(_addr >> 8);
    //    SPI.transfer(_addr & 0xFF);
    //    SPI.transfer(_cb);
    //    SPI.transfer(_data);
    //    resetSS();
    //    SPI.endTransaction();

    spi->take(&config);
    cs->reset();                               // CS=0, SPI start
    spi->transfer(_addr >> 8);
    spi->transfer(_addr & 0xFF);
    spi->transfer(_cb);
    spi->transfer(_data);
    cs->set();                                 // CS=1, SPI end
    spi->release();


    return 1;
}

uint16_t W5500Class::write(uint16_t _addr, uint8_t _cb, const uint8_t *_buf, uint16_t _len)
{
    //    SPI.beginTransaction(wiznet_SPI_settings);
    //    setSS();
    //    SPI.transfer(_addr >> 8);
    //    SPI.transfer(_addr & 0xFF);
    //    SPI.transfer(_cb);
    //    for (uint16_t i=0; i<_len; i++){
    //        SPI.transfer(_buf[i]);
    //    }
    //    resetSS();
    //    SPI.endTransaction();
    spi->take(&config);
    cs->reset();                               // CS=0, SPI start
    spi->transfer(_addr >> 8);
    spi->transfer(_addr & 0xFF);
    spi->transfer(_cb);
    for (uint16_t i = 0; i < _len; i++)
    {
        spi->transfer(_buf[i]);
    }

    cs->set();                                 // CS=1, SPI end
    spi->release();

    return _len;
}

uint8_t W5500Class::read(uint16_t _addr, uint8_t _cb)
{
    //    SPI.beginTransaction(wiznet_SPI_settings);
    //    setSS();
    //    SPI.transfer(_addr >> 8);
    //    SPI.transfer(_addr & 0xFF);
    //    SPI.transfer(_cb);
    //    uint8_t _data = SPI.transfer(0);
    //    resetSS();
    //    SPI.endTransaction();
    ;
    spi->take(&config);
    cs->reset();                               // CS=0, SPI start
    spi->transfer(_addr >> 8);
    spi->transfer(_addr & 0xFF);
    spi->transfer(_cb);
    uint8_t _data = spi->transfer(0);
    cs->set();                                 // CS=1, SPI end
    spi->release();
    return _data;
}

uint16_t W5500Class::read(uint16_t _addr, uint8_t _cb, uint8_t *_buf, uint16_t _len)
{
    //    SPI.beginTransaction(wiznet_SPI_settings);
    //    setSS();
    //    SPI.transfer(_addr >> 8);
    //    SPI.transfer(_addr & 0xFF);
    //    SPI.transfer(_cb);
    //    for (uint16_t i=0; i<_len; i++){
    //        _buf[i] = SPI.transfer(0);
    //    }
    //    resetSS();
    //    SPI.endTransaction();
    spi->take(&config);
    cs->reset();                               // CS=0, SPI start
    spi->transfer(_addr >> 8);
    spi->transfer(_addr & 0xFF);
    spi->transfer(_cb);
    for (uint16_t i = 0; i < _len; i++)
    {
        _buf[i] = spi->transfer(0);
    }

    cs->set();                                 // CS=1, SPI end
    spi->release();

    return _len;
}

void W5500Class::execCmdSn(SOCKET s, SockCMD _cmd)
{
    // Send command to socket
    writeSnCR(s, _cmd);
    // Wait for command to complete
    while (readSnCR(s))
        ;
}


uint8_t W5500Class::readVersion(void)
{
    //    SPI.beginTransaction(wiznet_SPI_settings);
    //    setSS();
    //    SPI.transfer( 0x00 );
    //    SPI.transfer( 0x39 );
    //    SPI.transfer( 0x01);
    //    uint8_t _data = SPI.transfer(0);
    //    resetSS();
    //    SPI.endTransaction();

    uint8_t _data;
    spi->take(&config);
    cs->reset();                               // CS=0, SPI start

    cs->set();                                 // CS=1, SPI end
    spi->release();
    return _data;
}

// Soft reset the Wiznet chip, by writing to its MR register reset bit
uint8_t W5500Class::softReset(void)
{
    uint16_t count = 0;
    // write to reset bit
    writeMR(0x80);
    // then wait for soft reset to complete
    do
    {
        uint8_t mr = readMR();
        if (mr == 0) return 1;
        delay_ms(1);
    }
    while (++count < 20);
    return 0;
}
uint8_t W5500Class::readSn(SOCKET _s, uint16_t _addr)
{
    uint8_t cntl_byte = (_s << 5) + 0x08;
    return read(_addr, cntl_byte);
}

uint8_t W5500Class::writeSn(SOCKET _s, uint16_t _addr, uint8_t _data)
{
    uint8_t cntl_byte = (_s << 5) + 0x0C;
    return write(_addr, cntl_byte, _data);
}

uint16_t W5500Class::readSn(SOCKET _s, uint16_t _addr, uint8_t *_buf, uint16_t _len)
{
    uint8_t cntl_byte = (_s << 5) + 0x08;
    return read(_addr, cntl_byte, _buf, _len );
}

uint16_t W5500Class::writeSn(SOCKET _s, uint16_t _addr, uint8_t *_buf, uint16_t _len)
{
    uint8_t cntl_byte = (_s << 5) + 0x0C;
    return write(_addr, cntl_byte, _buf, _len);
}

//#endif
