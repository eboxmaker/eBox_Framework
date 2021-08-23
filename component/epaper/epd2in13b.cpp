/**
 *  @filename   :   epd2in9b.cpp
 *  @brief      :   Implements for Dual-color e-paper library
 *  @author     :   Yehui from Waveshare
 *
 *  Copyright (C) Waveshare     August 10 2017
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documnetation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to  whom the Software is
 * furished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <stdlib.h>
#include "epd2in13b.h"


#if EBOX_DEBUG
// 是否打印调试信息, 1打印,0不打印
#define EBOX_DEBUG_EPD_ENABLE       true
#define EBOX_DEBUG_EPD_ENABLE_ERR   true
#endif


#if EBOX_DEBUG_EPD_ENABLE
#define epdDebug(...)  ebox_printf("[epd DBG]:%d: ",__LINE__),ebox_printf(__VA_ARGS__ )
#else
#define epdSpiDebug(...)
#endif

#if EBOX_DEBUG_EPD_ENABLE_ERR
#define epdDebugErr(fmt, ...)  ebox_printf("[epd err]:%d: " fmt "\n", __LINE__, __VA_ARGS__)
#else
#define epdDebugErr(fmt, ...)
#endif

Epd::~Epd() {
};

Epd::Epd(Gpio *rst,Gpio *dc,Gpio *busy,Gpio *cs,Spi *spi) {
    this->rst = rst;
    this->dc = dc;
    this->cs = cs;
    this->busy = busy;
    this->spi = spi;
    width = EPD_WIDTH;
    height = EPD_HEIGHT;
};

int Epd::Init(void) {
    /* this calls the peripheral hardware interface, see epdif */
    rst->mode(OUTPUT_PP);
    dc->mode(OUTPUT_PP);
    cs->mode(OUTPUT_PP);
    busy->mode(INPUT);
    Spi::Config_t cfg;
    cfg.bit_order = Spi::MSB;
    cfg.dev_num = cs->id;
    cfg.mode = Spi::MODE0;
    cfg.prescaler = Spi::DIV64;
    spi->begin(&cfg);
    
    /* EPD hardware init start */
    Reset();
    SendCommand(BOOSTER_SOFT_START);
    SendData(0x17);
    SendData(0x17);
    SendData(0x17);
    SendCommand(POWER_ON);
    WaitUntilIdle();
    SendCommand(PANEL_SETTING);
    SendData(0x8F);
    SendCommand(VCOM_AND_DATA_INTERVAL_SETTING);
    SendData(0x37);
    SendCommand(RESOLUTION_SETTING);
    SendData(0x68);     // width: 104
    SendData(0x00);
    SendData(0xD4);     // height: 212
    /* EPD hardware init end */
    return 0;

}

/**
 *  @brief: basic function for sending commands
 */
void Epd::SendCommand(unsigned char command) {
    dc->reset();
    cs->reset();
    spi->write(command);
    cs->set();
}

/**
 *  @brief: basic function for sending data
 */
void Epd::SendData(const unsigned char data) {
    dc->set();
    cs->reset();
    spi->write(data);
    cs->set();}

/**
 *  @brief: Wait until the busy_pin goes HIGH
 */
void Epd::WaitUntilIdle(void) {
    while(busy->read() == 0) {      //0: busy, 1: idle
        delay_ms(100);
        epdDebug(".");
    }     
    epdDebug("\n");
    
}

/**
 *  @brief: module reset. 
 *          often used to awaken the module in deep sleep, 
 *          see Epd::Sleep();
 */
void Epd::Reset(void) {
    rst->reset();
    delay_ms(200);
    rst->set();
    delay_ms(200);
}

/**
 *  @brief: transmit partial data to the SRAM
 */
void Epd::SetPartialWindow(const unsigned char* buffer_black, const unsigned char* buffer_red, int x, int y, int w, int l) {
    SendCommand(PARTIAL_IN);
    SendCommand(PARTIAL_WINDOW);
    SendData(x & 0xf8);     // x should be the multiple of 8, the last 3 bit will always be ignored
    SendData(((x & 0xf8) + w  - 1) | 0x07);
    SendData(y >> 8);        
    SendData(y & 0xff);
    SendData((y + l - 1) >> 8);        
    SendData((y + l - 1) & 0xff);
    SendData(0x01);         // Gates scan both inside and outside of the partial window. (default) 
    delay_ms(2);
    SendCommand(DATA_START_TRANSMISSION_1);
    if (buffer_black != NULL) {
        for(int i = 0; i < w  / 8 * l; i++) {
            SendData(buffer_black[i]);  
        }  
    } else {
        for(int i = 0; i < w  / 8 * l; i++) {
            SendData(0x00);  
        }  
    }
    delay_ms(2);
    SendCommand(DATA_START_TRANSMISSION_2);
    if (buffer_red != NULL) {
        for(int i = 0; i < w  / 8 * l; i++) {
            SendData(buffer_red[i]);  
        }  
    } else {
        for(int i = 0; i < w  / 8 * l; i++) {
            SendData(0x00);  
        }  
    }
    delay_ms(2);
    SendCommand(PARTIAL_OUT);  
}

