
#include <stdarg.h>
#include <stdlib.h>
#include "ebox_mem.h"

#include "myprintf.h"


int g_zero_flag=0;//��� ��־���뷽ʽ,Ϊ0ʱ��ʾ�Ҷ��뷽ʽ

#define isDigital(c)		((c>='0') && (c<='9'))//�ж��Ƿ�Ϊ���� 


#define OUTCHAR(str, len, size, ch)                                          \
do {                                                                         \
	if (len + 1 < size)                                                  \
		str[len] = ch;                                               \
	(len)++;                                                             \
} while (/* CONSTCOND */ 0)

//static bool isHex(char c);
static int format_num_str(char *dst,char *src, int len,int width, int flag);

//����������
static int float2str(double value,char *str,int precision);//������ת�����ַ���
static int float2str_format(double value,char *str,int precision,int width, int flag);

//��������
static int int2str(int value,char *str);//����ת�����ַ���
static int int2str_format(int value,char *dst,int width, int flag);//����ת�����ַ���
static int convert2str_format(int value,int base, int caps,char *dst,int width, int flag);

static int  str2digital(const char **str);//���ַ���ת��������
static void reverseStr(char *str);
static int convert(int value,char *buf, int base, int caps);//ʮ����ת�����������

static void addStrToBuf(char *buf, char *str, size_t *len, size_t size,int width);//�ַ�����ӵ�buf��


