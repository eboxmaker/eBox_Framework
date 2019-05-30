#include "ebox.h"
#include "bsp_ebox.h"
#include "gui_button.h"
#include "gui_sidebar.h"
#include "ui_main.h"
#include "ui.h"

       GuiMessage msg;

/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"Virtual LCD example"
#define EXAMPLE_DATE	"2018-08-14"

//Lcd lcd(&PB5,&PB6,&PB4,&PB3,&spi1);
//Graphic gpu(&lcd,128,160);
void click(Object *sender)
{
}

void setup()
{
    ebox_init();
    UART.begin(115200);
    UART.setTimeout(10);
    print_log(EXAMPLE_NAME,EXAMPLE_DATE);
    lcd.begin();
    lcd.clear(BLUE);
    gpu.begin();
    gpu.clear();
    gpu.set_color(GREEN);

    gpu.set_rotation(0);
    
    ui.begin(pageMain);

    //pageMain.create();
    btn.event_click = click;
    btn.begin();
    manager.add(&btn);
}
uint32_t start ,stop;
#include "ebox_mem.h"


int main(void)
{
    int count = 32;
    setup();
    while (1)
    {

        manager.loop();
        start = millis();

        
        msg.str = UART.readString();
        if(msg.str != "")
        {
            ui.event(&msg,&msg);
            UART.printf("free:%d\r\n",ebox_get_free());
        }
        
        stop=millis();
    }
}