/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox


#include "ebox.h"
#include "math.h"
#include "colorled.h"
#include "nokia5110.h"
#include "ft800.h"



FT800 ft(&PA3, &PA4, &sspi1);

/* Clear Screen */
void clrscr(void)
{
    ft.cmd(CMD_DLSTART);
    ft.cmd(CLEAR_COLOR_RGB(3, 122, 0));
    ft.cmd(CLEAR(12, 12, 1));
    ft.cmd(DISPLAY());
    ft.cmd(CMD_SWAP);
}
/* Demo Screen */
void lcd_start_screen(uint8_t button)
{
    ft.cmd(CMD_DLSTART);
    ft.cmd(CLEAR_COLOR_RGB(0, 0, 0));
    ft.cmd(CLEAR(1, 1, 1));
    ft.cmd_gradient(0, 0, 0xA1E1FF, 0, 250, 0x000080);
    ft.cmd_text(10, 245, 27, 0, "Designed by: Akos Pasztor");
    ft.cmd_text(470, 250, 26, OPT_RIGHTX, "http://akospasztor.com");
    ft.cmd(COLOR_RGB(0xDE, 0x00, 0x08));
    ft.cmd_text(240, 40, 31, OPT_CENTERX, "FT800 Demo");
    ft.cmd(COLOR_RGB(255, 255, 255));
    ft.cmd(TAG(1));

    if(!button)
    {
        ft.cmd_fgcolor(0x228B22);
        ft.cmd_button(130, 150, 220, 48, 28, 0, "Tap to Continue");
    }
    else
    {
        ft.cmd_fgcolor(0x0A520A);
        ft.cmd_button(130, 150, 220, 48, 28, OPT_FLAT, "Tap to Continue");
    }

    ft.cmd(DISPLAY());
    ft.cmd(CMD_SWAP);
}

void setup()
{
    ebox_init();
    PB8.mode(OUTPUT_PP);
    PB8.set();
    uart1.begin(115200);

}
u8 str[] = "123";
int main(void)
{
    setup();
    while(1)
    {
        while(ft.begin(1));
        clrscr();
        lcd_start_screen(0);
        delay_ms(300);

    }

}




