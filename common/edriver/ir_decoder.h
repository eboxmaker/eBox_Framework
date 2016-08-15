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

//��׼Э��ʱ��
#define STARTBIT_uS (13500L)
#define REPEATBIT_uS (11250L)
#define ZERO_uS (1120L)
#define ONE_uS (2250L)
#define NORMAL_BITS 32//����ԭ���ܹ�Ϊ32λ
#define REPEAT_MIN_COUNT 3

//һ΢����ڶ��ٸ�TICK
#define uS_TO_TICKS(_uS)  ((_uS)*1L)

//����׼Э��ʱ��ת��ΪTICK
#define TICKS_ZERO  uS_TO_TICKS(ZERO_uS)
#define TICKS_ONE   uS_TO_TICKS(ONE_uS)
#define TICKS_STARTBIT   uS_TO_TICKS(STARTBIT_uS)
#define TICKS_REPEATBIT   uS_TO_TICKS(REPEATBIT_uS)



//���ʱ����ʹ�õ�������
#define TICKS_STARTBIT_MIN   (TICKS_STARTBIT * 7 / 8)//�ݴ�Ϊ����1/8
#define TICKS_STARTBIT_MAX   (TICKS_STARTBIT * 9 / 8)//�ݴ�Ϊ����1/8

#define TICKS_REPEATBIT_MIN   (TICKS_REPEATBIT * 7 / 8)//�ݴ�Ϊ����1/8
#define TICKS_REPEATBIT_MAX   (TICKS_REPEATBIT * 9 / 8)//�ݴ�Ϊ����1/8

#define TICKS_BIT_MIN (TICKS_ZERO * 3 / 4)//�ݴ�Ϊ����1/4
#define TICKS_BIT_MAX (TICKS_ONE * 5 / 4)//�ݴ�Ϊ����1/4

#define TICKS_BIT_AVG ((TICKS_ONE + TICKS_ZERO) / 2)//ȡЭ��0��1��Ӧʱ�����ֵ�������жϽ���


/*#define RECEIVE_OK(_irrxbuffer) ((_irrxbuffer.bitbuffer.nec.addr ^ _irrxbuffer.bitbuffer.nec.addr_inv) == 0xFF &&   \
                					 (_irrxbuffer.bitbuffer.nec.command ^ _irrxbuffer.bitbuffer.nec.command_inv) == 0xFF)
*/
#define RECEIVE_OK(_irrxbuffer) ((_irrxbuffer.bitbuffer.nec.command ^ _irrxbuffer.bitbuffer.nec.command_inv) == 0xFF)//Э��У���㷨������ԭ��Ͳ����Ƿ�һһ��Ӧ


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


