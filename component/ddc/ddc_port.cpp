#include "ddc_port.h"
#include "ebox.h"
struct ebox_fifo *fifo_recv;


void ddc_init()
{
    fifo_recv = ebox_fifo_alloc(DDC_MAX_FRAME_LENGTH * 10);

}
void ddc_get_char(uint8_t ch)
{
    ebox_fifo_put(fifo_recv, &ch, 1);
}
uint16_t ddc_send_char(uint8_t ch)
{
    /**************
        user add your send api.
    **************/
    return 1;
}
uint16_t ddc_send_buf(uint8_t *buf, uint16_t buflen)
{
    /**************
        user add your send api.
    **************/
    //    for(int i = 0; i < buflen;i++)
    //        uart1.printf("0X%02X ",*buf++);
    //    uart1.println();
    uart1.write(buf, buflen);
    return buflen;
}
