#include "ir_decoder.h"

#if EBOX_DEBUG
// 是否打印调试信息, 1打印,0不打印
#define EBOX_DEBUG_IR_ENABLE       true
#define EBOX_DEBUG_IR_ENABLE_ERR   true
#endif


#if EBOX_DEBUG_IR_ENABLE
#define irDebug(...)  ebox_printf("[ir DBG]:%d: ",__LINE__),ebox_printf(__VA_ARGS__ )
#else
#define irSpiDebug(...)
#endif

#if EBOX_DEBUG_IR_ENABLE_ERR
#define irSpiDebugErr(fmt, ...)  ebox_printf("[ir err]:%d: " fmt "\n", __LINE__, __VA_ARGS__)
#else
#define irSpiDebugErr(fmt, ...)
#endif

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


IrDecoder::IrDecoder(Gpio &io)
{
    this->ic =  new InCapture(&io);

}

void IrDecoder::begin(void)
{
    ring.begin(16);
    
    ic->begin(72);
    ic->attach(this,&IrDecoder::handler);//
    ic->set_polarity_falling();
    state = STATE_START;

}

void IrDecoder::handler( void )
{
    ic->complex_event();
    float time ;
    if(ic->polarity == InCapture::Falling)//如果现在是上升沿，说明刚刚采集的是下降沿
        return;
    
    
    ic->update_resault();
    time = ic->res.peroid;
    irDebug("%ms:0.3f",ic->res.peroid/1000.0);
    switch ((uint8_t)state)
    {

        case STATE_START:
        {
            state = STATE_HEADER;  //is first falling edge
            break;
        }
        case STATE_HEADER:
            if(BIT_START_MIN < time && time < BIT_START_MAX)
            {
                state = STATE_DATA;
                bit_count = 0;
                repeatbit_count = 0;
                break;
            }
            else if(BIT_REPEAT_MIN < time && time < BIT_REPEAT_MAX)
            {
                //is repeat
                if(repeatbit_count == REPEAT_MIN_COUNT)
                {
                    repeatbit_count = 0;
                    codeBuffer.code.is_repeat = 1;
                    state = STATE_START;
                    ring.write(codeBuffer.all);
                }
                else
                {
                    repeatbit_count ++;
                }
                break;
            }
            break;
        case STATE_DATA:
            //Within a valid range?
            if (BIT_MIN < time && time < BIT_MAX)
            {
                bit_count++;
                bitBuffer.all >>= 1; //Make room for new bit

                //0 or 1?
                if (time > BIT_AVG)
                {
                    bitBuffer.all |= ((uint32_t)1L << ((uint32_t)NORMAL_BITS - 1));
                }
                else
                {
                    //0
                }
                //Last bit?
                if (bit_count >= NORMAL_BITS)
                {

                    //Check that ir code looks ok (depends on remoters that what IRRX_RECEIVE_OK() means^-^)
                    if (check(bitBuffer))
                    {
                        codeBuffer.code.value = bitBuffer.byte.command;
                        codeBuffer.code.is_repeat = 0;
                        codeBuffer.code.addr = bitBuffer.byte.addr;
                        ring.write(codeBuffer.all);

                    }
                    else
                    {
                        codeBuffer.code.value = 0xff;
                        codeBuffer.code.is_repeat = 0;
                    }
                    state = STATE_HEADER;
                }
            }
            break;
        default:
            state = STATE_HEADER;
            break;
    }
}
bool IrDecoder::check(BitBuffer_t &bits)
{
    return ((bits.byte.command ^ bits.byte.command_inv) == 0xFF);
}

int IrDecoder::available(void)
{
    return (int)ring.available();
}

IrDecoder::IrCode_t IrDecoder::read(void)
{
    resault.all = ring.read();
    return resault;
}

