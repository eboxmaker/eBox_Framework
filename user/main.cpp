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


#include "bsp.h"
#include "ebox.h"
#include <sfud.h>
 
#define SFUD_DEMO_TEST_BUFFER_SIZE                     1024

static void sfud_demo(uint32_t addr, size_t size, uint8_t *data);

static uint8_t sfud_demo_test_buf[SFUD_DEMO_TEST_BUFFER_SIZE];


void setup()
{
    ebox_init();
    led1.begin();
    uart1.begin(115200);
    uart1.printf("test\r\n");
    if (sfud_init() == SFUD_SUCCESS) {
     sfud_demo(0, sizeof(sfud_demo_test_buf), sfud_demo_test_buf);
       delay_ms(1000);
    }

}

int main(void)
{
    setup();
    while(1)
    {
        led1.toggle();
        delay_ms(1000);
    }
}

/**
 * SFUD demo for the first flash device test.
 *
 * @param addr flash start address
 * @param size test flash size
 * @param size test flash data buffer
 */
static void sfud_demo(uint32_t addr, size_t size, uint8_t *data) {
    sfud_err result = SFUD_SUCCESS;
    const sfud_flash *flash = sfud_get_device_table() + 0;
    size_t i;
    /* prepare write data */
    for (i = 0; i < size; i++) {
        data[i] = i;
    }
    /* erase test */
    result = sfud_erase(flash, addr, size);
    if (result == SFUD_SUCCESS) {
        uart1.printf("Erase the %s flash data finish. Start from 0x%08X, size is %ld.\r\n", flash->name, addr,
                size);
    } else {
        uart1.printf("Erase the %s flash data failed.\r\n", flash->name);
        return;
    }
    /* write test */
    result = sfud_write(flash, addr, size, data);
    if (result == SFUD_SUCCESS) {
        uart1.printf("Write the %s flash data finish. Start from 0x%08X, size is %ld.\r\n", flash->name, addr,
                size);
    } else {
        uart1.printf("Write the %s flash data failed.\r\n", flash->name);
        return;
    }
    /* read test */
    result = sfud_read(flash, addr, size, data);
    if (result == SFUD_SUCCESS) {
        uart1.printf("Read the %s flash data success. Start from 0x%08X, size is %ld. The data is:\r\n", flash->name, addr,
                size);
        uart1.printf("Offset (h) 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\r\n");
        for (i = 0; i < size; i++) {
            if (i % 16 == 0) {
                uart1.printf("[%08X] ", addr + i);
            }
            uart1.printf("%02X ", data[i]);
            if (((i + 1) % 16 == 0) || i == size - 1) {
                uart1.printf("\r\n");
            }
        }
        uart1.printf("\r\n");
    } else {
        uart1.printf("Read the %s flash data failed.\r\n", flash->name);
    }
    /* data check */
    for (i = 0; i < size; i++) {
        if (data[i] != i % 256) {
            uart1.printf("Read and check write data has an error. Write the %s flash data failed.\r\n", flash->name);
			break;
        }
    }
    if (i == size) {
        uart1.printf("The %s flash test is success.\r\n", flash->name);
    }
}



