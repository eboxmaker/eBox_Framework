/**
 ******************************************************************************
 * @file    main.cpp
 * @author  cat_li
 * @version V1.0
 * @date    2017/07/13
 * @brief   ebox exti example, 基于stm32nucleo(072)平台验证
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


/* Includes ------------------------------------------------------------------*/

#include "ebox.h"
#include "bsp_ebox.h"
#include "./base64/base64.h"

/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"Base64 example"
#define EXAMPLE_DATE	"2018-12-10"


void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);

  
    String str1 = "Hello world";
    String str2;
    String str3;
    base64_encode(str2,str1);
    uart1.println(str2);
    base64_decode(str3,str2);
    uart1.println(str3);

    // encoding
    char input[] = "Hello world";
    int inputLen = sizeof(input);

    int encodedLen = base64_enc_len(inputLen);
    char encoded[100];

    Serial.print(input); Serial.print(" = ");

    // note input is consumed in this step: it will be empty afterwards
    base64_encode(encoded, input, inputLen); 

    Serial.println(encoded);



    // decoding
    char input2[] = "SGVsbG8gd29ybGQA";
    int input2Len = sizeof(input2);

    int decodedLen = base64_dec_len(input2, input2Len);
    char decoded[100];

    base64_decode(decoded, input2, input2Len);

    Serial.print(input2); Serial.print(" = "); Serial.println(decoded);
}


int main(void)
{
    setup();
    while(1)
    {
    }
}




