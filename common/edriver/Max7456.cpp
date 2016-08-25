/**
  ******************************************************************************
  * @file    max7456.cpp
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

#include "Max7456.h"

//------------------ begin ---------------------------------------------------

void OSD::begin(uint8_t dev_num)
{
    spi_dev_max7456.dev_num = dev_num;
    spi_dev_max7456.mode = SPI_MODE0;
    spi_dev_max7456.prescaler = SPI_CLOCK_DIV128;
    spi_dev_max7456.bit_order = SPI_BITODER_MSB;

    spi->begin(&spi_dev_max7456);
    cs->mode(OUTPUT_PP);
    cs->set();
    init();
}

//------------------ init ---------------------------------------------------

void OSD::init()
{
    uint8_t x;

    detect_mode();

    spi->take_spi_right(&spi_dev_max7456);

    cs->reset(); //select chip
    //read black level register

    spi->write(MAX7456_OSDBL_reg_read);//black level read register
    uint8_t osdbl_r = spi->read();

    spi->write(MAX7456_VM0_reg);
    spi->write(MAX7456_RESET | video_mode);
    delay_us(100);
    //set black level
    uint8_t osdbl_w = (osdbl_r & 0xef); //Set bit 4 to zero 11101111
    spi->write(MAX7456_OSDBL_reg); //black level write register
    spi->write(osdbl_w);

    // set all rows to same charactor white level, 90%
    for (x = 0; x < MAX7456_screen_rows; x++)
    {
        spi->write(x + 0x10);
        spi->write(MAX7456_WHITE_level_120);
    }

    // define sync (auto,int,ext) and
    // making sure the Max7456 is enabled
    control(1);

    spi->release_spi_right();
}

//------------------ Detect Mode (PAL/NTSC) ---------------------------------

void OSD::detect_mode()
{
    //    digitalWrite(MAX7456_SELECT,LOW);
    //read STAT and auto detect Mode PAL/NTSC
    //    Spi.transfer(MAX7456_STAT_reg_read);//status register
    //    byte osdstat_r = Spi.transfer(0xff);

    //    if ((B00000001 & osdstat_r) == 1){
    //        setMode(1);
    //    }
    //    else if((B00000010 & osdstat_r) == 1){
    set_mode(1);
    //    }
    //#ifdef MinimOSD
    //    else if (digitalRead(3) == 1){
    //        setMode(1);
    //    }
    //#endif
#if 0 //to do 
    if (EEPROM.read(PAL_NTSC_ADDR) == 1)
    {
        setMode(1);
        digitalWrite(MAX7456_SELECT, LOW);
    }
    else setMode(0);
#endif

    cs->reset(); //select chip
}

//------------------ Set Mode (PAL/NTSC) ------------------------------------

void OSD::set_mode(int themode)
{
    switch(themode)
    {
    case 0:
        video_mode = MAX7456_MODE_MASK_NTCS;
        video_center = MAX7456_CENTER_NTSC;
        break;
    case 1:
        video_mode = MAX7456_MODE_MASK_PAL;
        video_center = MAX7456_CENTER_PAL;
        break;
    }
}

//------------------ Get Mode (PAL 0/NTSC 1) --------------------------------

int OSD::get_mode()
{
    switch(video_mode)
    {
    case MAX7456_MODE_MASK_NTCS:
        return 0;
    //      break;
    case MAX7456_MODE_MASK_PAL:
        return 1;
        //      break;
    }
    return 0;
}

//------------------ Get Center (PAL/NTSC) ----------------------------------

int OSD::get_center()
{
    return video_center; //first line for center panel
}

//------------------ plug ---------------------------------------------------

void OSD::plug()
{

    cs->reset(); //select chip
}

//------------------ clear ---------------------------------------------------

void OSD::clear()
{
    spi->take_spi_right(&spi_dev_max7456);
    // clear the screen
    cs->reset(); //select chip
    spi->write(MAX7456_DMM_reg);
    spi->write(MAX7456_CLEAR_display);
    cs->set(); //undo select chip

    spi->release_spi_right();
}

//------------------ set panel -----------------------------------------------

void
OSD::set_panel(uint8_t st_col, uint8_t st_row)
{
    start_col = st_col;
    start_row = st_row;
    col = st_col;
    row = st_row;
}

//------------------ open panel ----------------------------------------------

void
OSD::open_panel(void)
{
    unsigned int linepos;
    uint8_t settings, char_address_hi, char_address_lo;

    //find [start address] position
    linepos = row * 30 + col;

    // divide 16 bits into hi & lo byte
    char_address_hi = linepos >> 8;
    char_address_lo = linepos;

    //Auto increment turn writing fast (less SPI commands).
    //No need to set next char address. Just send them
    spi->take_spi_right(&spi_dev_max7456);

    settings = MAX7456_INCREMENT_auto; //To Enable DMM Auto Increment

    cs->reset(); //select chip
    spi->write(MAX7456_DMM_reg); //dmm
    spi->write(settings);

    spi->write(MAX7456_DMAH_reg); // set start address high
    spi->write(char_address_hi);

    spi->write(MAX7456_DMAL_reg); // set start address low
    spi->write(char_address_lo);
    //Serial.printf("setPos -> %d %d\n", col, row);

    spi->release_spi_right();
}

//------------------ close panel ---------------------------------------------

void
OSD::close_panel(void)
{
    spi->take_spi_right(&spi_dev_max7456);

    spi->write(MAX7456_DMDI_reg);
    spi->write(MAX7456_END_string); //This is needed "trick" to finish auto increment
    cs->set(); //undo select chip

    spi->release_spi_right();
    //Serial.println("close");
    row++; //only after finish the auto increment the new row will really act as desired
}

//------------------ write single char ---------------------------------------------

void
OSD::open_single(uint8_t x, uint8_t y)
{
    unsigned int linepos;
    uint8_t char_address_hi, char_address_lo;

    //find [start address] position
    linepos = y * 30 + x;

    // divide 16 bits into hi & lo byte
    char_address_hi = linepos >> 8;
    char_address_lo = linepos;

    spi->take_spi_right(&spi_dev_max7456);

    //digitalWrite(MAX7456_SELECT,LOW);
    cs->reset(); //select chip

    spi->write(MAX7456_DMAH_reg); // set start address high
    spi->write(char_address_hi);

    spi->write(MAX7456_DMAL_reg); // set start address low
    spi->write(char_address_lo);
    //Serial.printf("setPos -> %d %d\n", col, row);

    spi->release_spi_right();
}

//------------------ write ---------------------------------------------------

uint8_t
OSD::write(uint8_t c)
{
    if(c == '|')
    {
        close_panel(); //It does all needed to finish auto increment and change current row
        open_panel(); //It does all needed to re-enable auto increment
    }
    else
    {
        spi->take_spi_right(&spi_dev_max7456);

        spi->write(MAX7456_DMDI_reg);
        spi->write(c);

        spi->release_spi_right();
    }
    return 1;
}

//------------------ printf ---------------------------------------------------
void OSD::printf(const char *fmt, ...)
{
    char buf[32];
    uint8_t i;
    va_list va_params;
    va_start(va_params, fmt);
    vsprintf(buf, fmt, va_params);
    va_end(va_params);

    while(buf[i] != '\0')
    {
        write(buf[i++]);
    }
}

//---------------------------------

void
OSD::control(uint8_t ctrl)
{

    spi->take_spi_right(&spi_dev_max7456);

    cs->reset(); //select chip
    spi->write(MAX7456_VM0_reg);
    switch(ctrl)
    {
    case 0:
        spi->write(MAX7456_DISABLE_display | video_mode);
        break;
    case 1:
        //Spi.transfer((MAX7456_ENABLE_display_vert | video_mode) | MAX7456_SYNC_internal);
        //Spi.transfer((MAX7456_ENABLE_display_vert | video_mode) | MAX7456_SYNC_external);
        spi->write((MAX7456_ENABLE_display_vert | video_mode) | MAX7456_SYNC_autosync);
        break;
    }

    cs->set(); //undo select chip

    spi->release_spi_right();
}

void
OSD::write_NVM(int font_count, uint8_t *character_bitmap)
{
    uint8_t x;
    uint8_t char_address_hi;
    //uint8_t char_address_lo;
    uint8_t screen_char;

    char_address_hi = font_count;
    //char_address_lo = 0;
    //Serial.println("write_new_screen");

    // disable display
    spi->take_spi_right(&spi_dev_max7456);

    cs->reset(); //select chip
    spi->write(MAX7456_VM0_reg);
    spi->write(MAX7456_DISABLE_display);

    spi->write(MAX7456_CMAH_reg); // set start address high
    spi->write(char_address_hi);

    for(x = 0; x < NVM_ram_size; x++) // write out 54 (out of 64) bytes of character to shadow ram
    {
        screen_char = character_bitmap[x];
        spi->write(MAX7456_CMAL_reg); // set start address low
        spi->write(x);
        spi->write(MAX7456_CMDI_reg);
        spi->write(screen_char);
    }

    // transfer a 54 bytes from shadow ram to NVM
    spi->write(MAX7456_CMM_reg);
    spi->write(WRITE_nvr);

    // wait until bit 5 in the status register returns to 0 (12ms)
    while (( spi->write(MAX7456_STAT_reg_read) & STATUS_reg_nvr_busy) != 0x00);

    spi->write(MAX7456_VM0_reg); // turn on screen next vertical
    spi->write(MAX7456_ENABLE_display_vert);
    cs->set(); //undo select chip

    spi->release_spi_right();
}

