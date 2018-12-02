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
UG_WINDOW* _wnd;
UG_U8			 _bID;		// button id
UG_U8			 _cID;		// checkbox id

class U_GUI{
public:
  U_GUI(){};
//  U_GUI(LCD_DRIVER * lcd,uint16_t x,uint16_t y){
//		P = *lcd;
//		Begin(&temp_DrawPixel,(void*)temp_DrawLine,(void*)temp_FillFrame,x,y);
//	};
  void Begin(void (*p)(UG_S16,UG_S16,UG_COLOR),void* driverLine,void* driverFill,UG_S16 x, UG_S16 y){
    UG_Init(&_gui,p,x,y);
    
    UG_DriverRegister( DRIVER_DRAW_LINE, driverLine);
    UG_DriverRegister( DRIVER_FILL_FRAME, driverFill);
    
    UG_DriverEnable( DRIVER_DRAW_LINE );
    UG_DriverEnable( DRIVER_FILL_FRAME );
  };
  // 填充
  void FillScreen(UG_COLOR c){UG_FillScreen(c);};
	void FillFrame( UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c ){UG_FillFrame(x1,y1,x2,y2,c);};
	void FillRoundFrame( UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_S16 r, UG_COLOR c ){UG_FillRoundFrame(x1,y1,x2,y2,r,c);};
	void FillCircle( UG_S16 x0, UG_S16 y0, UG_S16 r, UG_COLOR c ){UG_FillCircle(x0,y0,r,c);};
	// 画图
	void DrawMesh( UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c ){UG_DrawMesh(x1,y1,x2,y2,c);};
	void DrawFrame( UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c ){UG_DrawFrame(x1,y1,x2,y2,c);};
	void DrawRoundFrame( UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_S16 r, UG_COLOR c ){UG_DrawRoundFrame(x1,y1,x2,y2,r,c);};
	void DrawPixel( UG_S16 x0, UG_S16 y0, UG_COLOR c ){UG_DrawPixel(x0,y0,c);};
	void DrawCircle( UG_S16 x0, UG_S16 y0, UG_S16 r, UG_COLOR c ){UG_DrawCircle(x0,y0,r,c);};
	void DrawArc( UG_S16 x0, UG_S16 y0, UG_S16 r, UG_U8 s, UG_COLOR c ){UG_DrawArc(x0,y0,r,s,c);};
  void DrawLine( UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c ){UG_DrawLine(x1,y1,x2,y2,c);};
	void DrawBMP( UG_S16 xp, UG_S16 yp, UG_BMP* bmp ){UG_DrawBMP(xp,yp,bmp);};
	// 字体
	void FontSelect(const UG_FONT* font){UG_FontSelect(font);};
	void FontSetHSpace( UG_U16 s ){UG_FontSetHSpace(s);};
	void FontSetVSpace( UG_U16 s ){UG_FontSetVSpace(s);};
	// 文本输出
	void PutString( UG_S16 x, UG_S16 y, char* str ){UG_PutString(x,y,str);};
	void PutChar( char chr, UG_S16 x, UG_S16 y, UG_COLOR fc, UG_COLOR bc ){UG_PutChar(chr,x,y,fc,bc);};
	void ConsolePutString( char* str ){UG_ConsolePutString(str);};
	void ConsoleSetArea( UG_S16 xs, UG_S16 ys, UG_S16 xe, UG_S16 ye ){UG_ConsoleSetArea(xs,ys,xe,ye);};
	void ConsoleSetForecolor( UG_COLOR c ){UG_ConsoleSetForecolor(c);};
	void ConsoleSetBackcolor( UG_COLOR c ){UG_ConsoleSetBackcolor(c);};
	
