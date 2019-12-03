/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox
#include "ebox.h"
#include "mmc_sd.h"
#include "wrapperdiskio.h"
#include "ff.h"
#include "w25xxx.h"
#include "bsp_ebox.h"
#include "fatstructs.h"
mbr_t *mbr;
part_t  *part;
fat32_boot_t *bios;
//SD sd(&PB12, &spi2);

W25xxx flash(&PA15, &spi1);


static FATFS fs;            // Work area (file system object) for logical drive
FATFS *fss;
//DIR DirObject;       //Ŀ¼�ṹ
//FIL fsrc;            // �ļ��ṹ
FILINFO FilInfo;
FILINFO FilInfo1;
FRESULT res;


u8 mf_scan_files(const char * path)
{
    DIR DirObject;       //Ŀ¼�ṹ
    FRESULT res;
    char *fn;   /* This function is assuming non-Unicode cfg. */
	  char *p; 
#if _USE_LFN
    fileinfo.lfsize = _MAX_LFN * 2 + 1;
    fileinfo.lfname = mymalloc(SRAMIN,fileinfo.lfsize);
#endif
    uart1.printf("�鿴Ŀ¼��%s\r\n",path);

    res = f_opendir(&DirObject,path); //��һ��Ŀ¼
    if (res == FR_OK)
    {
        while(1)
        {
            res = f_readdir(&DirObject, &FilInfo);                   //��ȡĿ¼�µ�һ���ļ�
            if (res != FR_OK || FilInfo.fname[0] == 0) 
            {
                 uart1.printf("===============\r\n");
                break;  //������/��ĩβ��,�˳�
            }
 
#if _USE_LFN
            fn = *fileinfo.lfname ? fileinfo.lfname : fileinfo.fname;
#else
             fn = FilInfo.fname;				  
#endif                                               /* It is a file. */
			 
                 uart1.printf("%s\r\n",FilInfo.fname);
			 FilInfo = FilInfo1;
        }
    }
    else  
    {
        uart1.printf("����%d\r\n",res);

    }
    return res;
}


void fileOpt()
{
    u8 ret;
    u8 buf[100];
    u8 readBuf[6] ;
    u32 bw = 0;
    u32 br = 0;
    FIL fsrc;            // �ļ��ṹ

    uint32_t total = 4097;
    uint16_t counter;
    for(int i = 0; i < 100; i++)
        buf[i] = '1';
    res = f_open(&fsrc, "0:12345.txt", FA_WRITE | FA_READ | FA_CREATE_ALWAYS); //û������ļ��򴴽����ļ�
    uart1.printf("\r\n");

    if(res == FR_OK)
    {
        uart1.printf("open/make file  O(��_��)O\r\n");
        uart1.printf("file flag:%d\r\n", fsrc.flag);
        uart1.printf("file size��%d\r\n", fsrc.fsize);
        uart1.printf("file ptr(start location)��%d\r\n", fsrc.fptr);
        //		uart1.printf("���ļ���ʼ�غ�:%d\r\n",fsrc.org_clust);
        //		uart1.printf("���ļ���ǰ�غţ�%d\r\n",fsrc.curr_clust);
        uart1.printf("dsect num:%d\r\n", fsrc.dsect);

        f_lseek(&fsrc, 0);
        do
        {
            res = f_write(&fsrc, buf, sizeof(buf), &bw);
            if(res)
            {
                uart1.printf("write error : %d\r\n", res);
                break;
            }
            counter += bw;
//            uart1.printf("write ok!\r\n");
        }
        while (counter < total);  //  �ж��Ƿ�д��(bw > 100����ʾд�����)
        uart1.printf("write %d ok!\r\n",counter);
    }
    else if(res == FR_EXIST)
        uart1.printf("file exist\r\n");
    else
        uart1.printf("creat/open failed~~~~(>_<)~~~~ %d\r\n", res);
    f_close(&fsrc);//�ر��ļ�

    /////////////////////////////////////////////////////////////////////////////////////
    u32 readsize;
    u32 buflen;
    buflen = sizeof(readBuf);
    res = f_open(&fsrc, "0:12345.txt", FA_READ); //û������ļ��򴴽����ļ�
    if(res == FR_OK)
    {
        uart1.printf("file size��%d\r\n", fsrc.fsize);
    }
    readsize = 0;
    do
    {
        res = f_read(&fsrc, readBuf, buflen, &br);
        if(res == FR_OK)
        {
//            uart1.write((const char *)readBuf, sizeof(readBuf));
        }
        else
        {
            uart1.printf("\r\nread failed\r\n");
        }
        readsize += buflen;
        f_lseek(&fsrc, readsize);

    }
    while(br == buflen);
    uart1.printf("\r\nread %d end\r\n",readsize);
    f_close(&fsrc);//�ر��ļ�
    f_mount(&fs, "0:", 0);
}
    uint8_t buf[5000];

