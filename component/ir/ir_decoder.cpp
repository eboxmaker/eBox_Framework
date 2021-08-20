#include "ir_decoder.h"

#if EBOX_DEBUG
// �Ƿ��ӡ������Ϣ, 1��ӡ,0����ӡ
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
    if(ic->polarity == InCapture::Falling)//��������������أ�˵���ողɼ������½���
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

