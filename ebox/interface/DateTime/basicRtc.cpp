#include "basicRtc.h"


#if DATETIME_USE_PRINT

void BasicRtc::print(Stream &uart)
{
    uart.printf("=========system date time==========\n");
    uart.printf("%04d-%02d-%02d %02d:%02d:%02d weed:%d\n",dateTime.year,dateTime.month,dateTime.day,dateTime.hour,dateTime.minute,dateTime.second,dateTime.dayOfWeek());
    uart.printf("===================================\n");

}
#endif



/*********************************************************************************************************
** ��������:��ũ��ת��(ֻ����1901-2099��)
** �䡡��:  dt     ����
** �䡡��:  moon_dt    ũ��
********************************************************************************************************/
void ChinaCalendar::set(DateTime &dtSun)
{ 
	unsigned char temp1,temp2,temp3,month_p,yearH,yearL;	
	unsigned char flag_y;
	unsigned short temp4,table_addr;
    
	unsigned char month,date;

    sun_dt = dtSun;
	yearH=dtSun.year/100;	
    yearL=dtSun.year%100;
	month = dtSun.month;
    date = dtSun.day;
	// ��λ���ݱ��ַ
	if(yearH==20)	table_addr=(yearL+100-1)*3;
	else  			table_addr=(yearL-1)*3;

	// ȡ���괺�����ڵĹ����·� 
	temp1=year_code[table_addr+2]&0x60;	
	temp1>>=5;

	// ȡ���괺�����ڵĹ����� 
	temp2=year_code[table_addr+2]&31; 

	// ���㵱�괺���뵱��Ԫ��������,����ֻ���ڹ���1�»�2�� 
	if(temp1==1) 	temp3=temp2-1; 
	else 			temp3=temp2+31-1; 

	// ���㹫�����뵱��Ԫ�������� 
	if (month<10) 	temp4=day_code1[month-1]+date-1;
	else  			temp4=day_code2[month-10]+date-1;
	// ��������´���2�²��Ҹ����2��Ϊ����,������1 
	if ((month>2)&&(yearL%4==0)) 	temp4++;

	// �жϹ������ڴ���ǰ���Ǵ��ں� 
	if (temp4>=temp3)
	{ 						
		temp4-=temp3;
		month=1;
		month_p=1;
							
		flag_y=0;
		if(GetMoonDay(month_p,table_addr)==0)	temp1=29; //С��29��
		else 									temp1=30; //��С30��
		// �����ݱ���ȡ����������·�,��Ϊ0����������� 
		temp2=year_code[table_addr]/16; 	
		while(temp4>=temp1)
		{
			temp4-=temp1;
			month_p++;
			if(month==temp2)
			{
				flag_y=~flag_y;
				if(flag_y==0)month++;
			}
			else month++;
			if(GetMoonDay(month_p,table_addr)==0)	temp1=29;
			else 									temp1=30;
		}
		date=temp4+1;
	}
	// �������ڴ���ǰʹ���������������� 
	else
	{ 						
		temp3-=temp4;
		if (yearL==0)
		{
			yearL=100-1;
			yearH=19;
		}
		else yearL--;
		table_addr-=3;
		month=12;
		temp2=year_code[table_addr]/16; 	
		if (temp2==0)	month_p=12; 
		else 			month_p=13; 

		flag_y=0;
		if(GetMoonDay(month_p,table_addr)==0)	temp1=29; 
		else 									temp1=30; 
		while(temp3>temp1)
		{
			temp3-=temp1;
			month_p--;
			if(flag_y==0)		month--;
			if(month==temp2)	flag_y=~flag_y;
			if(GetMoonDay(month_p,table_addr)==0)	temp1=29;
			else 									temp1=30;
		}
		date=temp1-temp3+1;
	}

//	*p++=yearH;
	moon_dt.year=yearL + yearH*100;
	moon_dt.month=month;
	moon_dt.day = date;
    
    moon_dt.hour = dtSun.hour;
    moon_dt.minute = dtSun.minute;
    moon_dt.second = dtSun.second;
    return;
}

DateTime ChinaCalendar::get_moon()
{
    return moon_dt;
}
   
DateTime ChinaCalendar::get_sun()
{
    return sun_dt;
}



