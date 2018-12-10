#ifndef __U_GUI_H
#define __U_GUI_H

extern "C" {
#include "ugui.h"
}
//class LCD_DRIVER{
//public:
//  UG_RESULT _HW_DrawLine( UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c );
//  void _HW_DrawPixel( UG_S16 x, UG_S16 y, UG_COLOR c );
//  UG_RESULT _HW_FillFrame( UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c );
//};

//LCD_DRIVER P;

//void temp_DrawPixel( UG_S16 x, UG_S16 y, UG_COLOR c )
//{
//	P._HW_DrawPixel(x,y,c);
//}

//UG_RESULT temp_DrawLine( UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c )
//{
//	 P._HW_DrawLine(x1,y1,x2,y2,c);
//   return UG_RESULT_OK;
//}

//UG_RESULT temp_FillFrame( UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c )
//{
//	 P._HW_FillFrame(x1,y1,x2,y2,c);
//   return UG_RESULT_OK;
//}

class U_GUI
{
public:
    U_GUI() {};
    //  U_GUI(LCD_DRIVER * lcd,uint16_t x,uint16_t y){
    //		P = *lcd;
    //		Begin(&temp_DrawPixel,(void*)temp_DrawLine,(void*)temp_FillFrame,x,y);
    //	};
    void Begin(void (*p)(UG_S16, UG_S16, UG_COLOR), void *driverLine, void *driverFill, UG_S16 x, UG_S16 y)
    {
        UG_Init(&_gui, p, x, y);

        UG_DriverRegister( DRIVER_DRAW_LINE, driverLine);
        UG_DriverRegister( DRIVER_FILL_FRAME, driverFill);

        UG_DriverEnable( DRIVER_DRAW_LINE );
        UG_DriverEnable( DRIVER_FILL_FRAME );
    };
    // ���
    void FillScreen(UG_COLOR c)
    {
        UG_FillScreen(c);
    };
    void FillFrame( UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c )
    {
        UG_FillFrame(x1, y1, x2, y2, c);
    };
    void FillRoundFrame( UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_S16 r, UG_COLOR c )
    {
        UG_FillRoundFrame(x1, y1, x2, y2, r, c);
    };
    void FillCircle( UG_S16 x0, UG_S16 y0, UG_S16 r, UG_COLOR c )
    {
        UG_FillCircle(x0, y0, r, c);
    };
    // ��ͼ
    void DrawMesh( UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c )
    {
        UG_DrawMesh(x1, y1, x2, y2, c);
    };
    void DrawFrame( UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c )
    {
        UG_DrawFrame(x1, y1, x2, y2, c);
    };
    void DrawRoundFrame( UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_S16 r, UG_COLOR c )
    {
        UG_DrawRoundFrame(x1, y1, x2, y2, r, c);
    };
    void DrawPixel( UG_S16 x0, UG_S16 y0, UG_COLOR c )
    {
        UG_DrawPixel(x0, y0, c);
    };
    void DrawCircle( UG_S16 x0, UG_S16 y0, UG_S16 r, UG_COLOR c )
    {
        UG_DrawCircle(x0, y0, r, c);
    };
    void DrawArc( UG_S16 x0, UG_S16 y0, UG_S16 r, UG_U8 s, UG_COLOR c )
    {
        UG_DrawArc(x0, y0, r, s, c);
    };
    void DrawLine( UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c )
    {
        UG_DrawLine(x1, y1, x2, y2, c);
    };
    void DrawBMP( UG_S16 xp, UG_S16 yp, UG_BMP *bmp )
    {
        UG_DrawBMP(xp, yp, bmp);
    };
    // ����
    void FontSelect(const UG_FONT *font)
    {
        UG_FontSelect(font);
    };
    void FontSetHSpace( UG_U16 s )
    {
        UG_FontSetHSpace(s);
    };
    void FontSetVSpace( UG_U16 s )
    {
        UG_FontSetVSpace(s);
    };
    // �ı����
    void PutString( UG_S16 x, UG_S16 y, char *str )
    {
        UG_PutString(x, y, str);
    };
    void PutChar( char chr, UG_S16 x, UG_S16 y, UG_COLOR fc, UG_COLOR bc )
    {
        UG_PutChar(chr, x, y, fc, bc);
    };
    void ConsolePutString( char *str )
    {
        UG_ConsolePutString(str);
    };
    void ConsoleSetArea( UG_S16 xs, UG_S16 ys, UG_S16 xe, UG_S16 ye )
    {
        UG_ConsoleSetArea(xs, ys, xe, ye);
    };
    void ConsoleSetForecolor( UG_COLOR c )
    {
        UG_ConsoleSetForecolor(c);
    };
    void ConsoleSetBackcolor( UG_COLOR c )
    {
        UG_ConsoleSetBackcolor(c);
    };
private:
    UG_GUI _gui;
};
#endif