	// 窗口
	int8_t WindowCreate(UG_WINDOW* wnd, UG_OBJECT* objlst, UG_U8 objcnt, void (*cb)( UG_MESSAGE* ) ){
		if(UG_WindowCreate(wnd, objlst, objcnt, cb)==UG_RESULT_OK)
		{
				_wnd = wnd;
				return UG_RESULT_OK;
		}
		return UG_RESULT_FAIL;
	};
	void 	 WindowSelect(UG_WINDOW* wnd){_wnd = wnd;};
	int8_t WindowDelete(UG_WINDOW* wnd=_wnd){ return UG_WindowDelete(wnd);};
	int8_t WindowShow(UG_WINDOW* wnd=_wnd){return UG_WindowShow(wnd);};
	int8_t WindowHide(UG_WINDOW* wnd=_wnd){return UG_WindowHide(wnd);};
	int8_t WindowResize(UG_S16 xs,UG_S16 ys,UG_S16 xe,UG_S16 ye,UG_WINDOW* wnd=_wnd){return UG_WindowResize(wnd,xs,ys,xe,ye);};
	int8_t WindowAlert(UG_WINDOW* wnd=_wnd){ return UG_WindowAlert(wnd);};
	int8_t WindowSetForeColor(UG_COLOR fc,UG_WINDOW* wnd=_wnd){return UG_WindowSetForeColor(wnd, fc );};
	int8_t WindowSetBackColor(UG_COLOR bc,UG_WINDOW* wnd=_wnd){return UG_WindowSetBackColor(wnd,bc);};
	int8_t WindowSetTitleTextColor(UG_COLOR c,UG_WINDOW* wnd=_wnd){return UG_WindowSetTitleTextColor(wnd,c);};
	int8_t WindowSetTitleColor(UG_COLOR c,UG_WINDOW* wnd=_wnd){return UG_WindowSetTitleColor(wnd,c);};
	int8_t WindowSetTitleInactiveTextColor(UG_COLOR c,UG_WINDOW* wnd=_wnd){return UG_WindowSetTitleInactiveTextColor(wnd,c);};
	int8_t WindowSetTitleInactiveColor(UG_COLOR c,UG_WINDOW* wnd=_wnd){return UG_WindowSetTitleInactiveColor(wnd,c);};
	int8_t WindowSetTitleText(char* str,UG_WINDOW* wnd=_wnd){return UG_WindowSetTitleText(wnd,str);};
	int8_t WindowSetTitleTextFont(const UG_FONT* font,UG_WINDOW* wnd=_wnd){return UG_WindowSetTitleTextFont(wnd,font);};
	int8_t WindowSetTitleTextHSpace(UG_S8 hs,UG_WINDOW* wnd=_wnd){return UG_WindowSetTitleTextHSpace(wnd,hs);};
	int8_t WindowSetTitleTextVSpace(UG_S8 vs,UG_WINDOW* wnd=_wnd){return UG_WindowSetTitleTextVSpace(wnd,vs);};
	int8_t WindowSetTitleTextAlignment(UG_U8 align,UG_WINDOW* wnd=_wnd){return UG_WindowSetTitleTextAlignment(wnd,align);};
	int8_t WindowSetTitleHeight(UG_U8 height,UG_WINDOW* wnd=_wnd){ return UG_WindowSetTitleHeight(wnd, height);};
	int8_t WindowSetXStart(UG_S16 xs,UG_WINDOW* wnd=_wnd){ return UG_WindowSetXStart(wnd, xs);};
	int8_t WindowSetYStart(UG_S16 ys,UG_WINDOW* wnd=_wnd){ return UG_WindowSetYStart(wnd, ys);};
	int8_t WindowSetXEnd(UG_S16 xe,UG_WINDOW* wnd=_wnd){ return UG_WindowSetXEnd(wnd, xe);};
	int8_t WindowSetYEnd( UG_S16 ye,UG_WINDOW* wnd=_wnd){ return UG_WindowSetYEnd(wnd, ye);};
	int8_t WindowSetStyle(UG_U8 style,UG_WINDOW* wnd=_wnd){return UG_WindowSetStyle(wnd, style);};
	UG_COLOR WindowGetForeColor(UG_WINDOW* wnd=_wnd){return UG_WindowGetForeColor(wnd);};
	UG_COLOR WindowGetBackColor(UG_WINDOW* wnd=_wnd){return UG_WindowGetBackColor(wnd);};
	UG_COLOR WindowGetTitleTextColor(UG_WINDOW* wnd=_wnd){return UG_WindowGetTitleTextColor(wnd);};
	UG_COLOR WindowGetTitleColor(UG_WINDOW* wnd=_wnd){return UG_WindowGetTitleColor(wnd);};
	UG_COLOR WindowGetTitleInactiveTextColor(UG_WINDOW* wnd=_wnd){return UG_WindowGetTitleInactiveTextColor(wnd);};
	UG_COLOR WindowGetTitleInactiveColor(UG_WINDOW* wnd=_wnd){return UG_WindowGetTitleInactiveColor(wnd);};
	char* WindowGetTitleText(UG_WINDOW* wnd=_wnd){return UG_WindowGetTitleText(wnd);};
	UG_FONT* WindowGetTitleTextFont(UG_WINDOW* wnd=_wnd){return UG_WindowGetTitleTextFont(wnd);};
	UG_S8 WindowGetTitleTextHSpace(UG_WINDOW* wnd=_wnd){return UG_WindowGetTitleTextHSpace(wnd);};
	UG_S8 WindowGetTitleTextVSpace(UG_WINDOW* wnd=_wnd){return UG_WindowGetTitleTextVSpace(wnd);};
	UG_U8 WindowGetTitleTextAlignment(UG_WINDOW* wnd=_wnd){return UG_WindowGetTitleTextAlignment(wnd);};
	UG_U8 WindowGetTitleHeight(UG_WINDOW* wnd=_wnd){return UG_WindowGetTitleHeight(wnd);};
	UG_S16 WindowGetXStart(UG_WINDOW* wnd=_wnd){return UG_WindowGetXStart(wnd);};
	UG_S16 WindowGetYStart(UG_WINDOW* wnd=_wnd){return UG_WindowGetYStart(wnd);};
	UG_S16 WindowGetXEnd(UG_WINDOW* wnd=_wnd){return UG_WindowGetXEnd(wnd);};
	UG_S16 WindowGetYEnd(UG_WINDOW* wnd=_wnd){return UG_WindowGetYEnd(wnd);};
	UG_U8 WindowGetStyle(UG_WINDOW* wnd=_wnd){return UG_WindowGetStyle(wnd);};
	int8_t WindowGetArea(UG_AREA* a,UG_WINDOW* wnd=_wnd){return UG_WindowGetArea(wnd,a);};
	UG_S16 WindowGetInnerWidth(UG_WINDOW* wnd=_wnd){return UG_WindowGetInnerWidth(wnd);};
	UG_S16 WindowGetOuterWidth(UG_WINDOW* wnd=_wnd){return UG_WindowGetOuterWidth(wnd);};
	UG_S16 WindowGetInnerHeight(UG_WINDOW* wnd=_wnd){return UG_WindowGetInnerHeight(wnd);};
	UG_S16 WindowGetOuterHeight(UG_WINDOW* wnd=_wnd){return UG_WindowGetOuterHeight(wnd);};
	
