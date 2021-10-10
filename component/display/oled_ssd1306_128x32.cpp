#include "oled_ssd1306_128x32.h"
#include "math.h"

#define U8G_ESC_DLY(x) 255, ((x) & 0x7f)
#define U8G_ESC_CS(x) 255, (0xd0 | ((x)&0x0f))
#define U8G_ESC_ADR(x) 255, (0xe0 | ((x)&0x0f))
#define U8G_ESC_RST(x) 255, (0xc0 | ((x)&0x0f))
#define U8G_ESC_VCC(x) 255, (0xbe | ((x)&0x01))
#define U8G_ESC_END 255, 254
#define U8G_ESC_255 255, 255


uint8_t disp_buffer[ SSD1306_LCDWIDTH * SSD1306_LCDHEIGHT / (8/SSD1306_BITS_PER_PIXEL) ] = {0};

void OledSSD1306_128x32::draw_pixel(int16_t x, int16_t y,uint32_t color)
{
    if(x < 0 || x >= SSD1306_LCDWIDTH)return;
    if(y < 0 || y >= SSD1306_LCDHEIGHT)return;
    
    color &= 0x0f;
#ifdef SSD1306_128_32_4 // 4 bits per pixel
	register uint8_t mask = ((x % 2) ? color : color << 4);
	register uint8_t *pBuf = &disp_buffer[(x >> 1) + (y * (SSD1322_LCDWIDTH / 2))];
	register uint8_t b1 = *pBuf;
	b1 &= (x % 2) ? 0xF0 : 0x0F; // cleardown nibble to be replaced
	// write our value in
	*pBuf++ = b1 | mask;
#endif

#ifdef SSD1306_128_32_1 // 1 bit per pixel
    uint32_t index = x  + (y/8) * (SSD1306_LCDWIDTH );
    register uint8_t *pBuf = &disp_buffer[index];
    if(color > 0)*pBuf |=  (0x1 << (y%8));
    else *pBuf &= ~(0x1 << (y%8));
#endif
}


/* init sequence Univision datasheet (NOT TESTED) */
static const uint8_t u8g_dev_ssd1306_128x32_univision_init_seq[] = {
  U8G_ESC_CS(0),        /* disable chip */
  U8G_ESC_ADR(0),       /* instruction mode */
  U8G_ESC_RST(1),       /* do reset low pulse with (1*16)+2 milliseconds */
  U8G_ESC_CS(1),        /* enable chip */

  0x0ae,				/* display off, sleep mode */
  0x0d5, 0x080,			/* clock divide ratio (0x00=1) and oscillator frequency (0x8) */
  0x0a8, 0x03f,			/* multiplex ratio */
  0x0d3, 0x000,			/* display offset */
  0x040,				/* start line */
  0x08d, 0x010,			/* charge pump setting (p62): 0x014 enable, 0x010 disable */
  0x0a1,				/* segment remap a0/a1*/
  0x0c8,				/* c0: scan dir normal, c8: reverse */
  0x0da, 0x012,			/* com pin HW config, sequential com pin config (bit 4), disable left/right remap (bit 5) */
  0x081, 0x09f,			/* set contrast control */
  0x0d9, 0x022,			/* pre-charge period */
  0x0db, 0x040,			/* vcomh deselect level */
  0x022, 0x000,			/* page addressing mode WRONG: 3 byte cmd! */
  0x0a4,				/* output ram to display */
  0x0a6,				/* none inverted normal display mode */
  0x0af,				/* display on */
  U8G_ESC_CS(0),        /* disable chip */
  U8G_ESC_END           /* end of sequence */
};

