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
    
    
	uart1.printf("\r\n ����һ�� NRF24L01 ���ߴ���ʵ�� \r\n");
   	uart1.printf("\r\n �������ߴ��� ������ �ķ�����Ϣ\r\n");
	uart1.printf("\r\n   ���ڼ��NRF��MCU�Ƿ��������ӡ�����\r\n");

	 /*���NRFģ����MCU������*/
    status = nrf.self_check();

	/*�ж�����״̬*/  
   if(status == SUCCESS)	   
   		 uart1.printf("\r\n      NRF��MCU���ӳɹ���\r\n");  
   else	  
   	     uart1.printf("\r\n  NRF��MCU����ʧ�ܣ������¼����ߡ�\r\n");

}
u8 txbuf[4]={0,1,2,3};	 //���ͻ���
u8 rxbuf[4];			 //���ջ���

int main(void)
{
	setup();
	while(1)
	{
//        uart1.printf("\r\n ������ ������Ӧ����ģʽ\r\n");
//        nrf.set_tx_mode();

//        status = nrf.send(txbuf,32);
//        switch(status)
//        {
//            case MAX_TX:
// 			 	 	uart1.printf("\r\n ������ û���յ�Ӧ���źţ����ʹ��������޶�ֵ������ʧ�ܡ� \r\n");
//				 break;
//            
//			  case ERROR:
//			  	 	uart1.printf("\r\n δ֪ԭ���·���ʧ�ܡ� \r\n");
//				 break;
//	
//			  case TX_OK:
//			  		uart1.printf("\r\n ������ ���յ� �ӻ��� ��Ӧ���źţ����ͳɹ��� \r\n");	 		
//				 break;  								       
//        }
        uart1.printf("\r\n ������ �������ģʽ�� \r\n");	
		nrf.set_rx_mode();
        
		status = nrf.recv(rxbuf);

			/*�жϽ���״̬*/
			switch(status)
			{
			 case RX_OK:
                uart1.printf("\r\n ������ ���յ� �ӻ��� ���͵�����Ϊ��");
			 	for(int i=0;i<4;i++)
				{					
					uart1.printf(" %d ",rxbuf[i]);
               	}
				  break;
	
			 case ERROR:
				  	uart1.printf("\r\n ������ ���ճ���   \r\n");
				  break;  		
			}
	}
}
//����
//int main(void)
//{
//	setup();
//	while(1)
//	{
//        uart1.printf("\r\n ������ ������Ӧ����ģʽ\r\n");
//        nrf.set_tx_mode();

//        do{
//        status = nrf.send(txbuf,4);
//        }while(status == MAX_TX);

//        delay_ms(1000);

//	}
//}