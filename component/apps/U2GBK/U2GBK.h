#ifndef __U2GBK_H
#define __U2GBK_H

#include "ebox.h"

int GetUtf8ByteNumForWord(uint8_t firstCh);
void Utf8ToGb2312(const char* utf8, int len, uint8_t *temp);



#endif

