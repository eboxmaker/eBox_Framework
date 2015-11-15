
#include "util.h"


/**
@brief	CONVERT STRING INTO INTEGER
@return	a integer number
*/
uint16_t ATOI(
	char* str,	/**< is a pointer to convert */
	uint16_t base	/**< is a base value (must be in the range 2 - 16) */
	)
{
  unsigned int num = 0;
  while (*str !=0)
          num = num * base + C2D(*str++);
  return num;
}

uint32_t ATOI32(
	char* str,	/**< is a pointer to convert */
	uint16_t base	/**< is a base value (must be in the range 2 - 16) */
	)
{
  uint32_t num = 0;
  while (*str !=0)
          num = num * base + C2D(*str++);
  return num;
}


void itoa(uint16_t n,uint8_t str[5], uint8_t len)
{
  
  uint8_t i=len-1;

  memset(str,0x20,len);
  do{
  str[i--]=n%10+'0';
  
 }while((n/=10)>0);

 return;
}
/**
@brief	CONVERT STRING INTO HEX OR DECIMAL
@return	success - 1, fail - 0
*/
int ValidATOI(
	char* str, 	/**< is a pointer to string to be converted */
	int base, 	/**< is a base value (must be in the range 2 - 16) */
	int* ret		/**<  is a integer pointer to return */
	)
{
  int c;
  char* tstr = str;
  if(str == 0 || *str == '\0') return 0;
  while(*tstr != '\0')
  {
    c = C2D(*tstr);
    if( c >= 0 && c < base) tstr++;
    else    return 0;
  }
  
  *ret = ATOI(str,base);
  return 1;
}

/**
@brief	replace the specified character in a string with new character
*/ 
void replacetochar(
	char * str, 		/**< pointer to be replaced */
	char oldchar, 	/**< old character */
	char newchar	/**< new character */
	)
{
  int x;
  for (x = 0; str[x]; x++) 
    if (str[x] == oldchar) str[x] = newchar;	
}
/**
@brief	CONVERT CHAR INTO HEX
@return	HEX
  
This function converts HEX(0-F) to a character
*/
char C2D(
	uint8_t c	/**< is a character('0'-'F') to convert to HEX */
	)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	if (c >= 'a' && c <= 'f')
		return 10 + c -'a';
	if (c >= 'A' && c <= 'F')
		return 10 + c -'A';

	return (char)c;
}

uint16_t swaps(uint16_t i16)//�����ߵ��ֽ�
{
  uint16_t ret=0;
  ret = (i16 & 0xFF) << 8;
  ret |= ((i16 >> 8)& 0xFF);
  return ret;	
}

uint32_t swapl(uint32_t l32)//�����ߵ��ֽ�
{
  uint32_t ret=0;
  ret = (l32 & 0xFF) << 24;
  ret |= ((l32 >> 8) & 0xFF) << 16;
  ret |= ((l32 >> 16) & 0xFF) << 8;
  ret |= ((l32 >> 24) & 0xFF);
  return ret;
}

////get mid str
//void mid(int8_t* src, int8_t* s1, int8_t* s2, int8_t* sub)
//{
//	int8_t* sub1;
//	int8_t* sub2;
//	uint16_t n;

//  sub1=strstr(src,s1);
//  sub1+=strlen(s1);
//  sub2=strstr(sub1,s2);
//  n=sub2-sub1;
//  strncpy(sub,sub1,n);
//  sub[n]=0;
//}
void inet_addr_(unsigned char* addr,unsigned char *ip)
{
	int i;
//	u_long inetaddr = 0;
	char taddr[30];
	char * nexttok;
	char num;
	strcpy(taddr,(char *)addr);
	
	nexttok = taddr;
	for(i = 0; i < 4 ; i++)
	{
		nexttok = strtok(nexttok,".");
		if(nexttok[0] == '0' && nexttok[1] == 'x') num = ATOI(nexttok+2,0x10);
		else num = ATOI(nexttok,10);
		
		ip[i] = num;
		nexttok = NULL;
	}
}	