uint8_t OledSSD1306_128x32::WriteEscSeqP(const uint8_t *esc_seq)
{
 
  return 1;
}
bool OledSSD1306_128x32::begin(I2c::Speed_t speed)
{
    
    buffer_size = SSD1306_LCDWIDTH * SSD1306_LCDHEIGHT / (8/SSD1306_BITS_PER_PIXEL);
    width = SSD1306_LCDWIDTH;
    hight = SSD1306_LCDHEIGHT;
    
    i2c->begin();
    i2c->setClock(speed);
    
   write_cmd((byte) 0xAE); //关显示
 
	write_cmd((byte) 0xD5); //设置显示始终 分比率
	write_cmd((byte) 0x80); //建议比率0x80
 
	write_cmd((byte) 0xA8); //设置MUX比率
	write_cmd((byte) 0x1F); //设置为0x1F，即十进制31
 
	write_cmd((byte) 0xD3); //设置显示补偿
	write_cmd((byte) 0x00); //no offset
	write_cmd((byte) (0x40 | 0x0)); //line #0
 
	write_cmd((byte) 0x8D); //是否使用电源
	write_cmd((byte) 0x14); //使用外置电源，固定值，见ssd1306文档
 
	write_cmd((byte) 0x20); //设置内存地址模式
	write_cmd((byte) 0x00); //水平地址模式
 
	write_cmd((byte) (0xA0 | 0x1)); //实在没搞懂这个是什么用处的
 
	write_cmd((byte) 0xC8); //设置列输出扫描方向,Scan from COM[N-1] to COM0
	write_cmd((byte) 0xDA); //设置列引脚硬件配置
	write_cmd((byte) 0x02); //上文已经描述该字段
 
	write_cmd((byte) 0x81); //设置对比度
	write_cmd((byte) 0x8F);
 
	write_cmd((byte) 0xD9); //设置预充电周期
	write_cmd((byte) 0xF1);
	write_cmd((byte) 0xDB); //设置VCOMH反压值
	write_cmd((byte) 0x40);
	write_cmd((byte) 0xA4); //启用输出GDDRAM中的数据
	write_cmd((byte) 0xA6); //设置正常显示，A7表示反转显示
 
	write_cmd((byte) 0xAF);//--turn on oled panel

    write_cmd( 0xAF); //--turn on oled panel
    return true;
}
uint8_t OledSSD1306_128x32::write_byte(uint8_t value)
{
    i2c->beginTransmission(SlaveAddr);
    i2c->write(value);
    return i2c->endTransmission();
}
void OledSSD1306_128x32::write_cmd(uint8_t cmd)
{
    i2c->beginTransmission(SlaveAddr);
    i2c->write(0);
    i2c->write(cmd);
    i2c->endTransmission();
}
void OledSSD1306_128x32::write_data(uint8_t data)
{
    i2c->beginTransmission(SlaveAddr);
    i2c->write(0x40);
    i2c->write(data);
    i2c->endTransmission();

}

void OledSSD1306_128x32::fill_screen(uint32_t color)
{
    color = (color>0)?0xff:0;
    for(uint32_t i = 0; i < buffer_size; i++)
        disp_buffer[i] = color & 0xff;
}
void OledSSD1306_128x32::flush()
{
    uint8_t i, n;

    for(i = 0; i < 8; i++)
    {
        write_cmd(0xb0 + i); //设置页地址（0~7）
        write_cmd(0X00);//设置显示位置—列低地址
        write_cmd(0X10);//设置显示位置—列高地址
        for(n = 0; n < 128; n++)
        {
            write_data(disp_buffer[i*128 + n]);
        }
    } //更新显示
}

//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!
void OledSSD1306_128x32::clear(void)
{
    uint8_t i, n;
    for(i = 0; i < 8; i++)
    {
        write_cmd(0xb0 + i); //设置页地址（0~7）
        write_cmd(0X00);//设置显示位置—列低地址
        write_cmd(0X10);//设置显示位置—列高地址
        for(n = 0; n < 128; n++)
            write_data(0X00);
    } //更新显示
}
void OledSSD1306_128x32::on(void)
{
    uint8_t i, n;
    for(i = 0; i < 8; i++)
    {
        write_cmd(0xb0 + i);
        write_cmd(0X00);
        write_cmd(0X10);
        for(n = 0; n < 128; n++)
            write_data(0xff);
        for(n = 0; n < 128; n++)
            write_data(0x01);
    } //更新显示
}

//开启OLED显示
void OledSSD1306_128x32::display_on(void)
{
    write_cmd(0X8D);
    write_cmd(0X14);
    write_cmd(0XAF);
}
//关闭OLED显示
void OledSSD1306_128x32::display_off(void)
{
    write_cmd(0X8D);
    write_cmd(0X10);
    write_cmd(0XAE);
}

void OledSSD1306_128x32::set_pos(unsigned char x, unsigned char y)
{
    write_cmd(0xb0 + y);
    write_cmd(((x & 0xf0) >> 4) | 0x10);
    write_cmd(x & 0x0f);
}

void OledSSD1306_128x32::draw_bmp(unsigned char x0, unsigned char y0, unsigned char BMP[], unsigned char w, unsigned char h )
{
    for(int i = 0; i < w ; i++)
    {
        for(int j = 0; j < h ; j++)
            draw_pixel(x0 + i,y0 + j,BMP[i*h + j]);
    }

}
