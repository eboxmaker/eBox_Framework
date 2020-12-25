#include "charcoding.h "
#include "stdio.h"
#include <stdlib.h>
#include <string.h>
 
// 将Unicode（UCS-2LE）编码转换成UTF8编码
int UnicodeToUtf8(char* pInput, char *pOutput)  
{  
	int len = 0; //记录转换后的Utf8字符串的字节数
	while (*pInput)
	{
		//处理一个unicode字符
		char low = *pInput;//取出unicode字符的低8位
		pInput++;
		char high = *pInput;//取出unicode字符的高8位
		int w=high<<8;
		unsigned  wchar = (high<<8)+low;//高8位和低8位组成一个unicode字符,加法运算级别高
 
		if (wchar <= 0x7F ) //英文字符
		{   
			pOutput[len] = (char)wchar;  //取wchar的低8位
			len++;
		}  
		else if (wchar >=0x80 && wchar <= 0x7FF)  //可以转换成双字节pOutput字符
		{  
			pOutput[len] = 0xc0 |((wchar >> 6)&0x1f);  //取出unicode编码低6位后的5位，填充到110yyyyy 10zzzzzz 的yyyyy中
			len++;
			pOutput[len] = 0x80 | (wchar & 0x3f);  //取出unicode编码的低6位，填充到110yyyyy 10zzzzzz 的zzzzzz中
			len++;
		}  
		else if (wchar >=0x800 && wchar < 0xFFFF)  //可以转换成3个字节的pOutput字符
		{  
			pOutput[len] = 0xe0 | ((wchar >> 12)&0x0f);  //高四位填入1110xxxx 10yyyyyy 10zzzzzz中的xxxx
			len++;
			pOutput[len] = 0x80 | ((wchar >> 6) & 0x3f);  //中间6位填入1110xxxx 10yyyyyy 10zzzzzz中的yyyyyy
			len++;
			pOutput[len] = 0x80 | (wchar & 0x3f);  //低6位填入1110xxxx 10yyyyyy 10zzzzzz中的zzzzzz
			len++;
		}  
 
		else //对于其他字节数的unicode字符不进行处理
		{
			return -1;
		}
		pInput ++;//处理下一个unicode字符
	}
	//utf8字符串后面，有个\0
	pOutput [len]= 0;
	return len;  
}  
 
/*************************************************************************************************
* 将UTF8编码转换成Unicode（UCS-2LE）编码  低地址存低位字节
* 参数：
*    char* pInput     输入字符串
*    char*pOutput   输出字符串
* 返回值：转换后的Unicode字符串的字节数，如果出错则返回-1
**************************************************************************************************/
//utf8转unicode
int Utf8ToUnicode(char* pInput, char* pOutput)
{
	int outputSize = 0; //记录转换后的Unicode字符串的字节数
 
	while (*pInput)
	{
		if (*pInput > 0x00 && *pInput <= 0x7F) //处理单字节UTF8字符（英文字母、数字）
		{
			*pOutput = *pInput;
			 pOutput++;
			*pOutput = 0; //小端法表示，在高地址填补0
		}
		else if (((*pInput) & 0xE0) == 0xC0) //处理双字节UTF8字符
		{
			char high = *pInput;
			pInput++;
			char low = *pInput;
			if ((low & 0xC0) != 0x80)  //检查是否为合法的UTF8字符表示
			{
				return -1; //如果不是则报错
			}
 
			*pOutput = (high << 6) + (low & 0x3F);
			pOutput++;
			*pOutput = (high >> 2) & 0x07;
		}
		else if (((*pInput) & 0xF0) == 0xE0) //处理三字节UTF8字符
		{
			char high = *pInput;
			pInput++;
			char middle = *pInput;
			pInput++;
			char low = *pInput;
			if (((middle & 0xC0) != 0x80) || ((low & 0xC0) != 0x80))
			{
				return -1;
			}
			*pOutput = (middle << 6) + (low & 0x3F);//取出middle的低两位与low的低6位，组合成unicode字符的低8位
			pOutput++;
			*pOutput = (high << 4) + ((middle >> 2) & 0x0F); //取出high的低四位与middle的中间四位，组合成unicode字符的高8位
		}
		else //对于其他字节数的UTF8字符不进行处理
		{
			return -1;
		}
		pInput ++;//处理下一个utf8字符
		pOutput ++;
		outputSize += 2;
	}
	//unicode字符串后面，有两个\0
	*pOutput = 0;
	 pOutput++;
	*pOutput = 0;
	return outputSize;
}



