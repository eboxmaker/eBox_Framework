#include "ft800.h"
SPI_CONFIG_TYPE spi_dev_ft800;

u8 FT800::begin(uint8_t dev_num)
{
    spi_dev_ft800.dev_num = dev_num;
    spi_dev_ft800.mode = SPI_MODE0;
    spi_dev_ft800.prescaler = SPI_CLOCK_DIV4;
    spi_dev_ft800.bit_order = SPI_BITODER_MSB;


    spi->begin(&spi_dev_ft800);
    cs->mode(OUTPUT_PP);
    cs->set();
    int_pin->mode(INPUT);

    uint8_t dev_id = 0;                  // Variable for holding the read device id
    u16 ret;
    //
    //	hostcmd(FT_GPU_INTERNAL_OSC);
    //	delay_ms(20);
    //	hostcmd(FT_GPU_ACTIVE_M);
    //WAKE

    host_cmd_write(CMD_CORERST);
    delay_ms(100);

    host_cmd_active();
    delay_ms(100);

    //Ext Clock
    host_cmd_write(CMD_CLKINT);         // Send CLK_EXT Command (0x44)

    //PLL (48M) Clock
    //	host_cmd_write(CMD_CLK48M);         // Send CLK_48M Command (0x62)

    //Read Dev ID
    dev_id = HOST_MEM_RD8(REG_ID);      // Read device id
    uart1.printf("\r\ndev_id = %x", dev_id);
    if(dev_id != 0x7C)                  // Device ID should always be 0x7C
    {
        return 1;
    }
    HOST_MEM_WR8(REG_GPIO, 0x00);			// Set REG_GPIO to 0 to turn off the LCD DISP signal
    HOST_MEM_WR8(REG_PCLK, 0x00);      		// Pixel Clock Output disable

    HOST_MEM_WR16(REG_HCYCLE, 548);         // Set H_Cycle to 548
    HOST_MEM_WR16(REG_HOFFSET, 43);         // Set H_Offset to 43
    HOST_MEM_WR16(REG_HSYNC0, 0);           // Set H_SYNC_0 to 0
    HOST_MEM_WR16(REG_HSYNC1, 41);          // Set H_SYNC_1 to 41
    HOST_MEM_WR16(REG_VCYCLE, 292);         // Set V_Cycle to 292
    HOST_MEM_WR16(REG_VOFFSET, 12);         // Set V_OFFSET to 12
    HOST_MEM_WR16(REG_VSYNC0, 0);           // Set V_SYNC_0 to 0
    HOST_MEM_WR16(REG_VSYNC1, 10);          // Set V_SYNC_1 to 10
    HOST_MEM_WR8(REG_SWIZZLE, 0);           // Set SWIZZLE to 0
    HOST_MEM_WR8(REG_PCLK_POL, 1);          // Set PCLK_POL to 1
    HOST_MEM_WR8(REG_CSPREAD, 1);           // Set CSPREAD to 1
    HOST_MEM_WR16(REG_HSIZE, 480);          // Set H_SIZE to 480
    HOST_MEM_WR16(REG_VSIZE, 272);          // Set V_SIZE to 272
    ret = HOST_MEM_RD16(REG_HCYCLE);
    uart1.printf("\r\nret = %d", ret);
    ret = HOST_MEM_RD16(REG_HOFFSET);
    uart1.printf("\r\nret = %d", ret);

    ret = HOST_MEM_RD16(REG_HSYNC0);
    uart1.printf("\r\nret = %d", ret);

    ret = HOST_MEM_RD16(REG_HSYNC1);
    uart1.printf("\r\nret = %d", ret);

    ret = HOST_MEM_RD16(REG_VCYCLE);
    uart1.printf("\r\nret = %d", ret);

    ret = HOST_MEM_RD16(REG_VOFFSET);
    uart1.printf("\r\nret = %d", ret);

    ret = HOST_MEM_RD16(REG_VSYNC0);
    uart1.printf("\r\nret = %d", ret);

    /* configure touch & audio */
    //	HOST_MEM_WR8(REG_TOUCH_MODE, 0x03);     	//set touch on: continous
    //	HOST_MEM_WR8(REG_TOUCH_ADC_MODE, 0x01); 	//set touch mode: differential
    //	HOST_MEM_WR8(REG_TOUCH_OVERSAMPLE, 0x0F); 	//set touch oversampling to max
    //	HOST_MEM_WR16(REG_TOUCH_RZTHRESH, 5000);	//set touch resistance threshold
    //	HOST_MEM_WR8(REG_VOL_SOUND, 0xFF);      	//set the volume to maximum

    /* write first display list */
    //	HOST_MEM_WR32(RAM_DL+0, CLEAR_COLOR_RGB(0,0,0));  // Set Initial Color to BLACK
    //	HOST_MEM_WR32(RAM_DL+4, CLEAR(1,1,1));            // Clear to the Initial Color
    //	HOST_MEM_WR32(RAM_DL+8, DISPLAY());               // End Display List

    //	HOST_MEM_WR8(REG_DLSWAP, DLSWAP_FRAME);           // Make this display list active on the next frame

    //	HOST_MEM_WR8(REG_GPIO_DIR, 0x80);                 // Set Disp GPIO Direction
    //	HOST_MEM_WR8(REG_GPIO, 0x80);                     // Enable Disp (if used)
    //	HOST_MEM_WR16(REG_PWM_HZ, 0x00FA);                // Backlight PWM frequency
    //	HOST_MEM_WR8(REG_PWM_DUTY, 0x80);                 // Backlight PWM duty

    //	 HOST_MEM_WR8(REG_PCLK, 0x05);                     // After this display is visible on the LCD
    return 0;
}
/*
    Function: host_mem_read
    ARGS:     addr: 24 Bit Command Address
              pnt:  output buffer for read data
              len:  length of bytes to be read

    Description: Reads len(n) bytes of data, starting at addr into pnt(buffer)
*/
void FT800::host_mem_read_start(uint32_t addr, uint8_t *pnt, uint8_t len)
{
    cs->reset();
    spi->write(((addr >> 16) & 0x3F) );			// Send out bits 23:16 of addr, bits 7:6 of this byte must be 00
    spi->write(((addr >> 8) & 0xFF));       	// Send out bits 15:8 of addr
    spi->write((addr & 0xFF));            	// Send out bits 7:0 of addr

    spi->write(0);                      	// Send out DUMMY (0) byte

    while(len--)                      	// While Len > 0 Read out n bytes
        *pnt++ = spi->read();

    cs->set();
}

