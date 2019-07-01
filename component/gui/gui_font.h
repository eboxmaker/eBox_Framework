#ifndef __GUI_FONT_H
#define __GUI_FONT_H
#include "ebox_core.h"

typedef struct
{
    uint8_t XSize;
    uint8_t XDist;
    uint8_t BytesPerLine;
    const unsigned char   *pData;
} GUI_CHARINFO;

typedef struct GUI_FONT_PROP
{
    uint16_t First;                                /* first character               */
    uint16_t Last;                                 /* last character                */
    const GUI_CHARINFO   *paCharInfo;            /* address of first character    */
    const struct GUI_FONT_PROP   *pNext;        /* pointer to next */
} GUI_FONT_PROP;

typedef struct _tFont
{
    const GUI_FONT_PROP *list;
    uint16_t    YSize;
    uint8_t     YDist;
} GUI_FONT;

/*************************************
 * GUI FONTS
*************************************/
extern const GUI_FONT GUI_Font8_ASCII;
extern const GUI_FONT GUI_Font16_ASCII;
extern const GUI_FONT GUI_Font24_ASCII;
extern const GUI_FONT GUI_Font32_ASCII;
extern const GUI_FONT GUI_FontHZ24x36;
extern const GUI_FONT GUI_FontHZ16X16;
extern const GUI_FONT GUI_FontHZ20x20;
extern const GUI_FONT GUI_FontD32;
extern const GUI_FONT GUI_FontD48;
extern const GUI_FONT GUI_FontD36x48;
extern const GUI_FONT GUI_FontF32B_ASCII;


typedef enum{
    FONT_INNER,
    FONT_ONLY_HZ_EXTERN,
    FONT_ONLY_ASCII_EXTERN,
    FONT_ALL_EXTERN
}FontSelect_t;



#endif

