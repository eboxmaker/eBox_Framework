#ifndef _MY_FF
#define _MY_FF

#include "ff.h"

typedef struct
{
  char name[18];

}FILE_NAME_LIT;

void Del_oneWeekAgo(void);

void Del_files(
    char* FileName  /* Pointer to the object name */
);

void Del_All_bmp(char* path);

void Del_some_bmp(u16 down,u16 up);

void Load_files	(
    char* path,		// Pointer to the path name working buffer //
	char* fil_ext_lw,		// Pointer to the path name working buffer //
	char* fil_ext_up,		// Pointer to the path name working buffer //
    void (*pfun)(char*)
);


unsigned char List_files	(
    char* path,		     // Pointer to the path name working buffer //
    DIR *start_dirs,     //目录起点信息
	unsigned int page,   //页码
	FILE_NAME_LIT  *File_name
);


void My_scan_files(void);

#endif