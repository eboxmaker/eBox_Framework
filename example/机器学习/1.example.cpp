/**
 ******************************************************************************
 * @file    main.cpp
 * @author  shentq
 * @version V1.2
 * @date    2016/08/14
 * @brief   ebox application example .
*					 2018-8-5	通过引入bsp，定义硬件端口，方便例程可以在不同平台上运行
 ******************************************************************************
 * @attention
 *
 * No part of this software may be used for any commercial activities by any form
 * or means, without the prior written consent of shentq. This specification is
 * preliminary and is subject to change at any time without notice. shentq assumes
 * no responsibility for any errors contained herein.
 * <h2><center>&copy; Copyright 2015 shentq. All Rights Reserved.</center></h2>
 ******************************************************************************
 */

#include "ebox.h"
#include "bsp_ebox.h"
#include "sigmoid.h"
#include "data.h"
/**
	*	1	通过串口打印消息
	*/
/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"hello world example"
#define EXAMPLE_DATE	"2018-08-06"

void setup()
{
    ebox_init();
    LED1.mode(OUTPUT_PP);
    UART.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);
}

float a[4][5] = {
{1,3,-2,0,4},
{-2,-1,5,-7,2},
{0,8,4,1,-5},
{3,-3,2,-4,1},
};
float b[5][3] = {
{4,5,-1},
{2,-2,6},
{7,8,1},
{0,3,-5},
{9,8,-6},
};
float c[4][3];


//float a[32][32];
//float b[32][32];
//float c[32][32];

void trmul(float *a,float *b, int m, int n, int k, float *c);
uint32_t mark1,mark2;

float v[20],v1[20],v2[20],v3[20];
float y[25],y1[20],y2[20],y3[20];

void test(int index)
{
    mark1 = millis();
    switch(index)
    {
        case 1:
            //V1 = W1*X1
            trmul((float *)w1,(float *)x1,20,25,1,(float *)v1);
            break;
        case 2:
            //V1 = W1*X1
            trmul((float *)w1,(float *)x2,20,25,1,(float *)v1);
            break;
        case 3:
            //V1 = W1*X1
            trmul((float *)w1,(float *)x3,20,25,1,(float *)v1);
            break;
        case 4:
            //V1 = W1*X1
            trmul((float *)w1,(float *)x4,20,25,1,(float *)v1);
            break;
        case 5:
            //V1 = W1*X1
            trmul((float *)w1,(float *)x5,20,25,1,(float *)v1);
            break;

    }
    
    //Y1 = sigmoid(v1)
    for(int i = 0; i < 20; i++){ 
        y1[i] = sigmoid(v1[i]);
    }
    
    
    //V2 = W2*Y1
    trmul((float *)w2,(float *)y1,20,20,1,(float *)v2);
    //Y2 = sigmoid(v2)
    for(int i = 0; i < 20; i++){ 
        y2[i] = sigmoid(v2[i]);
    }
    
    //V3 = W3*Y2
    trmul((float *)w3,(float *)y2,20,20,1,(float *)v3);
    //Y2 = sigmoid(v2)
    for(int i = 0; i < 20; i++){ 
        y3[i] = sigmoid(v3[i]);
    }
    
    //V = W4*Y3
    trmul((float *)w4,(float *)y3,5,20,1,(float *)v);

    
    //Y1 = sigmoid(v1)
    Softmax(v,5,y);
    
    mark2 = millis();
    
    for(int i = 0; i < 5; i++){ 
        uart1.printf("%f\t",y[i]);
    }
    uart1.printf("使用时间:%dms\n",mark2 - mark1);
    
}
int main(void)
{
    setup();

    for(int i = 1; i < 6; i++)
    {
        test(i);
    }
    
    
//    uart1.println("V1");
//    for(int i = 0; i < 20; i++){
//            uart1.printf("%2.3f\t",v1[i]);
//    }
//    uart1.println();
//    
//    uart1.println("y1");
//    for(int i = 0; i < 20; i++){
//        uart1.printf("%2.1f\t",y1[i]);
//    }
//    uart1.println();
    
    
//    for(int i = 0; i < 32; i++){
//        for(int j = 0; j < 32; j++){
//            a[i][j] = 1;
//        }
//    }
//    for(int i = 0; i < 32; i++){
//        for(int j = 0; j < 32; j++){
//            b[i][j] = 1;
//        }
//    }
    
    uart1.printf("开始\n");
    uart1.flush();
    mark1 = millis();
    trmul((float *)a,(float *)b,4,5,3,(float *)c);
    mark2 = millis();
    for(int i = 0; i <= 3; i++){
        for(int j = 0; j <= 2; j++){
            uart1.printf("%2.1f\t",c[i][j]);
        }
        uart1.println();
    }
    uart1.println();

//    uart1.printf("time:%d\n",mark2 - mark1);
//    uart1.printf("sigmoid test\n");
//    
//    double x = 0,y;
//    y = sigmoid(x);
//    mark1 = millis();
//    uart1.printf("sigmoid(1) = %f\n",y);
//    mark2 = millis();
//    uart1.printf("time:%d\n",mark2 - mark1);

//    
//    
//    uart1.printf("softmax test\n");
//    float s_test[5];
//    float s_resault[5];
//    for(int i = 0; i < 5; i++)
//        s_test[i] = 1;
//    Softmax(s_test,5,s_resault);
//    
//    for(int i = 0; i < 5; i++)
//        uart1.printf("%f",s_resault[i]);
//    uart1.printf("\n");

    
    while(1)
    {

    }
}


void trmul(float *a,float *b, int m, int n, int k, float *c)
{
    int i,j,l,u;
    for(i = 0; i  <= m - 1; i++ ){
        for(j = 0; j  <= k - 1; j++ ){
            u = i*k+j;
            c[u] = 0.0;
            for(l = 0; l  <= n - 1; l++ ){
                c[u] = c[u] + a[i*n + l] * b[l*k + j];
//                uart1.printf("c[%d]:%f\t",u,c[u]);
            }
        }
    }
    return;
}