int _ebox_vsnprintf(char *buf,size_t size,const char *format,va_list args)
{
	
	int width;//�ַ����
	int precision;//��ȷ��
	int qualifier;//���η�
	int flag=0;//��� ��־���뷽ʽ,Ϊ0ʱ��ʾ�Ҷ��뷽ʽ
	size_t len=0;//���ڼ�¼str����
	char *p;
	char *str;
    

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
				//�����뷽ʽ
				if(*format == '-')
				{
					flag=1;//����뷽ʽ
					format++;
				}
				//����ַ����
				width = -1;
				if(isDigital(*format))
                {
                    if(*format == '0')
                        g_zero_flag = 1;
                    else
                        g_zero_flag = 0;
					width = str2digital(&format);
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
				//��龫ȷ��
				precision= -1;
				if(*format == '.')
				{
					format++;
					if(isDigital(*format))
						precision = str2digital(&format);
					else if(*format == '*')
					{
						precision = va_arg(args,int);
						if(precision < 0)
							precision = 0;
						format++;
					}
				}
				//������η�
				qualifier = -1;
				if(*format == 'h' ||*format == 'l' ||*format == 'L')
				{
					qualifier = *format;
					format ++;
				}
				//����ʽ��ӡ�ַ�			
				
				switch(*format)
				{
						case 'c':
						case 'C':
							if(!flag)//�Ҷ��뷽ʽ	
                            {                                
								while(--width>0)
                                {
                                    OUTCHAR(buf, len, size, ' ');

                                }
                            }
                            
                            OUTCHAR(buf, len, size, va_arg(args,int));
                            
							while(--width>0)//�����
                            {
                                    OUTCHAR(buf, len, size, ' ');
                            }                                
							break;
						case 's':
                            str = p;
                            str = va_arg(args,char *);
							addStrToBuf(buf,str,&len,size,width);
							break;
						case 'o':
                            str = p;
							convert(va_arg(args,int),str,16,1);
							addStrToBuf(buf,str,&len,size,width);
							break;
						case 'd':
						case 'D':
                            str = p;
                            width = int2str_format(va_arg(args,int),str,width,flag);
							addStrToBuf(buf,str,&len,size,width);
							break;
						case 'x':
						case 'X':
                            str = p;
							convert2str_format(va_arg(args,int),16,1,str,width,flag);
							addStrToBuf(buf,str,&len,size,width);
							break;
						case 'f':
                            str = p;
							width = float2str_format(va_arg(args,double),str,precision,width,flag);
							addStrToBuf(buf,str,&len,size,width);
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
    char *temp;

	memcpy(&aq, &ap,sizeof(ap));
    
	len = _ebox_vsnprintf(NULL, 0, format, aq);

	if (len < 0 || (temp = (char *)ebox_malloc(size = len + 1)) == NULL)
		return -1;
	len = _ebox_vsnprintf(temp, size, format, ap);
    ebox_memcpy(ret,temp,len);
    ebox_free(temp);
    return len;
}


int _ebox_sprintf(char *str,const char *format,...)
{

    
    va_list ap;
	int len;

	va_start(ap, format);
	len = _ebox_vsprintf(str, format, ap);
	va_end(ap);
	return len;
}


int float2str(double value,char *str,int precision)//������ת�����ַ���
{
    unsigned long int_part = 0;
    double remainder = 0;
    int n = 0;
    double rounding = 0.5;
    unsigned int toPrint = 0;
    int i = 0;

//    char str[30];
    if (value < 0.0)
    {
        value = -value;
        str[n++] = '-';
    }
  
    rounding = 0.5;
    for (i=0; i<precision; ++i)
        rounding /= 10.0;

    value += rounding;

    int_part = (unsigned long)value;
    remainder = value - (double)int_part;
    
    n += int2str(int_part,&str[n]);
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
int format_num_str(char *dst,char *src, int len,int width, int flag)
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
                {
                    if(*src == '-')
                    {
                        *dst++ = '-';
                        src++;
                    }
                    else
                        *dst++ = '0';
                }
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
            while(len--)
            {
                if(isDigital(*src) != 1 && *src != '.' && *src != '-')
                {
                    src++;
                    g_zero_flag = 0;
                }
            }
                
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
static int float2str_format(double value,char *str,int precision,int width, int flag)
{
    int n = 0;
    int len = 0;
    char buf[64];
    
    len = float2str(value,buf,precision);
    n = format_num_str(str,buf,len,width,flag);
    
    return n;
}
static int int2str(int value,char *str)//����ת�����ַ���
{
	const char *digits =  "0123456789ABCDEF" ;
	int k=0;
    int len = 0;
	if(value <0)
	{	
		value = -value;
		*str++='-';
        len++;
	}
    if(value == 0)
    {
        *str++='0';
        len++;
        *str = '\0';
        return 	len;
    }
	while(value)
	{
			*str= *(value%10 +digits);
			value=value/10;
			str++;
			k++;
            len++;
	}
	*str = '\0';
	reverseStr(str-k);
return 	len;
}
static int int2str_format(int value,char *dst,int width, int flag)//����ת�����ַ���
{
    int n;
    int len = 0;
    char buf[64];
    
    len = int2str(value,buf);
    n = format_num_str(dst,buf,len,width,flag);

    return n;    

}
static int convert2str_format(int value,int base, int caps,char *dst,int width, int flag)
{
    
    int n = 0;
    int len = 0;
    char buf[64];
    
    len = convert(value,buf,base,caps);
    reverseStr(buf);
    n = format_num_str(dst,buf,len,width,flag);
    
    return n;
}

static int str2digital(const char **str)//���ַ���ת��������
{
	int i=0;
	while(isDigital(**str))
		i = i*10+*((*str)++) - '0';
	return i;
}

static void reverseStr(char *str)//�ַ�������
{
	char tmp;
	int i,len;
	
	len = strlen(str);
	for(i=0;i<len/2;i++)
	{
		tmp =*(str+i);
		*(str+i) = *(str+len-1-i);
		*(str+len-1-i) = tmp;		
	}
}
static int convert(int value,char *buf, int base, int caps)//ʮ����ת�����������
{
	const char *digits = caps ? "0123456789ABCDEF" : "0123456789abcdef";
	size_t pos = 0;
    
    	/* We return an unterminated buffer with the digits in reverse order. */
	do {
		buf[pos++] = digits[value % base];
		value /= base;
	} while (value != 0 );

    buf[pos] = '\0';
	return (int)pos;
    
}
static void addStrToBuf(char *buf, char *str, size_t *len, size_t size,int width)//�ַ�����ӵ�buf��
{
	int _len = 0;

	_len = strlen(str);

    while(_len--)
        OUTCHAR(buf,*len,size,*str++);
}
//static bool isHex(char c)
//{
//	if((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F' ))
//        return true;
//    else
//        return false;
//}