/*
    Function: HOST_MEM_WR_STR
    ARGS:     addr: 24 Bit Command Address
              pnt:  input buffer of data to send
              len:  length of bytes to be send

    Description: Writes len(n) bytes of data from pnt (buffer) to addr
*/
void FT800::host_mem_write_start(uint32_t addr, uint8_t *pnt, uint8_t len)
{
    cs->reset();
    spi->write(((addr >> 16) & 0x3F) | 0x80); // Send out 23:16 of addr, bits 7:6 of this byte must be 10
    spi->write(((addr >> 8) & 0xFF));       // Send out bits 15:8 of addr
    spi->write((addr & 0xFF));              // Send out bits 7:0 of addr

    while(len--)                          // While Len > 0 Write *pnt (then increment pnt)
        spi->write(*pnt++);

    cs->set();
}

/*
    Function: HOST_CMD_WRITE
    ARGS:     CMD:  5 bit Command

    Description: Writes Command to FT800
*/
void FT800::host_cmd_write(uint8_t CMD)
{
    cs->reset();
    spi->write((uint8_t)(CMD | 0x40));      // Send out Command, bits 7:6 must be 01
    spi->write(0x00);
    spi->write(0x00);
    cs->set();
}

void FT800::host_cmd_active(void)
{
    cs->reset();
    spi->write(0x00);
    spi->write(0x00);
    spi->write(0x00);
    cs->set();
}


/*
    Function: HOST_MEM_WR8
    ARGS:     addr: 24 Bit Command Address
              data: 8bit Data Byte

    Description: Writes 1 byte of data to addr
*/
void FT800::HOST_MEM_WR8(uint32_t addr, uint8_t data)
{
    cs->reset();
    spi->write((addr >> 16) | 0x80);
    spi->write(((addr >> 8) & 0xFF));
    spi->write((addr & 0xFF));

    spi->write(data);

    cs->set();
}

