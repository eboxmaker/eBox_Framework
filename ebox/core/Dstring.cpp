#include "Dstring.h"
#include "ebox_core.h"
char C2D(
    uint8_t c	/**< is a character('0'-'F') to convert to HEX */
)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'a' && c <= 'f')
        return 10 + c - 'a';
    if (c >= 'A' && c <= 'F')
        return 10 + c - 'A';

    return (char)c;
}
int atoi(
    char *str,	/**< is a pointer to convert */
    uint16_t base	/**< is a base value (must be in the range 2 - 16) */
)
{
    int c;              /* current char */
    int total = 0;         /* current total */
    int sign;           /* if '-', then negative, otherwise positive */

    /* skip whitespace */
    while ( isspace((char)*str) )
        str++;

    c = (int)(unsigned char) * str;
    sign = c;           /* save sign indication */
    if (c == '-' || c == '+')
        c = (int)(unsigned char) * str++;  /* skip sign */

    while (*str != 0)
        total = total * base + C2D(*str++);
    if(sign == '-')
        return -total;
    else
        return total;
}

/**
@brief	CONVERT STRING INTO HEX OR DECIMAL
@return	success - 1, fail - 0
*/
int ValidATOI(
    char *str, 	/**< is a pointer to string to be converted */
    int base, 	/**< is a base value (must be in the range 2 - 16) */
    int *ret		/**<  is a integer pointer to return */
)
{
    int c;
    char *tstr = str;
    if(str == 0 || *str == '\0') return 0;
    while(*tstr != '\0')
    {
        c = C2D(*tstr);
        if( c >= 0 && c < base) tstr++;
        else    return 0;
    }

    *ret = atoi(str, base);
    return 1;
}

int find_str(uint8_t *s_str, uint8_t *p_str, uint16_t count, uint16_t &seek)
{
    uint16_t _count = 1;
    uint16_t len = 0;
    seek = 0;
    uint8_t *temp_str = NULL;
    uint8_t *temp_ptr = NULL;
    uint8_t *temp_char = NULL;
    if(0 == s_str || 0 == p_str)
        return 0;
    for(temp_str = s_str; *temp_str != '\0'; temp_str++)	 //依次查找字符串
    {
        temp_char = temp_str; //指向当前字符串
        //比较
        for(temp_ptr = p_str; *temp_ptr != '\0'; temp_ptr++)
        {
            if(*temp_ptr != *temp_char)
            {
                len = 0;
                break;
            }
            temp_char++;
            len++;
        }
        if(*temp_ptr == '\0') //出现了所要查找的字符，退出
        {
            if(_count == count)
                return len;
            else
            {
                _count++;
                len = 0;
            }
        }
        seek++;  //当前偏移量加1
    }
    return 0;
}

//查找字符串
//查找两个字符串中间的字符串，count1,count2分别是两个字符串出现的第几次
uint16_t get_str(char *source, const char *begin, uint16_t count1, const char *end, uint16_t count2, char *out)
{
    uint16_t i;
    uint16_t len1;
    uint16_t len2;
    uint16_t index1 = 0;
    uint16_t index2 = 0;
    uint16_t length = 0;
    len1 = find_str((uint8_t *)source, (uint8_t *)begin, count1, index1);
    len2 = find_str((uint8_t *)source, (uint8_t *)end, count2, index2);
    length = index2 - index1 - len1;
    if((len1 != 0) && (len2 != 0))
    {
        for( i = 0; i < index2 - index1 - len1; i++)
            out[i] = source[index1 + len1 + i];
        out[i] = '\0';
    }
    return length;

}
//查找字符串
//查找在第count个begin字符串之后length长的字符串
uint16_t get_str(char *source, const char *begin, uint16_t count, uint16_t length, char *out)
{
    uint16_t i = 0;
    uint16_t len1 = 0;
    uint16_t index1 = 0;
    len1 = find_str((uint8_t *)source, (uint8_t *)begin, count, index1);
    if(len1 != 0)
    {
        for(i = 0; i < length; i++)
            out[i] = source[index1 + len1 + i];
        out[i] = '\0';
    }
    return length;
}
//查找字符串
//查找从开始到length中间的字符串
uint16_t get_str(char *source, char *out, uint16_t length)
{
    uint16_t i = 0;
    for (i = 0 ; i < length ; i++)
    {
        out[i] = source[i];
    }
    out[i] = '\0';
    return length;
}
//查找字符串
//查找从开始到第count个end之间的字符串
uint16_t get_str(char *source, const char *end, uint16_t count, char *out)
{
    uint16_t i;
    uint16_t len;
    uint16_t index = 0;
    uint16_t length = 0;
    len = find_str((uint8_t *)source, (uint8_t *)end, count, index);
    length = index;
    if((len != 0) && (len != 0))
    {
        for( i = 0; i < length; i++)
            out[i] = source[i];
        out[i] = '\0';
    }
    return length;
}