// 将Utf8转成字符串形式
int Utf8ToStr(char* pInput, char* pOutput){
 
	char *ptemp = (char*)malloc(strlen(pInput)/3*2);
	int num = Utf8ToUnicode(pInput, ptemp);
	if (num != -1){
		unsigned char* p = (unsigned char*)ptemp;
		for (int i = 0; i <= num/2; i=i+2)
		{
			sprintf(&pOutput[strlen(pOutput)], "\\u%x%x", p[i+1], p[i]);
		}
	}
	return num;
}
 
// 
////  utf8转为Unicode，让pad可显示中文
//int SwitchToGbk(const unsigned char* pszBufIn, int nBufInLen, unsigned char* pszBufOut, int* pnBufOutLen)
//{
//	int i = 0;
//	int j = 0, nLen;
//	unsigned short unicode;
//	unsigned short gbk;
//	unsigned char temp;
//	
// 
//	for(; i < nBufInLen; i++, j++)
//	{
//		if((pszBufIn[i] & 0x80) == 0x00)		// 1?
//		{
//			nLen = 1;
//			pszBufOut[j]= pszBufIn[i];
//		}
//	/*		
//		else if((pszBufIn[i] & 0xE0) == 0xC0)// 2?
//		{
//			nLen = 2;
//			unicode = (pszBufIn[i] & 0x1F << 6) | (pszBufIn[i+1]& 0x3F);
//		}*/
//		else if ((pszBufIn[i] & 0xF0) == 0xE0) // 3? 
//		{
// 
//			if (i+ 2 >= nBufInLen) return -1; 
//			unicode = (((int)(pszBufIn[i] & 0x0F)) << 12) | (((int)(pszBufIn[i+1] & 0x3F)) << 6) | (pszBufIn[i+2]  & 0x3F); 
//			gbk = mb_uni2gb_table[unicode-0x4e00];
//			
//			pszBufOut[j]= gbk/256;
//			pszBufOut[j+1] = gbk%256;
//			j++;
//			i+=2;
//		}
//		else
//		{
//			return -1;
//		}
//	}
////	*pnBufOutLen = j;
// 
//	return 0;
//}
 
 
// hex型字符转hex
char *str2hex(const char *buf, char *dst)
{
 char buf_temp[2400] = {0};
	int x = 0;
	unsigned long i;
	while(*buf != '\0')
	{
		if(*buf == '\\')
		{
			strcpy(buf_temp,buf);
			*buf_temp = '0';
			*(buf_temp + 4) = '\0';
			i = strtoul(buf_temp, NULL, 16);
			dst[x] = i;
			buf += 3;
		}
		else
		{
			dst[x] = *buf;
		}
		x++;
		buf++;
	}
	dst[x] = '\0';
	return dst;
}
 
// 
///***********************************************************************/
// 
////一个调用示例
//void EnDeCodeDemo(){
//	//汉字“我”的UTF8编码是0xe68891，Unicode编码是 0x6211
//	//1、unicode转utf8
//	char unicodeStr[3]={0x11,0x62,0x00};//“我”的unicode编码是0x6211，按低地址存低位字节
//	char utf8Str[10]={0};
//	memset(utf8Str,0,5);
//	int num = UnicodeToUtf8(unicodeStr,utf8Str);
// 
//	unsigned char* p = (unsigned char*)utf8Str;   
//	for (int i = 0; i < num; i++)
//	{
//		printf("%0x", *p);
//		p++;
//	}//输出e68891
//	printf("\n");
// 
//	//2、utf8转unicode
//	char utf8Str2[4] = {0xe6, 0x88, 0x91, 0x00}; 
//	char unicodeStr2[10]={0};
//	memset(unicodeStr2,0,8);
//	int num2 = Utf8ToUnicode(utf8Str2, unicodeStr2);
//	if (num2 == -1)
//	{
//		printf("Error!\n");
//	}
//	else
//	{
//	  unsigned char* p = (unsigned char*)unicodeStr2;
//		for (int i = 0; i < num2; i++)
//		{
//			printf("%0x", *p);
//			p++;
//		}//输出1162
//		printf("\n");
//	}
// 
// 
//	// 3、Utf8转成字符串
//	char utf8[10] = {0xe6, 0x88, 0x91, 0xe6, 0x88, 0x91, 0x00}; 
//	char utf8Str3[20]={0};
//	Utf8ToStr(utf8,utf8Str3);
//	printf("%s\r\n", utf8Str3);
// 
//    // 4、utf8转为Unicode，让pad可显示中文
//	char *utf8str = music_name->valuestring;
//	char gbkstr[100]={0};
//	int len;
//	SwitchToGbk((const u8*)utf8str,strlen(utf8str),(u8 *)gbkstr,&len);
//    printf("%s\r\n", gbkstr);
//}
// 
// 