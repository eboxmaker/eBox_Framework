#include "ebox.h"
int  _Init(void);
void _On();
void _Off();

void _SetPixelIndex(u16 x,u16 y,int PixelIndex);
void _DrawHLine(int x0, int y,  int x1);
void _DrawVLine(int x0, int y,  int x1);
void _FillRect(int x0, int y0, int x1, int y1);