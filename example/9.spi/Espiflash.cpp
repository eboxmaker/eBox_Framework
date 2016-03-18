
#include "ebox.h"
#include "w25x16.h"



W25X flash(&PE5, &spi1);


void setup()
{
    ebox_init();
    uart3.begin(9600);
    flash.begin(1);
}


int16_t tmp[7];
uint16_t id;
uint8_t buf[10];
uint8_t wbuf[10];
int main(void)
{
    setup();
    for(int i = 0; i < 10; i++)

        wbuf[i] = i;
    while(1)
    {
        flash.read_id(&id);
        uart3.printf("\r\n==readid=======\r\n");
        uart3.printf("id = %x", id);

        uart3.printf("\r\n==write&read========\r\n");
        flash.write(wbuf, 0, 10);
        flash.read(buf, 0, 10);
        for(int i = 0; i < 10; i++)
            uart3.printf(" %x", buf[i]);

        uart3.printf("\r\n==erase&read========\r\n");
        flash.erase_sector(0);
        flash.read(buf, 1, 10);
        for(int i = 0; i < 10; i++)
            uart3.printf(" %x", buf[i]);
        uart3.printf("\r\n=========================\r\n");
        uart3.printf("\r\n\r\n");

        delay_ms(1000);
    }
}




