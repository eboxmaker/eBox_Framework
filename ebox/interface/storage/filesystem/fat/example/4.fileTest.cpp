/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox
#include "ebox.h"
#include "bsp_ebox.h"
#include "mmc_sd.h"
#include "w25xxx.h"
#include "interface/storage/filesystem/FileSystem.h"
#include "interface/storage/filesystem/FAT/FATFileSystem.h"
#include "interface/storage/filesystem/file.h"
#include "rtcmillis.h"
#include "ebox_mem.h"

FATFileSystem fs("sd");

W25xxx flash(&PA15, &spi1);
SD sd(&PB12, &spi2);
RtcMillis LocalTime;

void file_create()
{
    int ret;
    UART.printf("file_create...\n");
    for(int i = 0; i < 10; i++)
    {
        String path("/ebox_");
        path += i;
        path += ".txt";
        File file;
        ret = file.open(&fs,path.c_str(),O_CREAT);
        if(ret)
        {
            UART.printf("open[%s]:%d\n",path.c_str(), ret);
        }
        else
        {
            UART.printf("open:[%s]...ok\n", path.c_str());
        }
        file.close();
        UART.flush();
    }
    
}
void file_write()
{
    int ret;
    int bw;
    UART.printf("file_write...\n");
    String str = "ebox file opt test.";
    for(int i = 0; i < 10; i++)
    {
        String path("/ebox_");
        path += i;
        path += ".txt";
        File file;
        ret = file.open(&fs,path.c_str(),O_WRONLY);
        if(ret)
        {
            UART.printf("open[%s]:%d\n",path.c_str(), ret);
        }
        else
        {
            UART.printf("open:[%s]...ok\n", path.c_str());
        }
        
        bw = file.write(str.c_str(),str.length());
        if(bw == 0){
            UART.printf("file write err : %d\r\n", bw);
        }
        else 
        {
            UART.printf("writed size : %d\r\n", bw);
        }
        UART.flush();
    }
}
void file_write_append()
{
    int ret;
    int bw;
    UART.printf("file_write...\n");
    String str = "a!";

    String path("/ebox_9.txt");

    File file;
    ret = file.open(&fs,path.c_str(),O_WRONLY|O_APPEND);
    if(ret)
    {
        UART.printf("open[%s]:%d\n",path.c_str(), ret);
    }
    else
    {
        UART.printf("open:[%s]...ok\n", path.c_str());
    }
    
    bw = file.write(str.c_str(),str.length());
    if(bw == 0){
        UART.printf("file write err : %d\r\n", bw);
    }
    else 
    {
        UART.printf("writed size : %d\r\n", bw);
    }
    UART.flush();
    
}

void file_delete()
{
    int ret=0;
    UART.printf("file_delete...\n");
    for(int i = 0; i < 5; i++)
    {
        String path("/ebox_");
        path += i;
        path += ".txt";
        ret = fs.remove(path.c_str());
        if(ret)
        {
            UART.printf("remove[%s]:%d\n",path.c_str(), ret);
        }
        else
        {
            UART.printf("remove:[%s]...ok\n", path.c_str());
        }
        UART.flush();
    }  
}
void file_read()
{
    int ret;
    int br;
    UART.printf("file_write...\n");
    char buf[128];
    for(int i = 0; i < 10; i++)
    {
        memset(buf,0,128);
        String path("/ebox_");
        path += i;
        path += ".txt";
        File file;
        ret = file.open(&fs,path.c_str(),O_RDONLY);
        if(ret)
        {
            UART.printf("open[%s]:%d\n",path.c_str(), ret);
        }
        else
        {
            UART.printf("open:[%s]...ok\n", path.c_str());
        }
        br = file.read(buf,128);
        UART.printf("file read :[%s] size:%d\r\n",buf, br);
        file.close();
        UART.flush();
    }
}


void fileOpt()
{
    int ret;
    file_create();
    file_write();
    file_write_append();
    file_read();
    file_delete();
}
void setup()
{
    u8 ret;
    ebox_init();
    LocalTime.begin();
    LocalTime.dateTime.parse(String(__DATE__),String(__TIME__),8);

    UART.begin(115200);
    print_log();
    ret = fs.mount(&sd);
    if(ret)
    {
        UART.printf("mount err:%d\n",ret);
    }
    else
    {
        UART.printf("mount ok\n");
    }
    LocalTime.print(UART);
    fileOpt();
}
int main(void)
{
    setup();
    while(1)
    {
        LocalTime.update();
        delay_ms(1000);
    }


}