/*
    Function: HOST_MEM_WR16
    ARGS:     addr: 24 Bit Command Address
              data: 16bit (2 bytes)

    Description: Writes 2 bytes of data to addr
*/
void FT800::HOST_MEM_WR16(uint32_t addr, uint32_t data)
{
    cs->reset();
    spi->write((addr >> 16) | 0x80);
    spi->write(((addr >> 8) & 0xFF));
    spi->write((addr & 0xFF));

    /* Little-Endian: Least Significant Byte to: smallest address */
    spi->write( (uint8_t)((data & 0xFF)) );  //byte 0
    spi->write( (uint8_t)((data >> 8)) );    //byte 1

    cs->set();
}

/*
    Function: HOST_MEM_WR32
    ARGS:     addr: 24 Bit Command Address
              data: 32bit (4 bytes)

    Description: Writes 4 bytes of data to addr
*/
void FT800::HOST_MEM_WR32(uint32_t addr, uint32_t data)
{
    cs->reset();
    spi->write((addr >> 16) | 0x80);
    spi->write(((addr >> 8) & 0xFF));
    spi->write((addr & 0xFF));

    spi->write( (uint8_t)(data & 0xFF) );
    spi->write( (uint8_t)((data >> 8) & 0xFF) );
    spi->write( (uint8_t)((data >> 16) & 0xFF) );
    spi->write( (uint8_t)((data >> 24) & 0xFF) );

    cs->set();
}

/*
    Function: HOST_MEM_RD8
    ARGS:     addr: 24 Bit Command Address

    Description: Returns 1 byte of data from addr
*/
uint8_t FT800::HOST_MEM_RD8(uint32_t addr)
{
    uint8_t data_in;

    cs->reset();
    spi->write((uint8_t)((addr >> 16) & 0x3F));
    spi->write((uint8_t)((addr >> 8) & 0xFF));
    spi->write((uint8_t)(addr));
    spi->write(0);

    data_in = spi->read();

    cs->set();
    return data_in;
}

/*
    Function: HOST_MEM_RD16
    ARGS:     addr: 24 Bit Command Address

    Description: Returns 2 byte of data from addr in a 32bit variable
*/
uint32_t FT800::HOST_MEM_RD16(uint32_t addr)
{
    uint8_t data_in = 0;
    uint32_t data = 0;
    uint8_t i;

    cs->reset();
    spi->write(((addr >> 16) & 0x3F));
    spi->write(((addr >> 8) & 0xFF));
    spi->write((addr & 0xFF));
    spi->write(0);

    for(i = 0; i < 2; i++)
    {
        data_in = spi->read();
        data |= ( ((uint32_t)data_in) << (8 * i) );
    }

    cs->set();
    return data;
}

/*
    Function: HOST_MEM_RD32
    ARGS:     addr: 24 Bit Command Address

    Description: Returns 4 byte of data from addr in a 32bit variable
*/
uint32_t FT800::HOST_MEM_RD32(uint32_t addr)
{
    uint8_t data_in = 0;
    uint32_t data = 0;
    uint8_t i;

    cs->reset();
    spi->write(((addr >> 16) & 0x3F));
    spi->write(((addr >> 8) & 0xFF));
    spi->write((addr & 0xFF));
    spi->write(0);

    for(i = 0; i < 4; i++)
    {
        data_in = spi->read();
        data |= ( ((uint32_t)data_in) << (8 * i) );
    }

    cs->set();
    return data;
}
/*** CMD Functions *****************************************************************/
uint8_t FT800::cmd_execute(uint32_t data)
{
    uint32_t cmdBufferRd = 0;
    uint32_t cmdBufferWr = 0;

    cmdBufferRd = HOST_MEM_RD32(REG_CMD_READ);
    cmdBufferWr = HOST_MEM_RD32(REG_CMD_WRITE);

    uint32_t cmdBufferDiff = cmdBufferWr - cmdBufferRd;

    if( (4096 - cmdBufferDiff) > 4)
    {
        HOST_MEM_WR32(RAM_CMD + cmdBufferWr, data);
        HOST_MEM_WR32(REG_CMD_WRITE, cmdBufferWr + 4);
        return 1;
    }
    return 0;
}