/*�Ӻ���,���ڶ�ȡ���ݱ���ũ���µĴ��»�С��,�������Ϊ�󷵻�1,ΪС����0------------------*/
uint8_t ChinaCalendar::GetMoonDay(unsigned char month_p,unsigned short table_addr)
{
	switch (month_p)
	{
		case 1:
			if((year_code[table_addr]&0x08)==0)		return(0);
			else 									return(1); 
		case 2:
			if((year_code[table_addr]&0x04)==0)		return(0);
			else 									return(1);
		case 3:
			if((year_code[table_addr]&0x02)==0)		return(0);
			else 									return(1);
		case 4:
			if((year_code[table_addr]&0x01)==0)		return(0);
			else 								    return(1);
		case 5:
			if((year_code[table_addr+1]&0x80)==0)	return(0);
			else 									return(1);
		case 6:
			if((year_code[table_addr+1]&0x40)==0)	return(0);
			else 									return(1);
		case 7:
			if((year_code[table_addr+1]&0x20)==0)	return(0);
			else 									return(1);
		case 8:
			if((year_code[table_addr+1]&0x10)==0)	return(0);
			else 									return(1);
		case 9:
			if((year_code[table_addr+1]&0x08)==0)	return(0);
			else 									return(1);
		case 10:
			if((year_code[table_addr+1]&0x04)==0)	return(0);
			else 									return(1);
		case 11:
			if((year_code[table_addr+1]&0x02)==0)	return(0);
			else 									return(1);
		case 12:
			if((year_code[table_addr+1]&0x01)==0)	return(0);
			else 									return(1);
		case 13:
			if((year_code[table_addr+2]&0x80)==0)	return(0);
			else 									return(1);
	}
	return(0);
}


String ChinaCalendar::get_year_str()
{
	uint8_t SEyear;
	SEyear = get_sky_earth_year(moon_dt.year);
    String str = "";
    
    str += sky[SEyear%10];//  ��
    str += earth[SEyear%12];//  ��	
    return str;
}

