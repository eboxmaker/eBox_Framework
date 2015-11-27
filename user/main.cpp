/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/


#include "string.h"
#include "ebox.h"

#include "nrf24l01.h"

NRF24L01 nrf(&PA3,&PA2,&PA1,&spi1);


	
uint8_t buf[32];// = "adfafdadfaf";
int status;
void setup()
{
	ebox_init();
	uart1.begin(9600);
    uart1.printf("uart is ok !\r\n");
    nrf.begin(2);
    
    
	uart1.printf("\r\n 这是一个 NRF24L01 无线传输实验 \r\n");
   	uart1.printf("\r\n 这是无线传输 主机端 的反馈信息\r\n");
	uart1.printf("\r\n   正在检测NRF与MCU是否正常连接。。。\r\n");

	 /*检测NRF模块与MCU的连接*/
    status = nrf.self_check();

	/*判断连接状态*/  
   if(status == SUCCESS)	   
   		 uart1.printf("\r\n      NRF与MCU连接成功！\r\n");  
   else	  
   	     uart1.printf("\r\n  NRF与MCU连接失败，请重新检查接线。\r\n");

}
u8 txbuf[4]={0,1,2,3};	 //发送缓冲
u8 rxbuf[4];			 //接收缓冲

int main(void)
{
	setup();
	while(1)
	{
//        uart1.printf("\r\n 主机端 进入自应答发送模式\r\n");
//        nrf.set_tx_mode();

//        status = nrf.send(txbuf,32);
//        switch(status)
//        {
//            case MAX_TX:
// 			 	 	uart1.printf("\r\n 主机端 没接收到应答信号，发送次数超过限定值，发送失败。 \r\n");
//				 break;
//            
//			  case ERROR:
//			  	 	uart1.printf("\r\n 未知原因导致发送失败。 \r\n");
//				 break;
//	
//			  case TX_OK:
//			  		uart1.printf("\r\n 主机端 接收到 从机端 的应答信号，发送成功！ \r\n");	 		
//				 break;  								       
//        }
        uart1.printf("\r\n 主机端 进入接收模式。 \r\n");	
		nrf.set_rx_mode();
        
		status = nrf.recv(rxbuf);

			/*判断接收状态*/
			switch(status)
			{
			 case RX_OK:
                uart1.printf("\r\n 主机端 接收到 从机端 发送的数据为：");
			 	for(int i=0;i<4;i++)
				{					
					uart1.printf(" %d ",rxbuf[i]);
               	}
				  break;
	
			 case ERROR:
				  	uart1.printf("\r\n 主机端 接收出错。   \r\n");
				  break;  		
			}
	}
}
//发送
//int main(void)
//{
//	setup();
//	while(1)
//	{
//        uart1.printf("\r\n 主机端 进入自应答发送模式\r\n");
//        nrf.set_tx_mode();

//        do{
//        status = nrf.send(txbuf,4);
//        }while(status == MAX_TX);

//        delay_ms(1000);

//	}
//}