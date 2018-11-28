#include "ebox.h"
#include "bsp_ebox.h"
#include "U_GUI.h"

/**
  *	1	��������ʾ��uGUI�Ļ�������,���������������Σ�Բ��Բ����Բ�Ǿ��Σ�����ֱ�ߡ��������壬����ַ���
	*	2	����lcd_1.8.cpp
	*	3	������ѭ����ʾ����Ļ��
	*/

/* ���������������̷������� */
#define EXAMPLE_NAME	"uGUI example"
#define EXAMPLE_DATE	"2018-11-24"

// ��Ҫ���㣬���ߣ��������������������������gui
UG_RESULT _HW_DrawLine( UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c )
{
	 lcd.draw_line(x1,y1,x2,y2,c);
   return UG_RESULT_OK;
}

void _HW_DrawPixel( UG_S16 x, UG_S16 y, UG_COLOR c )
{
	lcd.draw_pixel(x,y,c);
}


/* Hardware accelerator for UG_FillFrame (Platform: STM32F4x9) */
UG_RESULT _HW_FillFrame( UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c )
{
	 lcd.fill_rect(x1,y1,x2,y2,c);
   return UG_RESULT_OK;
}

U_GUI eGui;

/* Some defines */
#define MAX_OBJECTS        10

/* Window 1 */
UG_WINDOW window_1;
UG_OBJECT obj_buff_wnd_1[MAX_OBJECTS];
UG_BUTTON button1_1;
UG_BUTTON button1_2;
UG_BUTTON button1_3;
UG_BUTTON button1_4;
UG_BUTTON button1_5;
UG_BUTTON button1_6;

UG_TEXTBOX textbox3_1;

/* Callback function for the main menu */
void window_1_callback( UG_MESSAGE* msg )
{
   if ( msg->type == MSG_TYPE_OBJECT )
   {
      if ( msg->id == OBJ_TYPE_BUTTON )
      {
         switch( msg->sub_id )
         {
            case BTN_ID_0: /* Toggle green LED */
            {
//               TOGGLE_GREEN_LED;
               break;
            }
            case BTN_ID_1: /* Toggle red LED */
            {
//               TOGGLE_RED_LED;
               break;
            }
            case BTN_ID_2: /* Show �GUI info */
            {
//               UG_WindowShow( &window_2 );
               break;
            }
            case BTN_ID_3: /* Toggle hardware acceleration */
            {
//               if ( !hw_acc )
//               {
//                  UG_ButtonSetForeColor( &window_1, BTN_ID_3, C_RED );
//                  UG_ButtonSetText( &window_1, BTN_ID_3, "HW_ACC\nOFF" );
//                  UG_DriverEnable( DRIVER_DRAW_LINE );
//                  UG_DriverEnable( DRIVER_FILL_FRAME );
//               }
//               else
//               {
//                  UG_ButtonSetForeColor( &window_1, BTN_ID_3, C_BLUE );
//                  UG_ButtonSetText( &window_1, BTN_ID_3, "HW_ACC\nON" );
//                  UG_DriverDisable( DRIVER_DRAW_LINE );
//                  UG_DriverDisable( DRIVER_FILL_FRAME );
//               }
//               hw_acc = !hw_acc;
               break;
            }
            case BTN_ID_4: /* Start benchmark */
            {
//               next_state = STATE_BENCHMARK_RUN;
               break;
            }
            case BTN_ID_5: /* Resize window */
            {
               static UG_U32 tog;

               if ( !tog )
               {
                  UG_WindowResize( &window_1, 0, 40, 239, 319-40 );
               }
               else
               {
                  UG_WindowResize( &window_1, 0, 0, 239, 319 );
               }
               tog = ! tog;
               break;
            }
         }
      }
   }
}



void setup()
{
	ebox_init();
	lcd.begin();
	eGui.Begin(&_HW_DrawPixel,(void*)_HW_DrawLine,(void*)_HW_FillFrame,128,160);
	eGui.FillScreen(C_BLACK);
	eGui.DrawRoundFrame(1,1,127,159,4,C_WHITE);
	
   /* -------------------------------------------------------------------------------- */
   /* Create Window 3 (Benchmark Result)                                               */
   /* -------------------------------------------------------------------------------- */
	 eGui.WindowCreate(&window_1, obj_buff_wnd_1, MAX_OBJECTS, window_1_callback);
	 eGui.WindowSetTitleHeight(0);
//   UG_WindowSetTitleText( &window_1, "Benchmark Result" );
//   UG_WindowSetTitleTextFont( &window_1, &FONT_8X12 );

   /* Create Textbox 1 */
   UG_TextboxCreate( &window_1, &textbox3_1, TXB_ID_0, 5, 10, UG_WindowGetInnerWidth( &window_1 )-5, 60 );
   UG_TextboxSetFont( &window_1, TXB_ID_0, &FONT_8X12 );
   UG_TextboxSetText( &window_1, TXB_ID_0, "Result:\n99999 frm/sec" );
   UG_TextboxSetAlignment( &window_1, TXB_ID_0, ALIGN_TOP_CENTER );

   /* Create Button 1 */
   UG_ButtonCreate( &window_1, &button1_1, BTN_ID_0, 40, 65, UG_WindowGetInnerWidth( &window_1 )-40, 100 );
   UG_ButtonSetFont( &window_1, BTN_ID_0, &FONT_8X12 );
   UG_ButtonSetText( &window_1, BTN_ID_0, "OK!" );

	 eGui.WindowShow();
	 attachSystickCallBack(UG_Update,500);	// ÿ500ms����һ�ν���
	 UG_WaitForUpdate();
}
int main(void)
{
	setup();
	while (1)
	{
			delay_us(500);
	}
}
