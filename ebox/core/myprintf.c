
#include <stdarg.h>
#include "stdlib.h"
#include "ebox_mem.h"

#include "myprintf.h"



int g_zero_flag=0;//标号 标志对齐方式,为0时表示右对齐方式

#define IsDigital(c)		((c>='0') && (c<='9'))//判断是否为数字 

const char *digits = "0123456789abcdefghijklmnopqrstuvwxyz";
char *strbuf;
void ReverseStr(char *str);
int mystrlen(char *str)//求字符串长度
{
	int i=0;
	while(*(str++))
		i++;
	return i;
} 


int Str2Digital(const char **str)//将字符串转换成整数
{
	int i=0;
	while(IsDigital(**str))
		i = i*10+*((*str)++) - '0';
	return i;
}
int float2str(double num,char *str,int precision)//浮点数转换成字符串
{
    unsigned long int_part;
    double remainder;
    int n;
    double rounding = 0.5;
    unsigned int toPrint;
    int i;

    if (num < 0.0)
    {
        num = -num;
    }
  
    rounding = 0.5;
    for (i=0; i<precision; ++i)
        rounding /= 10.0;

    num += rounding;

    int_part = (unsigned long)num;
    remainder = num - (double)int_part;
    
    n = Int2Str(int_part,str);
    str[n++] = '.';
    while (precision-- > 0)
    {
        remainder *= 10.0;
        toPrint = (unsigned int)(remainder);
        str[n++] = toPrint+0x30;
        remainder -= toPrint; 
    } 
    str[n] = '\0';
    return n;
}
int format_str(char *dst,char *src, int len,int width, int flag)
{
    int space_len = 0;
    if(width == -1 || width <= len) 
    {   
        width =len;
        ebox_memcpy(dst,src,width);
        dst+=len;
        *dst = '\0';
        return len;
    }
    else
    { 
        space_len = width - len;
        if(!flag)
        {
            while(space_len--)
            {
                if(g_zero_flag)
                    *dst++ = '0';
                else
                    *dst++ = ' ';
            }
            ebox_memcpy(dst,src,len);
            dst+=len;

        }
        else
        {
            ebox_memcpy(dst,src,len);
            dst+=len;
            while(space_len--)
            {
                if(g_zero_flag)
                    *dst++ = '0';
                else
                    *dst++ = ' ';
            }
        }
    }
    *dst = '\0';
    return width;
}
int float2str_format(double num,char *str,int precision,int width, int flag)
{
    int n = 0;
    int len = 0;
    char buf[64];
    
    len = float2str(num,buf,precision);
    n = format_str(str,buf,len,width,flag);
    
    return n;
}
int Int2Str(int num,char *str)//整数转换成字符串
{
	int k=0;
    int len = 0;
	if(num <0)
	{	
		num = -num;
		*str++='-';
        len++;
	}
    if(num == 0)
    {
        *str++='0';
        len++;
        *str = '\0';
        return 	len;
    }
	while(num)
	{
			*str= *(num%10 +digits);
			num=num/10;
			str++;
			k++;
            len++;
	}
	*str = '\0';
	ReverseStr(str-k);
return 	len;
}

int int2str_format(int num,char *dst,int width, int flag)//整数转换成字符串
{
    int len = 0;
    char buf[64];
    
    int space_len = 0;
    len = Int2Str(num,buf);


    if(width == -1 || width <= len) 
    {   
        width =len;
        ebox_memcpy(dst,buf,len);
        dst+=len;
    }
    else
    { 
        space_len = width - len;
        if(!flag)
        {
            while(space_len--)
            {
                if(g_zero_flag)
                    *dst++ = '0';
                else
                    *dst++ = ' ';
            }
            ebox_memcpy(dst,buf,len);
            dst+=len;

        }
        else
        {
            ebox_memcpy(dst,buf,len);
            dst+=len;
            while(space_len--)
            {
                    *dst++ = ' ';
            }
        }
    }
    *dst = '\0';
    return width;    

}

void ReverseStr(char *str)//字符串倒置
{
	char tmp;
	int i,len;
	
	len = mystrlen(str);
	for(i=0;i<len/2;i++)
	{
		tmp =*(str+i);
		*(str+i) = *(str+len-1-i);
		*(str+len-1-i) = tmp;		
	}
}
void Convert(int num,int n , char *str)//十进制转换成任意进制
{
	while(num)
	{
			*str= *(num%n +digits);
			num=num/n;
			str++;
	}
	*str = '\0';
}

char * AddBuf(char *buf,char *str,int precision,int width,int flag)//字符串添加到buf里
{
	int len = 0;
	int i;
	len =mystrlen(str);
//	printf("len:%dstr:%s",len,str);
	
	if( precision < 0)
		precision = len;
	else if(len > precision)
		len =precision;
	if(!flag)
		while(--width >0 && width-len > 0)
			*buf++ = ' ';
	for(i=0;i<len;i++)
		*buf++ = *str++;
	while(--width>0 && width-len>0)
		*buf++ = ' ';
	return buf;
}


