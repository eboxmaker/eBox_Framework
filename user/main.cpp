/**
  ******************************************************************************
  * @file   : *.cpp
  * @author : shentq
  * @version: V1.2
  * @date   : 2016/08/14

  * @brief   ebox application example .
  *
  * Copyright 2016 shentq. All Rights Reserved.         
  ******************************************************************************
 */
 
 
#include "ebox.h"
                                            
uint8_t *ptr;
uint8_t *ptx[100];
void setup()
{
    ebox_init();
    uart1.begin(115200);
    
    uart1.printf("\r\n");
    uart1.printf("----------------mem test--------------------\r\n");
    uart1.printf("mem :0x%x---0x%x(%dK%dbytes)\r\n",ebox_get_sram_start_addr(),ebox_get_sram_end_addr(),(ebox_get_free())/1024,(ebox_get_free())%1024);
    int     i = 0;

    delay_ms(100);
        uart1.printf("ptr ID:\tADDRE\t\t|free \r\n");
    for( i = 0; i < 100; i++)
    {
        ptx[i] = (uint8_t *)ebox_malloc(100);
        if(ptx[i] == NULL)
            uart1.printf("game over\r\n");
        else
            for(int j = 0; j < 16; j++)
            {
                ptx[i][j] = 0xFF;
            }
            
        uart1.printf("ptr %03d:\t0X%X\t|%05d\r\n",i,ptx[i],ebox_get_free());
        
    }

    for( i = 0; i < 100; i++)
    {
        for(int j = 0; j < 16; j++)
        {
            if(ptx[i][j] != 0xFF)
                uart1.printf("ptr err%03d:0X%X\r\n",i,ptx[i]);

        }
    }
    
    for( i = 0; i < 100;  i +=   2)
    {
       
        ebox_free(ptx[i]);
        uart1.printf("ptr %03d:\t0X%X\t|\t%d\t|\t%d\t|%05d\r\n",i,ptx[i],ptx[i],ptx[i] - ptx[i -1],ebox_get_free());

    }
    ebox_free_block_print();
    
    
    for( i = 1; i < 80;  i +=   2)
    {
       
        ebox_free(ptx[i]);
    }
    ebox_free_block_print();

}
int main(void)
{
    setup();
    while(1)
    {
        //uart1.print(".");
        delay_ms(1000);
    }

}