String ChinaCalendar::get_month_str()
{
    String str = "";
    str += monthcode[moon_dt.month - 1];
    str += "��";
    return str;
}
String ChinaCalendar::get_date_str()
{
    String str = "";
    if(moon_dt.day <= 10)
        str += "��";
    else if(moon_dt.day < 20)
        str += "ʮ";
    else if(moon_dt.day < 30)
        str += "إ";
    else if(moon_dt.day < 40)
        str += "��";
    str += monthcode[(moon_dt.day - 1 ) %10];
    return str;
}
String ChinaCalendar::get_fastival()
{
    for(int i = 0; i < 9; i++)
    {
        if(moon_dt.month == c_festival_table[i].month && moon_dt.day == c_festival_table[i].date)
        {
            return c_festival_table[i].name;
        }
    }
    return "��";
    
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��������:GetSkyEarth
// ��������:���빫�����ڵõ�һ��������(ֻ����1901-2099��)
// �䡡��:  year        ������
// �䡡��:  ������																						   
/////////////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t ChinaCalendar::get_sky_earth_year(uint16_t year)
{
	uint8_t x;
	
	if(year>=1984)
	{
		year=year-1984;
		x=year%60;				
	}
	else
	{
		year=1984-year;
		x=60-year%60;
	}
	return x;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��������:GetChinaCalendarStr
// ��������:	
//          
// �䡡��:  ����str="�������� ʮ��إ��"																						  
/////////////////////////////////////////////////////////////////////////////////////////////////////////
String ChinaCalendar::get_str()
{
    String str = "";
	uint8_t SEyear;
	SEyear = get_sky_earth_year(moon_dt.year);
    
    str += sky[SEyear%10];//  ��
    str += earth[SEyear%12];//  ��	
    str += get_zodiac_str();
    str += "�� ";
    
    str += get_month_str();// 
    str += get_date_str();// 
    
    str += "  ���գ�";//
    str += get_fastival();// 

    return str;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��������:���빫�����ڵõ�����24�������� day<15�����ϰ��½���,��֮�����°���	
// �䡡��:  dt     ���� 
// �䡡��:  ������ݽ�����ƫ��ֵ																		  
/////////////////////////////////////////////////////////////////////////////////////////////////////////

int ChinaCalendar::spacil_year_offset(DateTime dt)
{
    int year =dt.year;
    
    uint8_t jq_index = (dt.month-1) * 2 ;   //��ý���˳����(0��23
	if(dt.day >= 15) jq_index++;           //�ж��Ƿ����ϰ���
    
    if(jq_index == 0 && year == 2019)//С�� 
    {
        return -1;
    }
    else if(jq_index == 1 && year == 2082)//�� 
    {
        return 1;
    }
//    else if(jq_index == 2 && year == 2082)//���� 
//    {
//        return 1;
//    }
    else if(jq_index == 3 && year == 2026)//��ˮ 
    {
        return -1;
    }
//    else if(jq_index == 4 && year == 2082)//����  
//    {
//        return 1;
//    }
    else if(jq_index == 5 && year == 2084)//����  
    {
        return 1;
    }
//    else if(jq_index == 6 && year == 2082)//���� 
//    {
//        return 1;
//    }
//    else if(jq_index == 7 && year == 2082)//����  
//    {
//        return 1;
//    }
//    else if(jq_index == 8 && year == 2082)//����  
//    {
//        return 1;
//    }
    else if(jq_index == 9 && year == 2008)//С��  
    {
        return 1;
    }
//    else if(jq_index == 10 && year == 2082)//â��     
//    {
//        return 1;
//    }
//    else if(jq_index == 11 && year == 2082)//����   
//    {
//        return 1;
//    }
    else if(jq_index == 12 && year == 2016)//С��  
    {
        return 1;
    }
//    else if(jq_index == 13 && year == 2082)//����  
//    {
//        return 1;
//    }
    else if(jq_index == 14 && year == 2002)//����  
    {
        return 1;
    }
//    else if(jq_index == 15 && year == 2082)//����  
//    {
//        return 1;
//    }
//    else if(jq_index == 16 && year == 2082)//��¶  
//    {
//        return 1;
//    }
//    else if(jq_index == 17 && year == 2082)//���  
//    {
//        return 1;
//    }
//    else if(jq_index == 18 && year == 2082)//��¶  
//    {
//        return 1;
//    }
    else if(jq_index == 19 && year == 2089)//˪��  
    {
        return 1;
    }
    else if(jq_index == 20 && year == 2089)//����  
    {
        return 1;
    }
//    else if(jq_index == 21 && year == 2082)//Сѩ
//    {
//        return 1;
//    }
//    else if(jq_index == 22 && year == 2082)//��ѩ 
//    {
//        return 1;
//    }
    else if(jq_index == 23 && year == 2021)//���� 
    {
        return -1;
    }
    
    
    
return 0;
    
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��������:���빫�����ڵõ�����24�������� day<15�����ϰ��½���,��֮�����°���	
//          ��:GetJieQiStr(2007,02,08,str) ����str[0]=4
// �䡡��:  dt     ���� 
// �䡡��:  ����������																			  
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/** 
* ���� num =[Y*D+C]-L���Ǵ�˵�е�����ͨ�ù�ʽ 
* ��ʽ����������ĺ�2λ��0.2422��C(����centuryValue)ȡ�����󣬼������� 
* ���������ռ��������������ֵ��0-23��
*/ 
uint8_t ChinaCalendar::get_jieqi_mday(DateTime &dt)
{
    uint16_t y = dt.year;
    uint16_t yeayH = dt.year/100;
    uint16_t yeayL = dt.year%100;
    uint8_t day = 0;
    if(yeayH != 20 ) return 0;
    if(yeayL > 99) return 0;
    uint8_t jq_index = (dt.month-1) * 2 ;                             //��ý���˳����(0��23
	if(dt.day >= 15) jq_index++;                             //�ж��Ƿ����ϰ���
    
    //ע�⣺������3��1��ǰ������Ҫ��һ������L=[(Y-1)/4],
    //��ΪС�����󺮡���������ˮ������������С��3��1��,���� y = y-1 
    if(is_leap_year(dt.year))
    {
        if(dt.month < 3)
            yeayL -= 1;
    }
    day = yeayL*0.2422 + CENTURY_ARRAY[1][jq_index] - yeayL/4;
    day += spacil_year_offset(dt);
    return day;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��������:���빫�����ڵõ�24�����ַ���	
//          ��:GetJieQiStr(2007,02,08,str) ����str="����ˮ����11��"
// �䡡��:  dt     ���� 
// �䡡��:  str    ����24�����ַ�����ַ   15Byte																		  
/////////////////////////////////////////////////////////////////////////////////////////////////////////
String ChinaCalendar::get_jieqi_str(DateTime &_dt)
{
    DateTime dt = _dt;
    String str;
    uint8_t days = 0;
	uint8_t jq_mday,jq_index,max_days_in_month;
    
    jq_mday = get_jieqi_mday(dt);
	if(jq_mday==0)	return "";

	jq_index = (dt.month-1) *2 ;                             //��ý���˳����(0��23
	if(dt.day >= 15) jq_index++;                             //�ж��Ƿ����ϰ���

	if(dt.day==jq_mday)                                 //��������һ��������
	{
        str = JieQiStr[jq_index];
		return str;
	}
    str += "��";
    if(dt.day<jq_mday)                                  //�����������С�ڱ��µĽ�������
	{
        str +=JieQiStr[jq_index];
		days=jq_mday-dt.day;
	} 
	else                                            //����������ڴ��ڱ��µĽ�������
	{
        jq_index++;
        jq_index %= 24;
        str += JieQiStr[jq_index];
		if(dt.day < 15)
		{
            uint8_t temp = dt.day;
            dt.day = 15;
			jq_mday = get_jieqi_mday(dt);
            dt.day = temp;
			days = jq_mday - dt.day;
		}
		else                                        //����
		{
			max_days_in_month = days_in_month(dt.year,dt.month);

			if(++dt.month==13)
            {                
                dt.month=1;
                dt.year++;
                if(dt.year > 99) return "";
            }
            uint8_t temp = dt.day;
            dt.day = 1;
			jq_mday = get_jieqi_mday(dt);//  GetJieQi(year,month,1,&JQdate);
			days = max_days_in_month - temp + jq_mday;
		}
	}
    str += days ;
    str += "��";
    return str;
}
uint8_t ChinaCalendar::get_zodiac()
{
    return get_sky_earth_year(moon_dt.year)%12;
}
String ChinaCalendar::get_zodiac_str()
{
    return zodiac_table[get_sky_earth_year(moon_dt.year)%12];
}

#if DATETIME_USE_PRINT

void ChinaCalendar::print(Stream &uart)
{
    String str="";
    
    uart.print("����:");
    sun_dt.print(uart);
    
    uart.print("����:");
    moon_dt.print(uart);
    
    str = get_str();
    uart.printf("%s\t",str.c_str());
    
    str = get_jieqi_str(sun_dt);            
    uart.printf("%s\n",str.c_str());

}
#endif



/*****************************����ת������***********************************/
const unsigned char year_code[597]={
                    0x04,0xAe,0x53,    //1901 0
                    0x0A,0x57,0x48,    //1902 3
                    0x55,0x26,0xBd,    //1903 6
                    0x0d,0x26,0x50,    //1904 9
                    0x0d,0x95,0x44,    //1905 12
                    0x46,0xAA,0xB9,    //1906 15
                    0x05,0x6A,0x4d,    //1907 18
                    0x09,0xAd,0x42,    //1908 21
                    0x24,0xAe,0xB6,    //1909
                    0x04,0xAe,0x4A,    //1910
                    0x6A,0x4d,0xBe,    //1911
                    0x0A,0x4d,0x52,    //1912
                    0x0d,0x25,0x46,    //1913
                    0x5d,0x52,0xBA,    //1914
                    0x0B,0x54,0x4e,    //1915
                    0x0d,0x6A,0x43,    //1916
                    0x29,0x6d,0x37,    //1917
                    0x09,0x5B,0x4B,    //1918
                    0x74,0x9B,0xC1,    //1919
                    0x04,0x97,0x54,    //1920
                    0x0A,0x4B,0x48,    //1921
                    0x5B,0x25,0xBC,    //1922
                    0x06,0xA5,0x50,    //1923
                    0x06,0xd4,0x45,    //1924
                    0x4A,0xdA,0xB8,    //1925
                    0x02,0xB6,0x4d,    //1926
                    0x09,0x57,0x42,    //1927
                    0x24,0x97,0xB7,    //1928
                    0x04,0x97,0x4A,    //1929
                    0x66,0x4B,0x3e,    //1930
                    0x0d,0x4A,0x51,    //1931
                    0x0e,0xA5,0x46,    //1932
                    0x56,0xd4,0xBA,    //1933
                    0x05,0xAd,0x4e,    //1934
                    0x02,0xB6,0x44,    //1935
                    0x39,0x37,0x38,    //1936
                    0x09,0x2e,0x4B,    //1937
                    0x7C,0x96,0xBf,    //1938
                    0x0C,0x95,0x53,    //1939
                    0x0d,0x4A,0x48,    //1940
                    0x6d,0xA5,0x3B,    //1941
                    0x0B,0x55,0x4f,    //1942
                    0x05,0x6A,0x45,    //1943
                    0x4A,0xAd,0xB9,    //1944
                    0x02,0x5d,0x4d,    //1945
                    0x09,0x2d,0x42,    //1946
                    0x2C,0x95,0xB6,    //1947
                    0x0A,0x95,0x4A,    //1948
                    0x7B,0x4A,0xBd,    //1949
                    0x06,0xCA,0x51,    //1950
                    0x0B,0x55,0x46,    //1951
                    0x55,0x5A,0xBB,    //1952
                    0x04,0xdA,0x4e,    //1953
                    0x0A,0x5B,0x43,    //1954
                    0x35,0x2B,0xB8,    //1955
                    0x05,0x2B,0x4C,    //1956
                    0x8A,0x95,0x3f,    //1957
                    0x0e,0x95,0x52,    //1958
                    0x06,0xAA,0x48,    //1959
                    0x7A,0xd5,0x3C,    //1960
                    0x0A,0xB5,0x4f,    //1961
                    0x04,0xB6,0x45,    //1962
                    0x4A,0x57,0x39,    //1963
                    0x0A,0x57,0x4d,    //1964
                    0x05,0x26,0x42,    //1965
                    0x3e,0x93,0x35,    //1966
                    0x0d,0x95,0x49,    //1967
                    0x75,0xAA,0xBe,    //1968
                    0x05,0x6A,0x51,    //1969
                    0x09,0x6d,0x46,    //1970
                    0x54,0xAe,0xBB,    //1971
                    0x04,0xAd,0x4f,    //1972
                    0x0A,0x4d,0x43,    //1973
                    0x4d,0x26,0xB7,    //1974
                    0x0d,0x25,0x4B,    //1975
                    0x8d,0x52,0xBf,    //1976
                    0x0B,0x54,0x52,    //1977
                    0x0B,0x6A,0x47,    //1978
                    0x69,0x6d,0x3C,    //1979
                    0x09,0x5B,0x50,    //1980
                    0x04,0x9B,0x45,    //1981
                    0x4A,0x4B,0xB9,    //1982
                    0x0A,0x4B,0x4d,    //1983
                    0xAB,0x25,0xC2,    //1984
                    0x06,0xA5,0x54,    //1985
                    0x06,0xd4,0x49,    //1986
                    0x6A,0xdA,0x3d,    //1987
                    0x0A,0xB6,0x51,    //1988
                    0x09,0x37,0x46,    //1989
                    0x54,0x97,0xBB,    //1990
                    0x04,0x97,0x4f,    //1991
                    0x06,0x4B,0x44,    //1992
                    0x36,0xA5,0x37,    //1993
                    0x0e,0xA5,0x4A,    //1994
                    0x86,0xB2,0xBf,    //1995
                    0x05,0xAC,0x53,    //1996
                    0x0A,0xB6,0x47,    //1997
                    0x59,0x36,0xBC,    //1998
                    0x09,0x2e,0x50,    //1999 294
                    0x0C,0x96,0x45,    //2000 297
                    0x4d,0x4A,0xB8,    //2001
                    0x0d,0x4A,0x4C,    //2002
                    0x0d,0xA5,0x41,    //2003
                    0x25,0xAA,0xB6,    //2004
                    0x05,0x6A,0x49,    //2005
                    0x7A,0xAd,0xBd,    //2006
                    0x02,0x5d,0x52,    //2007
                    0x09,0x2d,0x47,    //2008
                    0x5C,0x95,0xBA,    //2009
                    0x0A,0x95,0x4e,    //2010
                    0x0B,0x4A,0x43,    //2011
                    0x4B,0x55,0x37,    //2012
                    0x0A,0xd5,0x4A,    //2013
                    0x95,0x5A,0xBf,    //2014
                    0x04,0xBA,0x53,    //2015
                    0x0A,0x5B,0x48,    //2016
                    0x65,0x2B,0xBC,    //2017
                    0x05,0x2B,0x50,    //2018
                    0x0A,0x93,0x45,    //2019
                    0x47,0x4A,0xB9,    //2020
                    0x06,0xAA,0x4C,    //2021
                    0x0A,0xd5,0x41,    //2022
                    0x24,0xdA,0xB6,    //2023
                    0x04,0xB6,0x4A,    //2024
                    0x69,0x57,0x3d,    //2025
                    0x0A,0x4e,0x51,    //2026
                    0x0d,0x26,0x46,    //2027
                    0x5e,0x93,0x3A,    //2028
                    0x0d,0x53,0x4d,    //2029
                    0x05,0xAA,0x43,    //2030
                    0x36,0xB5,0x37,    //2031
                    0x09,0x6d,0x4B,    //2032
                    0xB4,0xAe,0xBf,    //2033
                    0x04,0xAd,0x53,    //2034
                    0x0A,0x4d,0x48,    //2035
                    0x6d,0x25,0xBC,    //2036
                    0x0d,0x25,0x4f,    //2037
                    0x0d,0x52,0x44,    //2038
                    0x5d,0xAA,0x38,    //2039
                    0x0B,0x5A,0x4C,    //2040
                    0x05,0x6d,0x41,    //2041
                    0x24,0xAd,0xB6,    //2042
                    0x04,0x9B,0x4A,    //2043
                    0x7A,0x4B,0xBe,    //2044
                    0x0A,0x4B,0x51,    //2045
                    0x0A,0xA5,0x46,    //2046
                    0x5B,0x52,0xBA,    //2047
                    0x06,0xd2,0x4e,    //2048
                    0x0A,0xdA,0x42,    //2049
                    0x35,0x5B,0x37,    //2050
                    0x09,0x37,0x4B,    //2051
                    0x84,0x97,0xC1,    //2052
                    0x04,0x97,0x53,    //2053
                    0x06,0x4B,0x48,    //2054
                    0x66,0xA5,0x3C,    //2055
                    0x0e,0xA5,0x4f,    //2056
                    0x06,0xB2,0x44,    //2057
                    0x4A,0xB6,0x38,    //2058
                    0x0A,0xAe,0x4C,    //2059
                    0x09,0x2e,0x42,    //2060
                    0x3C,0x97,0x35,    //2061
                    0x0C,0x96,0x49,    //2062
                    0x7d,0x4A,0xBd,    //2063
                    0x0d,0x4A,0x51,    //2064
                    0x0d,0xA5,0x45,    //2065
                    0x55,0xAA,0xBA,    //2066
                    0x05,0x6A,0x4e,    //2067
                    0x0A,0x6d,0x43,    //2068
                    0x45,0x2e,0xB7,    //2069
                    0x05,0x2d,0x4B,    //2070
                    0x8A,0x95,0xBf,    //2071
                    0x0A,0x95,0x53,    //2072
                    0x0B,0x4A,0x47,    //2073
                    0x6B,0x55,0x3B,    //2074
                    0x0A,0xd5,0x4f,    //2075
                    0x05,0x5A,0x45,    //2076
                    0x4A,0x5d,0x38,    //2077
                    0x0A,0x5B,0x4C,    //2078
                    0x05,0x2B,0x42,    //2079
                    0x3A,0x93,0xB6,    //2080
                    0x06,0x93,0x49,    //2081
                    0x77,0x29,0xBd,    //2082
                    0x06,0xAA,0x51,    //2083
                    0x0A,0xd5,0x46,    //2084
                    0x54,0xdA,0xBA,    //2085
                    0x04,0xB6,0x4e,    //2086
                    0x0A,0x57,0x43,    //2087
                    0x45,0x27,0x38,    //2088
                    0x0d,0x26,0x4A,    //2089
                    0x8e,0x93,0x3e,    //2090
                    0x0d,0x52,0x52,    //2091
                    0x0d,0xAA,0x47,    //2092
                    0x66,0xB5,0x3B,    //2093
                    0x05,0x6d,0x4f,    //2094
                    0x04,0xAe,0x45,    //2095
                    0x4A,0x4e,0xB9,    //2096
                    0x0A,0x4d,0x4C,    //2097
                    0x0d,0x15,0x41,    //2098
                    0x2d,0x92,0xB5,    //2099
};
//�²���������ũ������Ҫʹ�õ�
//�·����ݱ�
const unsigned char day_code1[9]={0x0,0x1f,0x3b,0x5a,0x78,0x97,0xb5,0xd4,0xf3};
const unsigned int day_code2[3]={0x111,0x130,0x14e};
const unsigned char  table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //���������ݱ�
const  char  *sky[10]=  {"��","��","��","��","��","��","��","��","��","��",};//���
const  char  *earth[12]={"��","��","��","î","��","��","��","δ","��","��","��","��",};//��֧
const  char  *zodiac_table[12]={"��","ţ","��","��","��","��","��","��","��","��","��","��"};//��Ф
const  char  *monthcode[12]={"һ","��","��","��","��","��","��","��","��","ʮ","��","��",};//ũ���·�

//����һ����ά���飬��һά����洢����20���͵Ľ���Cֵ��
//�ڶ�ά����洢����21���͵Ľ���Cֵ,0��23����
//���δ�����������ˮ...�󺮽�����Cֵ 
const float CENTURY_ARRAY[2][24] = 
{
    {4.6295,19.4599,6.3826,21.4155,5.59,20.888,6.318,21.86,6.5,22.2,7.928,
    23.65,8.35,23.95,8.44,23.822,9.098,24.218,8.218,23.08,7.9,22.6,6.11,20.84} 
    ,{
    5.4055,//С��
    20.12,   //��
    3.87,
    18.73,
    5.63,
    20.646,
    4.81,
    20.1,
    5.52,
    21.04,
    5.678,
    21.37,
    7.108,
    22.83, 
        
    7.5,
    23.13,
    7.646,
    23.042,
    8.318,
    23.438,//˪��
    7.438,//����
    22.36,//Сѩ
    7.18,//��ѩ
    21.94,//����
    }
};



//�Թ��������Ⱥ�����
const char *JieQiStr[24]=   
{
 // ����        �Ƕ�    ��������     ���� //
	"С��",     //285     1�� 6��
	"��",     //300     1��20��    29.5��
	"����",     //315     2�� 4��
	"��ˮ",     //330     2��19��    29.8��
	"����",     //345     3�� 6��
	"����",     //  0     3��21��    30.2��
	"����",     // 15     4�� 5��
	"����",     // 30     4��20��    30.7��
	"����",     // 45     5�� 6��
	"����",     // 60     5��21��    31.2��
	"â��",     // 75     6�� 6��
	"����",     // 90     6��21��    31.4��
	"С��",     //105     7�� 7��
	"����",     //120     7��23��    31.4��
	"����",     //135     8�� 8��
	"����",     //150     8��23��    31.1��
	"��¶",     //165     9�� 8��
	"���",     //180     9��23��    30.7��
	"��¶",     //195    10�� 8��
	"˪��",     //210    10��24��    30.1��
	"����",     //225    11�� 8��
	"Сѩ",     //240    11��22��    29.7��
	"��ѩ",     //255    12�� 7��
	"����"      //270    12��22��    29.5��
};
const ChinaCalendar::Festival_t c_festival_table[9] = 
{
{1,1,"����"},
{1,15,"Ԫ����"},
{4,5,"������"},
{5,5,"�����"},
{7,7,"��Ϧ��"},
{7,15,"��Ԫ��"},
{8,15,"�����"},
{9,9,"������"},
{12,30,"��Ϧ"},
};
