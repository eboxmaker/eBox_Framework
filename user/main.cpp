/**
  ******************************************************************************
  * @file   : *.cpp
  * @author : shentq
  * @version: V1.2
  * @date   : 2016/08/14

  * @brief   ebox application example .
  *
  * Copyright 2016 shentq. All Rights Reserved.         
  ******************************************************************************
 */

#include "ebox.h"
#include "gui.h"
#include "ds18b20.h"
#include "menu.h"
#include "bsp.h"

Lcd lcd(&PB5, &PB6, &PB4, &PB3, &spi1);
GUI gui(&lcd,128,160);
MenuPage *pMenu = &main_page;

void updata_menu_disp()
{
    gui.fill_screen(BLACK);
    gui.set_cursor(0,5);
    if(pMenu->count == 0)
        pMenu->callback();
    for(int i = 0; i < pMenu->count; i++){
        if(pMenu->select == i)
            gui.set_text_mode(LCD_TEXTMODE_REV);
        else 
            gui.set_text_mode(LCD_DRAWMODE_NORMAL);
        
        gui.printf(pMenu->item[i].name);
        gui.printf("\n");
    }
}
void disp_version()
{
    gui.fill_screen(BLACK);
    gui.set_cursor(70,5);
    gui.printf("version:12.2.4");
}

void setup()
{
    int ret;
    ebox_init();
    lcd.begin(1);
    btn.begin();
    uart1.begin(115200);
    gui.begin();
    gui.fill_screen(BLACK);   
    
    version_page.callback = disp_version;
    
    
    
    gui.set_color(WHITE);
    //gui.set_back_color(RED);
    gui.set_font(&GUI_Font16_ASCII);
updata_menu_disp();
    
    
}
int main(void)
{
    float temper;
    setup();
    while(1)
    {
        btn.loop();
        if(btn.release())
        {
            pMenu->select++;
            pMenu->select = pMenu->select % pMenu->count;
            updata_menu_disp();
        }
        if(btn.pressed_for(2000,1))
        {
            if(pMenu->item[pMenu->select].child != NULL){
                pMenu = pMenu->item[pMenu->select].child;
                updata_menu_disp();
            }

        }
    }

}