#define OUTCHAR(str, len, size, ch)                                          \
do {                                                                         \
	if (len + 1 < size)                                                  \
		str[len] = ch;                                               \
	(len)++;                                                             \
} while (/* CONSTCOND */ 0)
void AddStrBuf(char *buf, char *str, size_t *len, size_t size,int width)//字符串添加到buf里
{
	int _len = 0;
	int integer_len;
    int space_len;
	_len =mystrlen(str);

    while(_len--)
        OUTCHAR(buf,*len,size,*str++);
}

int _ebox_vsnprintf(char *buf,size_t size,const char *format,va_list args)
{
	
	int width;//字符宽度
	int precision;//精确度
	int qualifier;//修饰符
	int flag=0;//标号 标志对齐方式,为0时表示右对齐方式
	size_t len=0;//用于记录str长度
	char *p;
	char *str;
    
//	int i;
	//char *pbuf=buf;
	str = p =(char *)ebox_malloc(64);	
	while(*format)
	{

		if(*format !='%')
        {
            OUTCHAR(buf, len, size, *format);
            format++;
        }
		else//format =='%'
			{
				format++;
				//检查对齐方式
				if(*format == '-')
				{
					flag=1;//左对齐方式
					format++;
				}
				//检查字符宽度
				width = -1;
				if(IsDigital(*format))
                {
                    if(*format == '0')
                        g_zero_flag = 1;
                    else
                        g_zero_flag = 0;
					width = Str2Digital(&format);
                }
				else if(*format == '*')
				{
					width = va_arg(args,int);
					if(width <0)
					{
						width = -width;
						flag =1;
					}
					format++;
				}	
				//检查精确度
				precision= -1;
				if(*format == '.')
				{
					format++;
					if(IsDigital(*format))
						precision = Str2Digital(&format);
					else if(*format == '*')
					{
						precision = va_arg(args,int);
						if(precision < 0)
							precision = 0;
						format++;
					}
				}
				//检查修饰符
				qualifier = -1;
				if(*format == 'h' ||*format == 'l' ||*format == 'L')
				{
					qualifier = *format;
					format ++;
				}
				//检查格式打印字符			
				
				switch(*format)
				{
						case 'c':
							if(!flag)//右对齐方式	
                            {                                
								while(--width>0)
                                {
                                    OUTCHAR(buf, len, size, ' ');

                                }
                            }
                            
                            OUTCHAR(buf, len, size, va_arg(args,int));
                            
							while(--width>0)//左对齐
                            {
                                    OUTCHAR(buf, len, size, ' ');
                            }                                
							break;
						case 's':
                            str = p;
                            str = va_arg(args,char *);
							AddStrBuf(buf,str,&len,size,width);
							break;
						case 'o':
                            str = p;
							Convert(va_arg(args,int), 8, str);
							ReverseStr(str);
							AddStrBuf(buf,str,&len,size,width);
							break;
						case 'd':
                            str = p;
                            width = int2str_format(va_arg(args,int),str,width,flag);
							AddStrBuf(buf,str,&len,size,width);
							break;
						case 'x':
                            str = p;
							Convert(va_arg(args,int),16,str);
							ReverseStr(str);
							AddStrBuf(buf,str,&len,size,width);
							break;
						case 'f':
                            str = p;
							width = float2str_format(va_arg(args,double),str,precision,width,flag);
							AddStrBuf(buf,str,&len,size,width);
							break;
						default:
							break;
				}
				format++;
				continue;
			}
			
	}
    if(len < size)
        buf[len] = '\0';
    else if(size > 0)
		buf[size - 1] = '\0';

	ebox_free(p);
	return (int)len;
}

int _ebox_snprintf(char *str, size_t size,const char *format,...)
{
	va_list args;
	int n ;

	va_start(args,format);
	n = _ebox_vsnprintf(str,size,format,args);	
	va_end(args);
	return n;
}
int _ebox_vsprintf(char *ret, const char *format, va_list ap)
{
	size_t size;
	int len;
	va_list aq;

	memcpy(&aq, &ap,sizeof(ap));
    
	len = _ebox_vsnprintf(NULL, 0, format, aq);

	if (len < 0 || (ret = (char *)malloc(size = len + 1)) == NULL)
		return -1;
	return _ebox_vsnprintf(ret, size, format, ap);
}


int _ebox_sprintf(char *str,const char *format,...)
{
//	va_list args;
//	int n ;
//    
//    memcpy(args,);
//	va_start(args,format);
//	n =MyVsnprintf(str,size,format,args);	
//	va_end(args);
//	return n;
    
    va_list ap;
	int len;

	va_start(ap, format);
	len = _ebox_vsprintf(str, format, ap);
	va_end(ap);
	return len;
}
//void vsn_test(const char *fmt, ...)
//{
//    char buf[100];
//    size_t  size1 = 0;
//    size_t  size2 = 32;
//    va_list va_params;

//    va_start(va_params, fmt);

//    size1 = MyVsnprintf(buf,size2,fmt, va_params);

//    va_end(va_params);

//    
//}