uint8_t FT800::cmd(uint32_t data)
{
    uint8_t tryCount = 255;
    for(tryCount = 255; tryCount > 0; --tryCount)
    {
        if(cmd_execute(data))
        {
            return 1;
        }
    }
    return 0;
}

uint8_t FT800::cmd_ready(void)
{
    uint32_t cmdBufferRd = HOST_MEM_RD32(REG_CMD_READ);
    uint32_t cmdBufferWr = HOST_MEM_RD32(REG_CMD_WRITE);

    return (cmdBufferRd == cmdBufferWr) ? 1 : 0;
}

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/*** Draw Text *********************************************************************/
void FT800::cmd_text(int16_t x, int16_t y, int16_t font, uint16_t options, const char *str)
{
    /*
    	i: data pointer
    	q: str  pointer
    	j: loop counter
    */

    uint16_t i, j, q;
    const uint16_t length = strlen(str);
    if(!length) return ;

    uint32_t *data = (uint32_t *) calloc((length / 4) + 1, sizeof(uint32_t));

    q = 0;
    for(i = 0; i < (length / 4); ++i, q = q + 4)
    {
        data[i] = (uint32_t)str[q + 3] << 24 | (uint32_t)str[q + 2] << 16 | (uint32_t)str[q + 1] << 8 | (uint32_t)str[q];
    }
    for(j = 0; j < (length % 4); ++j, ++q)
    {
        data[i] |= (uint32_t)str[q] << (j * 8);
    }

    cmd(CMD_TEXT);
    cmd( ((uint32_t)y << 16) | (x & 0xffff) );
    cmd( ((uint32_t)options << 16) | (font & 0xffff) );
    for(j = 0; j < (length / 4) + 1; ++j)
    {
        cmd(data[j]);
    }
    free(data);
}
void FT800::cmd_button(int16_t x, int16_t y, int16_t w, int16_t h, int16_t font, uint16_t options, const char *str)
{
    /*
    	i: data pointer
    	q: str  pointer
    	j: loop counter
    */

    uint16_t i, j, q;
    const uint16_t length = strlen(str);
    if(!length) return ;

    uint32_t *data = (uint32_t *) calloc((length / 4) + 1, sizeof(uint32_t));

    q = 0;
    for(i = 0; i < (length / 4); ++i, q = q + 4)
    {
        data[i] = (uint32_t)str[q + 3] << 24 | (uint32_t)str[q + 2] << 16 | (uint32_t)str[q + 1] << 8 | (uint32_t)str[q];
    }
    for(j = 0; j < (length % 4); ++j, ++q)
    {
        data[i] |= (uint32_t)str[q] << (j * 8);
    }

    cmd(CMD_BUTTON);
    cmd( ((uint32_t)y << 16) | (x & 0xffff) );
    cmd( ((uint32_t)h << 16) | (w & 0xffff) );
    cmd( ((uint32_t)options << 16) | (font & 0xffff) );
    for(j = 0; j < (length / 4) + 1; ++j)
    {
        cmd(data[j]);
    }
    free(data);
}
/*** Set FG color ******************************************************************/
void FT800::cmd_fgcolor(uint32_t c)
{
    cmd(CMD_FGCOLOR);
    cmd(c);
}

/*** Set BG color ******************************************************************/
void FT800::cmd_bgcolor(uint32_t c)
{
    cmd(CMD_BGCOLOR);
    cmd(c);
}

/*** Set Gradient color ************************************************************/
void FT800::cmd_gradcolor(uint32_t c)
{
    cmd(CMD_GRADCOLOR);
    cmd(c);
}

/*** Draw Gradient *****************************************************************/
void FT800::cmd_gradient(int16_t x0, int16_t y0, uint32_t rgb0, int16_t x1, int16_t y1, uint32_t rgb1)
{
    cmd(CMD_GRADIENT);
    cmd( ((uint32_t)y0 << 16) | (x0 & 0xffff) );
    cmd(rgb0);
    cmd( ((uint32_t)y1 << 16) | (x1 & 0xffff) );
    cmd(rgb1);
}
