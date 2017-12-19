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
#include "EventManager.h"
#include "enc28j60.h"
#include "lwip/etharp.h" // etharp_tmr�������ڵ�ͷ�ļ�
#include "lwip/init.h" // lwip_init�������ڵ�ͷ�ļ�
#include "lwip/priv/tcp_priv.h" // tcp_tmr�������ڵ�ͷ�ļ�
#include "netif/ethernet.h" // ethernet_input��������ͷ�ļ�

#include "eth_port.h"

#include "httpd.h"
#include <stdio.h>
#include "ringbuf.h"
#include "stdio.h"

#include "fifo.h"
struct my_fifo *x_ptr;


extern Enc28j60 eth;

// ����������λ��ethernetif.c��, ��û��ͷ�ļ�����

// ����httptest.c�еĺ���
void init_http(void);


    struct ip4_addr ipaddr, netmask, gw;
    struct netif enc28j60;
    uint8_t cnt = 0;
    
RINGBUF ring;
uint8_t xbuf[1000];

void setup()
{
    ebox_init();
    uart1.begin(115200);
    uart1.printf("being\r\n");


    x_ptr = my_fifo_alloc(100);
    
    
   // printf("123");
    // ���ö�ʱ��
    RCC->APB1ENR = RCC_APB1ENR_TIM6EN;
    TIM6->ARR = 2499; // ��2500����, 2500*0.1ms=250ms
    TIM6->PSC = 7199; // 72MHz/7200=10kHz -> 0.1ms
    TIM6->CR1 = TIM_CR1_URS; // ��ֹUG=1ʱUIF��λ
    TIM6->EGR = TIM_EGR_UG; // Ӧ����������
    TIM6->CR1 |= TIM_CR1_CEN; // ����ʱ��
  
//    lwip_init();
//    IP4_ADDR(&ipaddr, 192, 168, 1, 18); // IP��ַ
//    IP4_ADDR(&netmask, 255, 255, 255, 0); // ��������
//    IP4_ADDR(&gw, 192, 168, 1, 1); // ����
//            uart1.printf("x");

//    netif_add(&enc28j60, &ipaddr, &netmask, &gw, NULL, ethernetif_init, ethernet_input);
//    netif_set_default(&enc28j60); // ��ΪĬ������
//    netif_set_up(&enc28j60);
//    httpd_init(); // ��ʼ��HTTP����
    
//    uart1.printf("%c",eth.read(MAADR0));
//    uart1.printf("%c",eth.read(MAADR1));
//    uart1.printf("%c",eth.read(MAADR2));
//    uart1.printf("%c",eth.read(MAADR3));
//    uart1.printf("%c",eth.read(MAADR4));
//    uart1.printf("%c",eth.read(MAADR5));

    
}

uint8_t buf[100];
uint8_t len;
int main(void)
{
    setup();

    while(1)
    {

        
        
        len = my_fifo_get(x_ptr,buf,100);
        if(len != 0)
        uart1.write(buf,len);
                printf("123");

        delay_ms(1000);
//    if (eth.get_packet_num() != 0)
//        {
////            GPIOA->ODR ^= GPIO_ODR_ODR8; // PA8�ϵ�LED����˸����ϵͳ��������
//            ethernetif_input(&enc28j60);
//            
//            uart1.printf("123");
//        }
//        
//        // ����ʱ�����
//        if (TIM6->SR & TIM_SR_UIF)
//        {
//            uart1.printf("x");
//            // 250ms
//            TIM6->SR &= ~TIM_SR_UIF; // ��������־
//            cnt++;
//            if (cnt >= 20)
//            {
//                // 250ms * 20 = 5s
//                GPIOA->ODR ^= GPIO_ODR_ODR8; // LED��ÿ5���Ӹı�һ��״̬
//                cnt = 0;
//                etharp_tmr(); // ARP��ʱ����
//            }
//            tcp_tmr(); // TCP��ʱ����
//        }
    }
}

extern "C"
{

}