/**
 *  @brief: transmit partial data to the black part of SRAM
 */
void Epd::SetPartialWindowBlack(const unsigned char* buffer_black, int x, int y, int w, int l) {
    SendCommand(PARTIAL_IN);
    SendCommand(PARTIAL_WINDOW);
    SendData(x & 0xf8);     // x should be the multiple of 8, the last 3 bit will always be ignored
    SendData(((x & 0xf8) + w  - 1) | 0x07);
    SendData(y >> 8);        
    SendData(y & 0xff);
    SendData((y + l - 1) >> 8);        
    SendData((y + l - 1) & 0xff);
    SendData(0x01);         // Gates scan both inside and outside of the partial window. (default) 
    delay_ms(2);
    SendCommand(DATA_START_TRANSMISSION_1);
    if (buffer_black != NULL) {
        for(int i = 0; i < w  / 8 * l; i++) {
            SendData(buffer_black[i]);  
        }  
    } else {
        for(int i = 0; i < w  / 8 * l; i++) {
            SendData(0x00);  
        }  
    }
    delay_ms(2);
    SendCommand(PARTIAL_OUT);  
}

/**
 *  @brief: transmit partial data to the red part of SRAM
 */
void Epd::SetPartialWindowRed(const unsigned char* buffer_red, int x, int y, int w, int l) {
    SendCommand(PARTIAL_IN);
    SendCommand(PARTIAL_WINDOW);
    SendData(x & 0xf8);     // x should be the multiple of 8, the last 3 bit will always be ignored
    SendData(((x & 0xf8) + w  - 1) | 0x07);
    SendData(y >> 8);        
    SendData(y & 0xff);
    SendData((y + l - 1) >> 8);        
    SendData((y + l - 1) & 0xff);
    SendData(0x01);         // Gates scan both inside and outside of the partial window. (default) 
    delay_ms(2);
    SendCommand(DATA_START_TRANSMISSION_2);
    if (buffer_red != NULL) {
        for(int i = 0; i < w  / 8 * l; i++) {
            SendData(buffer_red[i]);  
        }  
    } else {
        for(int i = 0; i < w  / 8 * l; i++) {
            SendData(0x00);  
        }  
    }
    delay_ms(2);
    SendCommand(PARTIAL_OUT);  
}

/**
 * @brief: refresh and displays the frame
 */
void Epd::DisplayFrame(const unsigned char* frame_buffer_black, const unsigned char* frame_buffer_red) {
    if (frame_buffer_black != NULL) {
        SendCommand(DATA_START_TRANSMISSION_1);
        delay_ms(2);
        for (int i = 0; i < this->width * this->height / 8; i++) {
            SendData(frame_buffer_black[i]);
        }
        delay_ms(2);
    }
    if (frame_buffer_red != NULL) {
        SendCommand(DATA_START_TRANSMISSION_2);
        delay_ms(2);
        for (int i = 0; i < this->width * this->height / 8; i++) {
            SendData(frame_buffer_red[i]);
        }
        delay_ms(2);
    }
    SendCommand(DISPLAY_REFRESH);
    WaitUntilIdle();
}

/**
 * @brief: clear the frame data from the SRAM, this won't refresh the display
 */
void Epd::ClearFrame(void) {
    SendCommand(DATA_START_TRANSMISSION_1);           
    delay_ms(2);
    for(int i = 0; i < width * height / 8; i++) {
        SendData(0xFF);  
    }  
    delay_ms(2);
    SendCommand(DATA_START_TRANSMISSION_2);           
    delay_ms(2);
    for(int i = 0; i < width * height / 8; i++) {
        SendData(0xFF);  
    }  
    delay_ms(2);
}

/**
 * @brief: This displays the frame data from SRAM
 */
void Epd::DisplayFrame(void) {
    SendCommand(DISPLAY_REFRESH); 
    epdDebug("发送完刷新命令");
    WaitUntilIdle();
}

/**
 * @brief: After this command is transmitted, the chip would enter the deep-sleep mode to save power. 
 *         The deep sleep mode would return to standby by hardware reset. The only one parameter is a 
 *         check code, the command would be executed if check code = 0xA5. 
 *         You can use Epd::Reset() to awaken and use Epd::Init() to initialize.
 */
void Epd::Sleep() {
    SendCommand(POWER_OFF);
    WaitUntilIdle();
    SendCommand(DEEP_SLEEP);
    SendData(0xA5);     // check code
}


/* END OF FILE */


