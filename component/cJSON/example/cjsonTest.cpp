/**
  ******************************************************************************
  * @file    .cpp
  * @author  shentq
  * @version V1.2
  * @date    2016/08/14
  * @brief
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

#include  "../cjson/cjson.h"
#include "ebox_mem.h"
/**
	*	1	此例程演示了cJson使用方法
	*/


/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"cJSON example"
#define EXAMPLE_DATE	"2018-08-11"

/* Parse text to JSON, then render back to text, and print! */
void doit(char *text)
{
    char *out;
    cJSON *json;

    json = cJSON_Parse(text);
    if (!json)
    {
        uart1.printf("Error before: [%s]\n", cJSON_GetErrorPtr());
    }
    else
    {
        out = cJSON_Print(json);
        cJSON_Delete(json);
        uart1.printf("%s\n", out);
        ebox_free(out);
    }
}
/* Read a file, parse, render back, etc. */
void dofile(char *filename)
{
    FILE *f;
    long len;
    char *data;

    f = fopen(filename, "rb");
    fseek(f, 0, SEEK_END);
    len = ftell(f);
    fseek(f, 0, SEEK_SET);
    data = (char *)ebox_malloc(len + 1);
    fread(data, 1, len, f);
    fclose(f);
    doit(data);
    ebox_free(data);
}

/* Used by some code below as an example datatype. */
struct record
{
    const char *precision;
    double lat, lon;
    const char *address, *city, *state, *zip, *country;
};

/* Create a bunch of objects as demonstration. */
void create_objects()
{
    cJSON *root, *fmt, *img, *thm, *fld;
    char *out;
    int i;	/* declare a few. */
    /* Our "days of the week" array: */
    const char *strings[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
    /* Our matrix: */
    int numbers[3][3] = {{0, -1, 0}, {1, 0, 0}, {0, 0, 1}};
    /* Our "gallery" item: */
    int ids[4] = {116, 943, 234, 38793};
    /* Our array of "records": */
    struct record fields[2] =
    {
        {"zip", 37.7668, -1.223959e+2, "", "SAN FRANCISCO", "CA", "94107", "US"},
        {"zip", 37.371991, -1.22026e+2, "", "SUNNYVALE", "CA", "94085", "US"}
    };

    /* Here we construct some JSON standards, from the JSON site. */

    /* Our "Video" datatype: */
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "name", cJSON_CreateString("Jack (\"Bee\") Nimble"));
    cJSON_AddItemToObject(root, "format", fmt = cJSON_CreateObject());
    cJSON_AddStringToObject(fmt, "type",		"rect");
    cJSON_AddNumberToObject(fmt, "width",		1920);
    cJSON_AddNumberToObject(fmt, "height",		1080);
    cJSON_AddFalseToObject (fmt, "interlace");
    cJSON_AddNumberToObject(fmt, "frame rate",	24);

    out = cJSON_Print(root);
    cJSON_Delete(root);
    uart1.printf("%s\n", out);
    ebox_free(out);	/* Print to text, Delete the cJSON, print it, release the string. */

    /* Our "days of the week" array: */
    root = cJSON_CreateStringArray(strings, 7);

    out = cJSON_Print(root);
    cJSON_Delete(root);
    uart1.printf("%s\n", out);
    ebox_free(out);

    /* Our matrix: */
    root = cJSON_CreateArray();
    for (i = 0; i < 3; i++) cJSON_AddItemToArray(root, cJSON_CreateIntArray(numbers[i], 3));

    /*	cJSON_ReplaceItemInArray(root,1,cJSON_CreateString("Replacement")); */

    out = cJSON_Print(root);
    cJSON_Delete(root);
    uart1.printf("%s\n", out);
    ebox_free(out);


    /* Our "gallery" item: */
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "Image", img = cJSON_CreateObject());
    cJSON_AddNumberToObject(img, "Width", 800);
    cJSON_AddNumberToObject(img, "Height", 600);
    cJSON_AddStringToObject(img, "Title", "View from 15th Floor");
    cJSON_AddItemToObject(img, "Thumbnail", thm = cJSON_CreateObject());
    cJSON_AddStringToObject(thm, "Url", "http:/*www.example.com/image/481989943");
    cJSON_AddNumberToObject(thm, "Height", 125);
    cJSON_AddStringToObject(thm, "Width", "100");
    cJSON_AddItemToObject(img, "IDs", cJSON_CreateIntArray(ids, 4));

    out = cJSON_Print(root);
    cJSON_Delete(root);
    uart1.printf("%s\n", out);
    ebox_free(out);

    /* Our array of "records": */

    root = cJSON_CreateArray();
    for (i = 0; i < 2; i++)
    {
        cJSON_AddItemToArray(root, fld = cJSON_CreateObject());
        cJSON_AddStringToObject(fld, "precision", fields[i].precision);
        cJSON_AddNumberToObject(fld, "Latitude", fields[i].lat);
        cJSON_AddNumberToObject(fld, "Longitude", fields[i].lon);
        cJSON_AddStringToObject(fld, "Address", fields[i].address);
        cJSON_AddStringToObject(fld, "City", fields[i].city);
        cJSON_AddStringToObject(fld, "State", fields[i].state);
        cJSON_AddStringToObject(fld, "Zip", fields[i].zip);
        cJSON_AddStringToObject(fld, "Country", fields[i].country);
    }

    /*	cJSON_ReplaceItemInObject(cJSON_GetArrayItem(root,1),"City",cJSON_CreateIntArray(ids,4)); */

    out = cJSON_Print(root);
    cJSON_Delete(root);
    uart1.printf("%s\n", out);
    ebox_free(out);

}

char *xx;
void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);
    uart1.printf("free:%d\r\n", ebox_get_free());


    PB8.mode(OUTPUT_PP);
    struct record fields[2] =
    {
        {"zip", 37.7668, -1.223959e+2, "", "SAN FRANCISCO", "CA", "94107", "US"},
        {"zip", 37.371991, -1.22026e+2, "", "SUNNYVALE", "CA", "94085", "US"}
    };
    cJSON *root, *fmt, *img, *thm, *fld;
    char *out;
    int i;	/* declare a few. */

    root = cJSON_CreateArray();
    for (i = 0; i < 2; i++)
    {
        cJSON_AddItemToArray(root, fld = cJSON_CreateObject());
        cJSON_AddStringToObject(fld, "precision", fields[i].precision);
        cJSON_AddNumberToObject(fld, "Latitude", fields[i].lat);
        cJSON_AddNumberToObject(fld, "Longitude", fields[i].lon);
        cJSON_AddStringToObject(fld, "Address", fields[i].address);
        cJSON_AddStringToObject(fld, "City", fields[i].city);
        cJSON_AddStringToObject(fld, "State", fields[i].state);
        cJSON_AddStringToObject(fld, "Zip", fields[i].zip);
        cJSON_AddStringToObject(fld, "Country", fields[i].country);
    }

    /*	cJSON_ReplaceItemInObject(cJSON_GetArrayItem(root,1),"City",cJSON_CreateIntArray(ids,4)); */
    uart1.printf("free:%d\r\n", ebox_get_free());
    //	out = (char *)cjson_malloc(1000);
    out = cJSON_Print(root);
    cJSON_Delete(root);
    uart1.printf("%s\n", out);
    uart1.printf("free:%d\r\n", ebox_get_free());
    ebox_free(out);
    uart1.printf("free:%d\r\n", ebox_get_free());

}


int main(void)
{
    setup();
    while(1)
    {
        delay_ms(500);
        uart1.printf("free:%d\r\n", ebox_get_free());

    }


}








