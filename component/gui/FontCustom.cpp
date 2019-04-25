#include "GUI.h"
#include "gt30l32s4w.h"

extern FontLib font;

GUI_CHARINFO charInfo[100];

//第一步申请一个字的缓冲区，并从字库中读取，然后加入到charinfo链表中
void get_inner_code_buf(uint16_t icode)
{
}