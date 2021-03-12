#ifndef  __IR_DECODER_H
#define  __IR_DECODER_H
/*
 * ir_decoder.h
 *
 * Version 1.1
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
 *
 */


#include "ebox_core.h"
#include "ebox_in_capture.h"
#include "ringbuf.h"
//��׼Э��ʱ��
#define BIT_START_uS (13500L)
#define BIT_REPEAT_uS (11250L)
#define ZERO_uS (1120L)
#define ONE_uS (2250L)
#define NORMAL_BITS 32//����ԭ���ܹ�Ϊ32λ
#define REPEAT_MIN_COUNT 2//���ƶ�ȡ�ٶ�



//���ʱ����ʹ�õ�������
#define BIT_START_MIN   (BIT_START_uS * 7 / 8)//�ݴ�Ϊ����1/8
#define BIT_START_MAX   (BIT_START_uS * 9 / 8)//�ݴ�Ϊ����1/8

#define BIT_REPEAT_MIN   (BIT_REPEAT_uS * 7 / 8)//�ݴ�Ϊ����1/8
#define BIT_REPEAT_MAX   (BIT_REPEAT_uS * 9 / 8)//�ݴ�Ϊ����1/8

#define BIT_MIN (ZERO_uS * 3 / 4)//�ݴ�Ϊ����1/4
#define BIT_MAX (ONE_uS * 5 / 4)//�ݴ�Ϊ����1/4

#define BIT_AVG ((ZERO_uS + ONE_uS) / 2)//ȡЭ��0��1��Ӧʱ�����ֵ�������жϽ���




class IrDecoder
{
    public:
    typedef	union
    {
        uint32_t all;
        struct
        {
            uint8_t value;
            uint8_t is_repeat;
            uint8_t addr;
        } code;
    } IrCode_t;//���պ�洢��ң����
    IrCode_t resault;

    typedef enum
    {
        STATE_START = 0,
        STATE_HEADER,
        STATE_DATA,
    } State_t;//���չ���״̬��
    
    
    
    typedef union
    {
        uint32_t all;
        struct
        {
            uint8_t addr;
            uint8_t addr_inv;
            uint8_t command;
            uint8_t command_inv;
        } byte;
    } BitBuffer_t;//���ڽ��չ��̵�bit�洢����

    
    public:
        IrDecoder(Gpio &io);
        void begin(void);
        int available(void);
        IrCode_t read(void);
    
    private:
        void handler( void );
        bool check(BitBuffer_t &bits);
    

    private:
        void init(void);

        InCapture *ic;
        RingBufUint32 ring;
    
        BitBuffer_t bitBuffer;
        IrCode_t codeBuffer;
    
        State_t state;
        //counted bits
        uint8_t bit_count;
        //repeat counted bits
        uint8_t repeatbit_count;
        //Last received code
//        uint8_t last_received;
};

#endif


