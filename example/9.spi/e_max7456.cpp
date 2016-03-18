
#include "ebox.h"
#include "Max7456.h"


OSD osd(&PB0, &spi1);

void setup()
{
    ebox_init();
    uart1.begin(115200);
    osd.begin(1);


}

int main(void)
{
    setup();

    while(1)
    {
        uart1.printf("ebox max7456 test\n\r");

        osd.set_panel(6, 6);
        osd.open_panel();
        osd.printf("ebox Max7456 test");
        osd.close_panel();
        delay_ms(1000);


    }


}

