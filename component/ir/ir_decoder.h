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
//标准协议时间
#define BIT_START_uS (13500L)
#define BIT_REPEAT_uS (11250L)
#define ZERO_uS (1120L)
#define ONE_uS (2250L)
#define NORMAL_BITS 32//数据原码总共为32位
#define REPEAT_MIN_COUNT 2//控制读取速度



//检测时各自使用的上下限
#define BIT_START_MIN   (BIT_START_uS * 7 / 8)//容错为正负1/8
#define BIT_START_MAX   (BIT_START_uS * 9 / 8)//容错为正负1/8

#define BIT_REPEAT_MIN   (BIT_REPEAT_uS * 7 / 8)//容错为正负1/8
#define BIT_REPEAT_MAX   (BIT_REPEAT_uS * 9 / 8)//容错为正负1/8

#define BIT_MIN (ZERO_uS * 3 / 4)//容错为正负1/4
#define BIT_MAX (ONE_uS * 5 / 4)//容错为正负1/4

#define BIT_AVG ((ZERO_uS + ONE_uS) / 2)//取协议0和1对应时间的中值，划分判断界限




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
    } IrCode_t;//接收后存储的遥控码
    IrCode_t resault;

    typedef enum
    {
        STATE_START = 0,
        STATE_HEADER,
        STATE_DATA,
    } State_t;//接收过程状态机
    
    
    
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
    } BitBuffer_t;//用于接收过程单bit存储处理

    
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


