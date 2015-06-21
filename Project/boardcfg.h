

#ifndef __BOARDFCG_H
#define __BOARDFCG_H

#ifdef cplusplus
extern "C"{
#endif



/////////////IO Config//////////////
#define BTNM_PIN 11 
#define EC11A_PIN 25 
#define EC11B_PIN 24 

#define DS3231_SDA_PIN 5
#define DS3231_SCL_PIN 4

#define _595_DATA_PIN 47
#define _595_SCK_PIN 	45
#define _595_RCK_PIN 	46

#define LCD_LED_PIN 	28
#define LCD_EN_PIN 		29
#define LCD_RW_PIN 		30
#define LCD_RS_PIN 		31
#define LCD_D0_PIN 		16
#define LCD_D1_PIN 		17
#define LCD_D2_PIN 		18
#define LCD_D3_PIN 		19
#define LCD_D4_PIN 		20
#define LCD_D5_PIN 		21
#define LCD_D6_PIN 		22
#define LCD_D7_PIN 		23

#define _1PPS_PIN	7

#define _485_RTCTR_PIN	
#define _485_RX_PIN	
#define _485_TX_PIN	

#ifdef cplusplus
}
#endif

#endif
