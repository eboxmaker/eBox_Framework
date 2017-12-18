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
ʹ��˵����
1.ȷ�����չܵ��������ӵ�����ȷ�Ķ�ʱ��ĳ��ͨ���ϵ����š����޸�IN_CAPTURE ic(&GPIO);Ϊ��ȷ��ֵ��
2.���������д���һ��IR_DECODER ir_decoder;����
3.����begin������ʼ��״̬�������벶��ͨ��
4.��ѭ������available������Ƿ��кϷ��İ���ֵ������е���read��������ȡ������ֵ��
5.read�õ�һ��uint16���͵���ֵ���߰�λΪ�Ƿ�Ϊ�ظ����գ��ڰ�λΪ���հ�����ֵ��
ע�⣺һ����ĳ����ʱ����ʼ��Ϊ���벶��ͨ������ö�ʱ��������ͨ��ֻ�ܹ��������벶��ģʽ��
       ���Ҳ��������µĶ�ʱ��ʱ�Ӻͷ�Ƶϵ�������籾���̷�Ƶϵ��Ϊ72,����ͨ��������72��Ƶ��
       ��Ȼ���ö�ʱ������ͨ��������������Ϊ����ģʽ��
*/
//#define DEBUG

InCapture ic(&PB0);//���벶��ͨ��

///Internal buffer and state
static volatile irrx_internal_t irrx_internal;
///Contains the currently received bits
static volatile irrx_code_u irrx_receivebuffer;

#define IRRX_BUFFER_SIZE 16

//���λ���������/////////////////////////////////////////
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
//���λ������������
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