	// 文本框
	int8_t TextboxCreate(UG_TEXTBOX* txb, UG_U8 id, UG_S16 xs, UG_S16 ys, UG_S16 xe, UG_S16 ye,UG_WINDOW* wnd = _wnd){return UG_TextboxCreate(wnd,txb,id,xs,ys,xe,ye );};
	int8_t TextboxDelete(UG_U8 id ,UG_WINDOW* wnd=_wnd){return UG_TextboxDelete(wnd,id );};
	int8_t TextboxShow(UG_U8 id, UG_WINDOW* wnd = _wnd){return UG_TextboxShow(wnd,id );};
	int8_t TextboxHide(UG_U8 id, UG_WINDOW* wnd = _wnd){return UG_TextboxHide(wnd,id );};
	int8_t TextboxSetForeColor(UG_U8 id, UG_COLOR fc, UG_WINDOW* wnd=_wnd){return UG_TextboxSetForeColor(wnd,id,fc);};
	int8_t TextboxSetBackColor(UG_U8 id, UG_COLOR bc, UG_WINDOW* wnd=_wnd){return UG_TextboxSetBackColor(wnd,id,bc);};
	int8_t TextboxSetText(UG_U8 id, char* str, UG_WINDOW* wnd=_wnd){return UG_TextboxSetText(wnd,id,str);};
	int8_t TextboxSetFont(UG_U8 id, const UG_FONT* font, UG_WINDOW* wnd=_wnd){return UG_TextboxSetFont(wnd,id,font);};
	int8_t TextboxSetHSpace(UG_U8 id, UG_S8 hs,UG_WINDOW* wnd=_wnd){return UG_TextboxSetHSpace(wnd,id,hs);};
	int8_t TextboxSetVSpace(UG_U8 id, UG_S8 vs,UG_WINDOW* wnd=_wnd){return UG_TextboxSetVSpace(wnd,id,vs);};
	int8_t TextboxSetAlignment(UG_U8 id, UG_U8 align, UG_WINDOW* wnd=_wnd){return UG_TextboxSetAlignment(wnd,id,align);};
	UG_COLOR TextboxGetForeColor(UG_U8 id, UG_WINDOW* wnd=_wnd){return UG_TextboxGetForeColor(wnd,id);};
	UG_COLOR TextboxGetBackColor(UG_U8 id, UG_WINDOW* wnd=_wnd){return UG_TextboxGetBackColor(wnd,id);};
	char* TextboxGetText(UG_U8 id, UG_WINDOW* wnd=_wnd){return UG_TextboxGetText(wnd,id);};
	UG_FONT* TextboxGetFont(UG_U8 id, UG_WINDOW* wnd=_wnd){return UG_TextboxGetFont(wnd,id);};
	UG_S8 TextboxGetHSpace(UG_U8 id, UG_WINDOW* wnd=_wnd){return UG_TextboxGetHSpace(wnd,id);};
	UG_S8 TextboxGetVSpace(UG_U8 id, UG_WINDOW* wnd=_wnd){return UG_TextboxGetVSpace(wnd,id);};
	UG_U8 UG_TextboxGetAlignment(UG_U8 id,UG_WINDOW* wnd=_wnd){return UG_TextboxGetVSpace(wnd,id);};
	
