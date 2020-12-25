#ifndef _GB2312ToUnicode_H_BAB
#define _GB2312ToUnicode_H_BAB

/* Includes ------------------------------------------------------------------*/
typedef unsigned char u8; 
typedef unsigned short u16; 
typedef unsigned int u32; 	
typedef char s8; 
typedef short s16; 
typedef int s32; 

extern const u16 UnicodeToGB2312_Tab[][2];


u16 UnicodeToGB2312(u16 unicode);
u16 GB2312ToUnicode(u16 data);
void GB2312ToUnicode_Str(u16 * const pOutUnicodeStr,u8 * const pGB2312Str);
s16 NetworkByteOrder(u8 * const pOut,u16 * const pIn,u16  const nByte);
#endif
