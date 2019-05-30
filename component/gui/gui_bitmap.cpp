#include "graphic.h"
// Draw a 1-bit image (bitmap) at the specified (x,y) position from the
// provided bitmap buffer (must be PROGMEM memory) using the specified
// foreground color (unset bits are transparent).
void Graphic::drawBitmap(int16_t x, int16_t y,
                     const uint8_t *bitmap, int16_t w, int16_t h, uint32_t color)
{

    int16_t i, j, byteWidth = (w + 7) / 8;
    uint8_t byte;

    for(j = 0; j < h; j++)
    {
        for(i = 0; i < w; i++)
        {
            if(i & 7) byte <<= 1;
            else      byte   = *(bitmap + j * byteWidth + i / 8);
            if(byte & 0x80) draw_pixel(x + i, y + j, color);
        }
    }
}

// Draw a 1-bit image (bitmap) at the specified (x,y) position from the
// provided bitmap buffer (must be PROGMEM memory) using the specified
// foreground (for set bits) and background (for clear bits) colors.
void Graphic::drawBitmap(int16_t x, int16_t y,
                     const uint8_t *bitmap, int16_t w, int16_t h, uint32_t color, uint16_t bg)
{

    int16_t i, j, byteWidth = (w + 7) / 8;
    uint8_t byte;

    for(j = 0; j < h; j++)
    {
        for(i = 0; i < w; i++ )
        {
            if(i & 7) byte <<= 1;
            else      byte   = *(bitmap + j * byteWidth + i / 8);
            if(byte & 0x80) draw_pixel(x + i, y + j, color);
            else            draw_pixel(x + i, y + j, bg);
        }
    }
}

// drawBitmap() variant for RAM-resident (not PROGMEM) bitmaps.
void Graphic::drawBitmap(int16_t x, int16_t y,
                     uint8_t *bitmap, int16_t w, int16_t h, uint32_t color)
{

    int16_t i, j, byteWidth = (w + 7) / 8;
    uint8_t byte;

    for(j = 0; j < h; j++)
    {
        for(i = 0; i < w; i++ )
        {
            if(i & 7) byte <<= 1;
            else      byte   = bitmap[j * byteWidth + i / 8];
            if(byte & 0x80) draw_pixel(x + i, y + j, color);
        }
    }
}

// drawBitmap() variant w/background for RAM-resident (not PROGMEM) bitmaps.
void Graphic::drawBitmap(int16_t x, int16_t y,
                     uint8_t *bitmap, int16_t w, int16_t h, uint32_t color, uint16_t bg)
{

    int16_t i, j, byteWidth = (w + 7) / 8;
    uint8_t byte;

    for(j = 0; j < h; j++)
    {
        for(i = 0; i < w; i++ )
        {
            if(i & 7) byte <<= 1;
            else      byte   = bitmap[j * byteWidth + i / 8];
            if(byte & 0x80) draw_pixel(x + i, y + j, color);
            else            draw_pixel(x + i, y + j, bg);
        }
    }
}

//Draw XBitMap Files (*.xbm), exported from GIMP,
//Usage: Export from GIMP to *.xbm, rename *.xbm to *.c and open in editor.
//C Array can be directly used with this function
void Graphic::drawXBitmap(int16_t x, int16_t y,
                      const uint8_t *bitmap, int16_t w, int16_t h, uint32_t color)
{

    int16_t i, j, byteWidth = (w + 7) / 8;
    uint8_t byte;

    for(j = 0; j < h; j++)
    {
        for(i = 0; i < w; i++ )
        {
            if(i & 7) byte >>= 1;
            else      byte   = *(bitmap + j * byteWidth + i / 8);
            if(byte & 0x01) draw_pixel(x + i, y + j, color);
        }
    }
}