	// Image 
	int8_t ImageCreate(UG_IMAGE* img,UG_U8 id,UG_S16 xs,UG_S16 ys,UG_S16 xe,UG_S16 ye,UG_WINDOW* wnd=_wnd){return UG_ImageCreate(wnd,img,id,xs,ys,xe,ye);};
	int8_t ImageDelete(UG_U8 id,UG_WINDOW* wnd=_wnd){return UG_ImageDelete(wnd,id);};
	int8_t ImageShow(UG_U8 id,UG_WINDOW* wnd=_wnd){return UG_ImageShow(wnd,id);};
	int8_t ImageHide(UG_U8 id,UG_WINDOW* wnd=_wnd){return UG_ImageHide(wnd,id);};
	int8_t ImageSetBMP(UG_U8 id, const UG_BMP* bmp, UG_WINDOW* wnd=_wnd){return UG_ImageSetBMP(wnd,id,bmp);};	
	
	// 按钮
	int8_t ButtonCreate(UG_BUTTON* btn,UG_U8 id,UG_S16 xs,UG_S16 ys,UG_S16 xe,UG_S16 ye,UG_WINDOW* wnd=_wnd){
		_bID = id;	
		return UG_ButtonCreate(wnd,btn,id,xs,ys,xe,ye);
	};
	int8_t ButtonDelete(UG_U8 id=_bID,UG_WINDOW* wnd=_wnd){return UG_ButtonDelete(wnd,id);};
	int8_t ButtonShow(UG_U8 id=_bID,UG_WINDOW* wnd=_wnd){return UG_ButtonShow(wnd,id);};
	int8_t ButtonHide(UG_U8 id=_bID,UG_WINDOW* wnd=_wnd){return UG_ButtonHide(wnd,id);};
	int8_t ButtonSetForeColor(UG_COLOR fc,UG_U8 id=_bID,UG_WINDOW* wnd=_wnd){return UG_ButtonSetForeColor(wnd,id,fc);};
	int8_t ButtonSetBackColor(UG_COLOR bc,UG_U8 id=_bID,UG_WINDOW* wnd=_wnd){return UG_ButtonSetBackColor(wnd,id,bc);};
	int8_t ButtonSetAlternateForeColor(UG_COLOR afc,UG_U8 id=_bID,UG_WINDOW* wnd=_wnd){return UG_ButtonSetAlternateForeColor(wnd,id,afc);};
	int8_t ButtonSetAlternateBackColor(UG_COLOR abc,UG_U8 id=_bID,UG_WINDOW* wnd=_wnd){return UG_ButtonSetAlternateBackColor(wnd,id,abc);};
	int8_t ButtonSetText(char* str,UG_U8 id=_bID,UG_WINDOW* wnd=_wnd){return UG_ButtonSetText(wnd,id,str);};
	int8_t ButtonSetFont(const UG_FONT* font,UG_U8 id=_bID,UG_WINDOW* wnd=_wnd){return UG_ButtonSetFont(wnd,id,font);};
	int8_t ButtonSetStyle(UG_U8 style,UG_U8 id=_bID,UG_WINDOW* wnd=_wnd){return UG_ButtonSetStyle(wnd,id,style);};
	int8_t ButtonSetHSpace(UG_S8 hs,UG_U8 id=_bID,UG_WINDOW* wnd=_wnd){return UG_ButtonSetHSpace(wnd,id,hs);};
	int8_t ButtonSetVSpace(UG_S8 vs,UG_U8 id=_bID,UG_WINDOW* wnd=_wnd){return UG_ButtonSetVSpace(wnd,id,vs);};
	int8_t ButtonSetAlignment(UG_U8 align,UG_U8 id=_bID,UG_WINDOW* wnd=_wnd){return UG_ButtonSetAlignment(wnd,id,align);};
	UG_COLOR ButtonGetForeColor(UG_U8 id=_bID,UG_WINDOW* wnd=_wnd){return UG_ButtonGetForeColor(wnd,id);};
	UG_COLOR ButtonGetBackColor(UG_U8 id=_bID,UG_WINDOW* wnd=_wnd){return UG_ButtonGetBackColor(wnd,id);};
	UG_COLOR ButtonGetAlternateForeColor(UG_U8 id=_bID,UG_WINDOW* wnd=_wnd){return UG_ButtonGetAlternateForeColor(wnd,id);};
	UG_COLOR ButtonGetAlternateBackColor(UG_U8 id=_bID,UG_WINDOW* wnd=_wnd){return UG_ButtonGetAlternateBackColor(wnd,id);};
	char* ButtonGetText(UG_U8 id=_bID,UG_WINDOW* wnd=_wnd){return UG_ButtonGetText(wnd,id);};
	UG_FONT* ButtonGetFont(UG_U8 id=_bID,UG_WINDOW* wnd=_wnd){return UG_ButtonGetFont(wnd,id);};
	UG_U8 ButtonGetStyle(UG_U8 id=_bID,UG_WINDOW* wnd=_wnd){return UG_ButtonGetStyle(wnd,id);};
	UG_S8 ButtonGetHSpace(UG_U8 id=_bID,UG_WINDOW* wnd=_wnd){return UG_ButtonGetHSpace(wnd,id);};
	UG_S8 ButtonGetVSpace(UG_U8 id=_bID,UG_WINDOW* wnd=_wnd){return UG_ButtonGetVSpace(wnd,id);};
	UG_U8 ButtonGetAlignment(UG_U8 id=_bID,UG_WINDOW* wnd=_wnd){return UG_ButtonGetAlignment(wnd,id);};
	
