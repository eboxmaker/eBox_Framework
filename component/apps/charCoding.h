#ifndef __CHARCODING_H
#define __CHARCODING_H
#include "ebox_type.h"


int UnicodeToUtf8(char* pInput, char *pOutput);
int Utf8ToUnicode(char* pInput, char* pOutput);
int Utf8ToStr(char* pInput, char* pOutput);

#endif
