#include "ir_decoder.h"
/*
 * ir_decoder.cpp
 *
 * Version 1.3
 *
 * NEC IR decoder library for eBOX.
 *
 * See
 *
 * License:   GPL v3 (http://www.gnu.org/licenses/gpl.html)
 * Datasheet:
 *
 * Modification History:
 * -Link                                  - Version 1.0 (2015/12/28)
 * -Link                                  - Version 1.1 (2016/01/03)
 *       * use swith case function instead of if
 * -Link                                  - Version 1.2 (2016/01/10)
 * 			 * use class IC_CAPTURE instead of timer
 *-shentq                                 - Version 1.3 (2016/03/09)
 *           * update class IC_CAPTURE
 */

/*
使用说明：
1.确定接收管的引脚链接到了正确的定时器某个通道上的引脚。并修改IN_CAPTURE ic(&GPIO);为正确的值。
2.在主函数中创建一个IR_DECODER ir_decoder;对象。
3.调用begin函数初始化状态机和输入捕获通道
4.主循环调用available，检测是否有合法的按键值。如果有调用read方法，获取按键的值。
5.read得到一个uint16类型的数值，高八位为是否为重复接收，第八位为接收按键的值。
注意：一但将某个定时器初始化为输入捕获通道，则该定时器的其他通道只能工作在输入捕获模式，
       并且不能设置新的定时器时钟和分频系数。比如本例程分频系数为72,其他通道必须是72分频。
       当然，该定时的其他通道更不能再配置为其他模式。
*/
//#define DEBUG

InCapture ic(&PB0);//输入捕获通道

///Internal buffer and state
static volatile irrx_internal_t irrx_internal;
///Contains the currently received bits
static volatile irrx_code_u irrx_receivebuffer;

#define IRRX_BUFFER_SIZE 16

//环形缓冲区代码/////////////////////////////////////////
struct irrx_ring_buffer
{
    uint16_t buffer[IRRX_BUFFER_SIZE];
    volatile uint16_t head;
    volatile uint16_t tail;
};
static irrx_ring_buffer rx_buffer = { { 0 }, 0, 0};


inline void store_value(uint16_t c)
{
    int i = (unsigned int)(rx_buffer.head + 1) % IRRX_BUFFER_SIZE;

    // if we should be storing the received character into the location
    // just before the tail (meaning that the head would advance to the
    // current location of the tail), we're about to overflow the buffer
    // and so we don't write the character or advance the head.
    if (i != rx_buffer.tail)
    {
        rx_buffer.buffer[rx_buffer.head] = c;
        rx_buffer.head = i;
    }
}

int IR_DECODER::available(void)
{
    return (int)(IRRX_BUFFER_SIZE + rx_buffer.head - rx_buffer.tail) % IRRX_BUFFER_SIZE;
}

int IR_DECODER::read(void)
{
    // if the head isn't ahead of the tail, we don't have any characters
    if (rx_buffer.head == rx_buffer.tail)
    {
        return -1;
    }
    else
    {
        uint16_t c = rx_buffer.buffer[rx_buffer.tail];
        rx_buffer.tail = (unsigned int)(rx_buffer.tail + 1) % IRRX_BUFFER_SIZE;
        return c;
    }
}
//环形缓冲区代码结束
//////////////////////////////////////////////////////////////////////////////////////////

void ir_decoder_Handler( void )
{
    uint32_t ticks ;
    ticks = ic.get_capture();  //get capture value

#ifdef DEBUG
    uart1.printf("s:%d,t:%d\r\n", irrx_internal.irrx_state, ticks);
#endif

    switch (irrx_internal.irrx_state)
    {

    case STATE_START:
    {
        irrx_internal.irrx_state = STATE_HEADER;  //is first falling edge
        break;
    }
    case STATE_HEADER:
    {

        if(TICKS_STARTBIT_MIN < ticks && ticks < TICKS_STARTBIT_MAX)
        {
            //is header
            irrx_internal.irrx_state = STATE_DATA;
            irrx_internal.bitcount = 0;
            irrx_internal.repeatbitcount = 0;

            break;
        }
        else if(TICKS_REPEATBIT_MIN < ticks && ticks < TICKS_REPEATBIT_MAX)
        {
            //is repeat
            if(irrx_internal.repeatbitcount == REPEAT_MIN_COUNT)
            {
                irrx_internal.repeatbitcount = 0;
                irrx_receivebuffer.ir_code.is_repeat = 1;
                irrx_internal.irrx_state = STATE_START;
                store_value(irrx_receivebuffer.all);
            }
            else
            {
                irrx_internal.repeatbitcount ++;
            }
            break;
        }
        break;
    }//end of case IRRX_STATE_HEADER
    case STATE_DATA:
    {
        //Within a valid range?
        if (TICKS_BIT_MIN < ticks && ticks < TICKS_BIT_MAX)
        {
            irrx_internal.bitcount++;
            irrx_internal.bitbuffer.all >>= 1; //Make room for new bit

            //0 or 1?
            if (ticks > TICKS_BIT_AVG)
            {
                irrx_internal.bitbuffer.all |= ((uint32_t)1L << ((uint32_t)NORMAL_BITS - 1));
            }
            else
            {
                //0
            }
            //Last bit?
            if (irrx_internal.bitcount >= NORMAL_BITS)
            {

                //Check that ir code looks ok (depends on remoters that what IRRX_RECEIVE_OK() means^-^)
                if (RECEIVE_OK(irrx_internal))
                {
                    irrx_receivebuffer.ir_code.irrx_value = irrx_internal.bitbuffer.nec.command;
                    irrx_receivebuffer.ir_code.is_repeat = 0;
                    store_value(irrx_receivebuffer.all);

                }
                else
                {
                    irrx_receivebuffer.ir_code.irrx_value = 0xff;
                    irrx_receivebuffer.ir_code.is_repeat = 0;
                }
                irrx_internal.irrx_state = STATE_HEADER;
            }
        }
        break;
    }  //end of case IRRX_STATE_DATA
    default:
        irrx_internal.irrx_state = STATE_HEADER;
        break;
    }
}

void IR_DECODER::begin(void)
{
    ic.begin(72);
    ic.attach(ir_decoder_Handler);//
    ic.set_polarity_falling();
    irrx_internal.irrx_state = STATE_START;


}

