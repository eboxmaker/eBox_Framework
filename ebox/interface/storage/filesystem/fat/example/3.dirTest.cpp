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
#include "interface/storage/filesystem/dir.h"
#include "rtcmillis.h"
#include "ebox_mem.h"

FATFileSystem fs("sd");

W25xxx flash(&PA15, &spi1);
SD sd(&PB12, &spi2);
RtcMillis LocalTime;

void dir_create()
{
    int ret;
    UART.printf("mkdir...\n");
    for(int i = 0; i < 10; i++)
    {
        String path("/ebox_");
        path += i;
        ret = fs.mkdir(path.c_str(),O_CREAT);
        if(ret)
        {
            UART.printf("mkdir[%s]:%d\n",path.c_str(), ret);
        }
        else
        {
            UART.printf("mkdir:[%s]...ok\n", path.c_str());
        }
        UART.flush();
    }
    
}
int leve = 0;
void dir_tree(const char *path)
{
    int ret;
    Dir dir;
    ret = dir.open(&fs,path);
    if(ret)
    {
        UART.printf("open dir err:%d\n",ret);
    }
    else
    {
        leve++;
    }
    UART.flush();
    struct dirent de;
    while(1)
    {
        String str = "";
        ret = dir.read(&de);
        if(ret)
        {
            for(int i = 1; i < leve; i++)
                UART.printf("\t|");
            UART.printf("%s\n", &(de.d_name)[0]);
            if(de.d_type == DT_DIR)
            {
                str += path;
                str += "/";
                str += de.d_name;
                dir_tree(str.c_str());
            }
        }
        else
        {
            break;
        }
        UART.flush();
    } 
    leve--;
}
void dir_delete()
{
    int ret=0;
    UART.printf("delet dir...\n");
    for(int i = 0; i < 10; i++)
    {
        String path("/ebox_");
        path += i;
        ret = fs.remove(path.c_str());
        if(ret)
        {
            UART.printf("remove[%s]:%d\n",path.c_str(), ret);
        }
        else
        {
            UART.printf("remove:[%s]\n", path.c_str());
        }
        UART.flush();
    }  
}


void dirOpt()
{
    int ret;
    dir_create();
    UART.printf("read dir...\n");
    dir_tree("/");
    dir_delete();
 
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
    dirOpt();
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




