#include "stdio.h"

typedef unsigned char u8_t;
typedef signed char s8_t;
typedef unsigned short u16_t;
typedef signed short s16_t;
typedef unsigned long u32_t;
typedef signed long s32_t;

typedef u32_t mem_ptr_t;

/*************CPU������ݶ���ΪС��ģʽ******************/
#ifndef BYTE_ORDER
#define BYTE_ORDER LITTLE_ENDIAN
#endif

/**********ʹ��KEIL/RVMDK��������ʱ�ĺ궨��**************/
//#if defined(__arm__) && defined(__ARMCC_VERSION)
#define PACK_STRUCT_BEGIN __packed
#define PACK_STRUCT_STRUCT
#define PACK_STRUCT_END
#define PACK_STRUCT_FIELD(x) x
/*---define (sn)printf formatters for these lwip types, for lwip DEBUG/STATS--*/

#define U16_F "4d"
#define S16_F "4d"
#define X16_F "4x"
#define U32_F "8ld"
#define S32_F "8ld"
#define X32_F "8lx"

#ifndef LWIP_PLATFORM_ASSERT
#define LWIP_PLATFORM_ASSERT(x) \
do \
{ ebox_printf("Assertion \"%s\" failed at line %d in %s\n", x, __LINE__, __FILE__); \
} while(0)
#endif

#ifndef LWIP_PLATFORM_DIAG
#define LWIP_PLATFORM_DIAG(x) do {ebox_printf x;} while(0)
#endif

