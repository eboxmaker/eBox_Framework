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


#include "ebox.h"

//标准协议时间
#define STARTBIT_uS (13500L)
#define REPEATBIT_uS (11250L)
#define ZERO_uS (1120L)
#define ONE_uS (2250L)
#define NORMAL_BITS 32//数据原码总共为32位
#define REPEAT_MIN_COUNT 3

//一微秒等于多少个TICK
#define uS_TO_TICKS(_uS)  ((_uS)*1L)

//将标准协议时间转换为TICK
#define TICKS_ZERO  uS_TO_TICKS(ZERO_uS)
#define TICKS_ONE   uS_TO_TICKS(ONE_uS)
#define TICKS_STARTBIT   uS_TO_TICKS(STARTBIT_uS)
#define TICKS_REPEATBIT   uS_TO_TICKS(REPEATBIT_uS)



//检测时各自使用的上下限
#define TICKS_STARTBIT_MIN   (TICKS_STARTBIT * 7 / 8)//容错为正负1/8
#define TICKS_STARTBIT_MAX   (TICKS_STARTBIT * 9 / 8)//容错为正负1/8

#define TICKS_REPEATBIT_MIN   (TICKS_REPEATBIT * 7 / 8)//容错为正负1/8
#define TICKS_REPEATBIT_MAX   (TICKS_REPEATBIT * 9 / 8)//容错为正负1/8

#define TICKS_BIT_MIN (TICKS_ZERO * 3 / 4)//容错为正负1/4
#define TICKS_BIT_MAX (TICKS_ONE * 5 / 4)//容错为正负1/4

#define TICKS_BIT_AVG ((TICKS_ONE + TICKS_ZERO) / 2)//取协议0和1对应时间的中值，划分判断界限


/*#define RECEIVE_OK(_irrxbuffer) ((_irrxbuffer.bitbuffer.nec.addr ^ _irrxbuffer.bitbuffer.nec.addr_inv) == 0xFF &&   \
                					 (_irrxbuffer.bitbuffer.nec.command ^ _irrxbuffer.bitbuffer.nec.command_inv) == 0xFF)
*/
#define RECEIVE_OK(_irrxbuffer) ((_irrxbuffer.bitbuffer.nec.command ^ _irrxbuffer.bitbuffer.nec.command_inv) == 0xFF)//协议校验算法，验算原码和补码是否一一对应


//#define RECEIVE_OK(_irrxbuffer) (1)
typedef enum _IRRX_STATE
{
    STATE_START = 0,
    STATE_HEADER,
    STATE_DATA,
} IRRX_STATE;

/**
 * Ir code received
 */
typedef	union
{
    uint16_t all;
    struct
    {
        uint8_t irrx_value;
        uint8_t is_repeat;
    } ir_code;
} irrx_code_u;


///Internal stuff
typedef struct
{
    IRRX_STATE irrx_state;
    //counted bits
    uint8_t bitcount;
    //repeat counted bits
    uint8_t repeatbitcount;
    //Last received code
    uint8_t last_received;
    union
    {
        uint32_t all;
        struct
        {
            uint8_t addr;
            uint8_t addr_inv;
            uint8_t command;
            uint8_t command_inv;
        } nec;
    } bitbuffer;
} irrx_internal_t;





class IR_DECODER
{
public:
    void begin(void);
    int available(void);
    int read(void);

private:
    void init(void);



};

#endif