void setup()
{
    u8 ret;
    ebox_init();
    uart1.begin(256000);
    print_log();

//    flash.begin();
//    flash.erase_chip();
//    flash.read(buf,0,4096);
//    uart1.printf("=====sct 0====\r\n");
//    for(int i = 0; i < 256; i++)    {
//        for(int j = 0; j < 16; j++)    {
//            uart1.printf("0X%02X ",buf[i*16 + j]);
//            }
//        uart1.printf("\r\n");
//    }

    attach_sd_to_fat(0,&flash);

    res = f_mount(&fs, "0", 1);
    if(res == FR_OK)
        uart1.printf("mount ok!\r\n", res);
    else
        uart1.printf("mount err!err = %d\r\n", res);
    f_mkfs("0",0,4096);
    fileOpt();
    mf_scan_files("0:");
    
//    flash.read_sector(buf,0,1);
//    uart1.printf("=====sct %d====\r\n",0);
//    for(int i = 0; i < 256; i++)    {
//        for(int j = 0; j < 16; j++)    {
//            uart1.printf("0X%02X ",buf[i*16 + j]);
//            }
//        uart1.printf("\r\n");
//    }
    
    
    DataU32_t value;
    flash.read_sector(buf,0,1);
    mbr = (mbr_t *)buf;

    
    part = mbr->part;
    
    uart1.printf("diskSignature:%d\r\n",mbr->diskSignature);
    uart1.printf("usuallyZero:%d\r\n",mbr->usuallyZero);
    
    uart1.printf("totalSectors:%d\r\n",part->totalSectors);
    uart1.printf("beginSector:%d\r\n",part->beginSector);
    uart1.printf("firstSector:%d\r\n",part->firstSector);
    uart1.printf("endSector:%d\r\n",part->endSector);

    
    
    flash.read_sector(buf,63,1);
    bios = (fat32_boot_t *)buf;
    
    uart1.printf("fat32Version:%d\r\n",bios->fat32Version);
    uart1.printf("bytesPerSector:%d\r\n",bios->bytesPerSector);
    uart1.printf("sectorsPerCluster:%d\r\n",bios->sectorsPerCluster);
    uart1.printf("fatCount:%d\r\n",bios->fatCount);
    uart1.printf("fat32RootCluster:%d\r\n",bios->fat32RootCluster);

    
    uart1.printf("winsect:%d\r\n",fs.winsect);
    uart1.printf("fs_type:%d\r\n",fs.fs_type);
    uart1.printf("fsize:%d\r\n",fs.fsize);
    uart1.printf("csize:%d\r\n",fs.csize);
    uart1.printf("dirbase:%d\r\n",fs.dirbase);
    uart1.printf("database:%d\r\n",fs.database);

    uart1.printf("=====sct %d====\r\n",0);
    for(int i = 0; i < 256; i++)    {
        for(int j = 0; j < 16; j++)    {
            uart1.printf("0X%02X ",buf[i*16 + j]);
        }
        uart1.printf("\r\n");
    }
//    for(int sct = 63; sct < 69; sct++)    {
//        flash.read_sector(buf,sct,1);
//        uart1.printf("=====sct %d====\r\n",sct);
//        for(int i = 0; i < 256; i++)    {
//            for(int j = 0; j < 16; j++)    {
//                uart1.printf("0X%02X ",buf[i*16 + j]);
//                }
//            uart1.printf("\r\n");
//        }
//    }

}


int main(void)
{
    setup();
    while(1)
    {

        //uart1.printf("\r\nrunning��");
        delay_ms(1000);
    }


}