	// check box
	int8_t CheckboxCreate(UG_CHECKBOX* cbox,UG_U8 id,UG_S16 xs,UG_S16 ys,UG_S16 xe,UG_S16 ye,UG_WINDOW* wnd=_wnd){
		_cID = id;
		return UG_CheckboxCreate(wnd,cbox,id,xs,ys,xe,ye);
	};
	int8_t CheckboxDelete(UG_U8 id=_cID, UG_WINDOW* wnd=_wnd){ return UG_CheckboxDelete(wnd,id);};
	int8_t CheckboxShow(UG_U8 id=_cID, UG_WINDOW* wnd=_wnd){ return UG_CheckboxShow(wnd,id);};
	int8_t CheckboxHide(UG_U8 id=_cID, UG_WINDOW* wnd=_wnd){ return UG_CheckboxHide(wnd,id);};
	int8_t CheckboxSetCheched(UG_U8 ch,UG_U8 id=_cID, UG_WINDOW* wnd=_wnd){ return UG_CheckboxSetCheched(wnd,id,ch);};
	int8_t CheckboxSetForeColor(UG_COLOR fc,UG_U8 id=_cID, UG_WINDOW* wnd=_wnd){ return UG_CheckboxSetForeColor(wnd,id,fc);};
	int8_t CheckboxSetBackColor(UG_COLOR bc,UG_U8 id=_cID, UG_WINDOW* wnd=_wnd){ return UG_CheckboxSetBackColor(wnd,id,bc);};
	int8_t CheckboxSetAlternateForeColor(UG_COLOR afc,UG_U8 id=_cID,UG_WINDOW* wnd=_wnd){ return UG_CheckboxSetAlternateForeColor(wnd,id,afc);};
	int8_t CheckboxSetAlternateBackColor(UG_COLOR bfc,UG_U8 id=_cID,UG_WINDOW* wnd=_wnd){ return UG_CheckboxSetAlternateBackColor(wnd,id,bfc);};
	int8_t CheckboxSetText(char* str,UG_U8 id=_cID,UG_WINDOW* wnd=_wnd){ return UG_CheckboxSetText(wnd,id,str);};
	int8_t CheckboxSetFont(const UG_FONT* font,UG_U8 id=_cID,UG_WINDOW* wnd=_wnd){ return UG_CheckboxSetFont(wnd,id,font);};
	int8_t CheckboxSetStyle(UG_U8 style,UG_U8 id=_cID,UG_WINDOW* wnd=_wnd){ return UG_CheckboxSetStyle(wnd,id,style);};
	int8_t CheckboxSetHSpace(UG_S8 hs,UG_U8 id=_cID,UG_WINDOW* wnd=_wnd){ return UG_CheckboxSetHSpace(wnd,id,hs);};
	int8_t CheckboxSetVSpace(UG_S8 vs,UG_U8 id=_cID,UG_WINDOW* wnd=_wnd){ return UG_CheckboxSetVSpace(wnd,id,vs);};
	int8_t CheckboxSetAlignment(UG_U8 align,UG_U8 id=_cID,UG_WINDOW* wnd=_wnd){ return UG_CheckboxSetAlignment(wnd,id,align);};
	UG_U8 CheckboxGetChecked(UG_U8 id=_cID,UG_WINDOW* wnd=_wnd){ return UG_CheckboxGetChecked(wnd,id);};
	UG_COLOR CheckboxGetForeColor(UG_U8 id=_cID,UG_WINDOW* wnd=_wnd){ return UG_CheckboxGetForeColor(wnd,id);};
	UG_COLOR CheckboxGetBackColor(UG_U8 id=_cID,UG_WINDOW* wnd=_wnd){ return UG_CheckboxGetBackColor(wnd,id);};
	UG_COLOR CheckboxGetAlternateForeColor(UG_U8 id=_cID,UG_WINDOW* wnd=_wnd){ return UG_CheckboxGetAlternateForeColor(wnd,id);};
	UG_COLOR CheckboxGetAlternateBackColor(UG_U8 id=_cID,UG_WINDOW* wnd=_wnd){ return UG_CheckboxGetAlternateBackColor(wnd,id);};
	char* CheckboxGetText(UG_U8 id=_cID,UG_WINDOW* wnd=_wnd){ return UG_CheckboxGetText(wnd,id);};
	UG_FONT* CheckboxGetFont(UG_U8 id=_cID,UG_WINDOW* wnd=_wnd){ return UG_CheckboxGetFont(wnd,id);};
	UG_U8 CheckboxGetStyle(UG_U8 id=_cID,UG_WINDOW* wnd=_wnd){ return UG_CheckboxGetStyle(wnd,id);};
	UG_S8 CheckboxGetHSpace(UG_U8 id=_cID,UG_WINDOW* wnd=_wnd){ return UG_CheckboxGetHSpace(wnd,id);};
	UG_S8 CheckboxGetVSpace(UG_U8 id=_cID,UG_WINDOW* wnd=_wnd){ return UG_CheckboxGetVSpace(wnd,id);};
	UG_U8 CheckboxGetAlignment(UG_U8 id=_cID,UG_WINDOW* wnd=_wnd){ return UG_CheckboxGetAlignment(wnd,id);};
	
	/* Miscellaneous functions */
	void WaitForUpdate(){UG_WaitForUpdate();};
	void Update(){UG_Update();};
//	void UG_DrawBMP( UG_S16 xp, UG_S16 yp, UG_BMP* bmp );
	void TouchUpdate( UG_S16 xp, UG_S16 yp, UG_U8 state ){UG_TouchUpdate(xp,yp,state);};
	UG_RESULT Button_CLICK( UG_WINDOW* wnd, UG_U8 id){return UG_Button_CLICK(wnd,id);};
private:
  UG_GUI _gui;	
};
#endif
