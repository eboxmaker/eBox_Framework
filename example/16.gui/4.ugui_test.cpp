#include "ebox.h"
#include "bsp_ebox.h"
extern "C" {
#include "ugui.h"
}



/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"Virtual LCD example"
#define EXAMPLE_DATE	"2018-08-14"

#define MAX_OBJECTS        10
UG_GUI gui;
UG_WINDOW window_1;
UG_OBJECT obj_buff_wnd_1[MAX_OBJECTS];

/* Hardware accelerator for UG_DrawLine (Platform: STM32F4x9) */
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

/* Callback function for the main menu */
void window_1_callback( UG_MESSAGE* msg ){
}

void setup()
{
	ebox_init();
    UART.begin(115200);
	lcd.begin();
	UG_Init(&gui,&_HW_DrawPixel,128,160);
	   /* Register hardware acceleration */
  UG_DriverRegister( DRIVER_DRAW_LINE, (void*)_HW_DrawLine );
   UG_DriverRegister( DRIVER_FILL_FRAME, (void*)_HW_FillFrame );
   UG_DriverEnable( DRIVER_DRAW_LINE );
   UG_DriverEnable( DRIVER_FILL_FRAME );
	 UG_FillScreen( C_BLACK );
	
	 UG_DrawCircle(63,80,40,C_LIGHT_GRAY);
	 UG_FontSelect(&FONT_5X12 );
	 UG_PutString(0,30,(char*)"hello word");
	
//	   /* Create Window 1 */
   UG_WindowCreate( &window_1, obj_buff_wnd_1, MAX_OBJECTS, window_1_callback );
   UG_WindowSetTitleText( &window_1,(char*)"UI @ STM32F429" );
   UG_WindowSetTitleTextFont( &window_1, &FONT_5X12 );
	 UG_WindowResize( &window_1, 2, 9, 59, 70 );
	   /* Show Window 1 */
   UG_WindowShow( &window_1 );
	 UG_WaitForUpdate();
}
int main(void)
{
    int count =32;
	setup();
	while (1)
	{
    if(count == 0)count = 32;
    delay_ms(100);
        
	}
}
