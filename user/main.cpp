#include "ebox.h"
#include "ADS8866.h"

#include "calendar.h"
Flash flash;//创建一个内部flash读写对象

Calendar clock;

Ads8866 adc(&PA0,&PA1,&PA2);
uint8_t wbuf[10];
uint8_t rbuf[10];
char strx[] = "165536";
int a = 'a',b = 'b';
int num;


void setup()
{
    ebox_init();
    uart1.begin(115200);
    uart1.println();
    adc.begin();
    clock.begin();
    clock.set(17,12,8,18,41,00);
    
    num = atoi(strx,10);
    uart1.printf("%d\r\n",num);
    swap(&a,&b);
        uart1.printf("a= %c,b=%c",a,b);
    while(1);

}
int main(void)
{
    setup();

    random_seed(10);
    int i = 0 ;
    while(1)
    {
        uart1.printf(clock.get_unix_timestamp_to_string().c_str());
        uart1.printf("\t%1.3f\r\n",adc.read()*4.999/65536.0);

        clock.sec_process();
        i++;
        delay_ms(1000);
    }
}
