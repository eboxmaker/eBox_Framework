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
** 功能描述:公农历转换(只允许1901-2099年)
** 输　入:  dt     公历
** 输　出:  moon_dt    农历
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
	// 定位数据表地址
	if(yearH==20)	table_addr=(yearL+100-1)*3;
	else  			table_addr=(yearL-1)*3;

	// 取当年春节所在的公历月份 
	temp1=year_code[table_addr+2]&0x60;	
	temp1>>=5;

	// 取当年春节所在的公历日 
	temp2=year_code[table_addr+2]&31; 

	// 计算当年春年离当年元旦的天数,春节只会在公历1月或2月 
	if(temp1==1) 	temp3=temp2-1; 
	else 			temp3=temp2+31-1; 

	// 计算公历日离当年元旦的天数 
	if (month<10) 	temp4=day_code1[month-1]+date-1;
	else  			temp4=day_code2[month-10]+date-1;
	// 如果公历月大于2月并且该年的2月为闰月,天数加1 
	if ((month>2)&&(yearL%4==0)) 	temp4++;

	// 判断公历日在春节前还是春节后 
	if (temp4>=temp3)
	{ 						
		temp4-=temp3;
		month=1;
		month_p=1;
							
		flag_y=0;
		if(GetMoonDay(month_p,table_addr)==0)	temp1=29; //小月29天
		else 									temp1=30; //大小30天
		// 从数据表中取该年的闰月月份,如为0则该年无闰月 
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
	// 公历日在春节前使用下面代码进行运算 
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



/*子函数,用于读取数据表中农历月的大月或小月,如果该月为大返回1,为小返回0------------------*/
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
    
    str += sky[SEyear%10];//  甲
    str += earth[SEyear%12];//  子	
    return str;
}

String ChinaCalendar::get_month_str()
{
    String str = "";
    str += monthcode[moon_dt.month - 1];
    str += "月";
    return str;
}
String ChinaCalendar::get_date_str()
{
    String str = "";
    if(moon_dt.day <= 10)
        str += "初";
    else if(moon_dt.day < 20)
        str += "十";
    else if(moon_dt.day < 30)
        str += "廿";
    else if(moon_dt.day < 40)
        str += "三";
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
    return "无";
    
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// 函数名称:GetSkyEarth
// 功能描述:输入公历日期得到一个甲子年(只允许1901-2099年)
// 输　入:  year        公历年
// 输　出:  甲子年																						   
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
// 函数名称:GetChinaCalendarStr
// 功能描述:	
//          
// 输　出:  返回str="己亥猪年 十月廿七"																						  
/////////////////////////////////////////////////////////////////////////////////////////////////////////
String ChinaCalendar::get_str()
{
    String str = "";
	uint8_t SEyear;
	SEyear = get_sky_earth_year(moon_dt.year);
    
    str += sky[SEyear%10];//  甲
    str += earth[SEyear%12];//  子	
    str += get_zodiac_str();
    str += "年 ";
    
    str += get_month_str();// 
    str += get_date_str();// 
    
    str += "  节日：";//
    str += get_fastival();// 

    return str;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
// 功能描述:输入公历日期得到本月24节气日期 day<15返回上半月节气,反之返回下半月	
// 输　入:  dt     公历 
// 输　出:  特殊年份节气的偏差值																		  
/////////////////////////////////////////////////////////////////////////////////////////////////////////

int ChinaCalendar::spacil_year_offset(DateTime dt)
{
    int year =dt.year;
    
    uint8_t jq_index = (dt.month-1) * 2 ;   //获得节气顺序标号(0～23
	if(dt.day >= 15) jq_index++;           //判断是否是上半月
    
    if(jq_index == 0 && year == 2019)//小寒 
    {
        return -1;
    }
    else if(jq_index == 1 && year == 2082)//大寒 
    {
        return 1;
    }
//    else if(jq_index == 2 && year == 2082)//立春 
//    {
//        return 1;
//    }
    else if(jq_index == 3 && year == 2026)//雨水 
    {
        return -1;
    }
//    else if(jq_index == 4 && year == 2082)//惊蛰  
//    {
//        return 1;
//    }
    else if(jq_index == 5 && year == 2084)//春分  
    {
        return 1;
    }
//    else if(jq_index == 6 && year == 2082)//清明 
//    {
//        return 1;
//    }
//    else if(jq_index == 7 && year == 2082)//谷雨  
//    {
//        return 1;
//    }
//    else if(jq_index == 8 && year == 2082)//立夏  
//    {
//        return 1;
//    }
    else if(jq_index == 9 && year == 2008)//小满  
    {
        return 1;
    }
//    else if(jq_index == 10 && year == 2082)//芒种     
//    {
//        return 1;
//    }
//    else if(jq_index == 11 && year == 2082)//夏至   
//    {
//        return 1;
//    }
    else if(jq_index == 12 && year == 2016)//小暑  
    {
        return 1;
    }
//    else if(jq_index == 13 && year == 2082)//大暑  
//    {
//        return 1;
//    }
    else if(jq_index == 14 && year == 2002)//立秋  
    {
        return 1;
    }
//    else if(jq_index == 15 && year == 2082)//处暑  
//    {
//        return 1;
//    }
//    else if(jq_index == 16 && year == 2082)//白露  
//    {
//        return 1;
//    }
//    else if(jq_index == 17 && year == 2082)//秋分  
//    {
//        return 1;
//    }
//    else if(jq_index == 18 && year == 2082)//寒露  
//    {
//        return 1;
//    }
    else if(jq_index == 19 && year == 2089)//霜降  
    {
        return 1;
    }
    else if(jq_index == 20 && year == 2089)//立冬  
    {
        return 1;
    }
//    else if(jq_index == 21 && year == 2082)//小雪
//    {
//        return 1;
//    }
//    else if(jq_index == 22 && year == 2082)//大雪 
//    {
//        return 1;
//    }
    else if(jq_index == 23 && year == 2021)//冬至 
    {
        return -1;
    }
    
    
    
return 0;
    
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
// 功能描述:输入公历日期得到本月24节气日期 day<15返回上半月节气,反之返回下半月	
//          如:GetJieQiStr(2007,02,08,str) 返回str[0]=4
// 输　入:  dt     公历 
// 输　出:  节气的日期																			  
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/** 
* 计算 num =[Y*D+C]-L这是传说中的寿星通用公式 
* 公式解读：年数的后2位乘0.2422加C(即：centuryValue)取整数后，减闰年数 
* 根据年月日计算出节气的索引值（0-23）
*/ 
uint8_t ChinaCalendar::get_jieqi_mday(DateTime &dt)
{
    uint16_t y = dt.year;
    uint16_t yeayH = dt.year/100;
    uint16_t yeayL = dt.year%100;
    uint8_t day = 0;
    if(yeayH != 20 ) return 0;
    if(yeayL > 99) return 0;
    uint8_t jq_index = (dt.month-1) * 2 ;                             //获得节气顺序标号(0～23
	if(dt.day >= 15) jq_index++;                             //判断是否是上半月
    
    //注意：凡闰年3月1日前闰年数要减一，即：L=[(Y-1)/4],
    //因为小寒、大寒、立春、雨水这两个节气都小于3月1日,所以 y = y-1 
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
// 功能描述:输入公历日期得到24节气字符串	
//          如:GetJieQiStr(2007,02,08,str) 返回str="离雨水还有11天"
// 输　入:  dt     公历 
// 输　出:  str    储存24节气字符串地址   15Byte																		  
/////////////////////////////////////////////////////////////////////////////////////////////////////////
String ChinaCalendar::get_jieqi_str(DateTime &_dt)
{
    DateTime dt = _dt;
    String str;
    uint8_t days = 0;
	uint8_t jq_mday,jq_index,max_days_in_month;
    
    jq_mday = get_jieqi_mday(dt);
	if(jq_mday==0)	return "";

	jq_index = (dt.month-1) *2 ;                             //获得节气顺序标号(0～23
	if(dt.day >= 15) jq_index++;                             //判断是否是上半月

	if(dt.day==jq_mday)                                 //今天正是一个节气日
	{
        str = JieQiStr[jq_index];
		return str;
	}
    str += "离";
    if(dt.day<jq_mday)                                  //如果今天日期小于本月的节气日期
	{
        str +=JieQiStr[jq_index];
		days=jq_mday-dt.day;
	} 
	else                                            //如果今天日期大于本月的节气日期
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
		else                                        //翻月
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
    str += "天";
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
    
    uart.print("阳历:");
    sun_dt.print(uart);
    
    uart.print("阴历:");
    moon_dt.print(uart);
    
    str = get_str();
    uart.printf("%s\t",str.c_str());
    
    str = get_jieqi_str(sun_dt);            
    uart.printf("%s\n",str.c_str());

}
#endif



/*****************************阳历转换阴历***********************************/
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
//下部分数据是农历部分要使用的
//月份数据表
const unsigned char day_code1[9]={0x0,0x1f,0x3b,0x5a,0x78,0x97,0xb5,0xd4,0xf3};
const unsigned int day_code2[3]={0x111,0x130,0x14e};
const unsigned char  table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //月修正数据表
const  char  *sky[10]=  {"甲","乙","丙","丁","戊","己","庚","辛","壬","癸",};//天干
const  char  *earth[12]={"子","丑","寅","卯","辰","巳","午","未","申","酉","戌","亥",};//地支
const  char  *zodiac_table[12]={"鼠","牛","虎","兔","龙","蛇","马","羊","猴","鸡","狗","猪"};//生肖
const  char  *monthcode[12]={"一","二","三","四","五","六","七","八","九","十","冬","腊",};//农历月份

//定义一个二维数组，第一维数组存储的是20世纪的节气C值，
//第二维数组存储的是21世纪的节气C值,0到23个，
//依次代表立春、雨水...大寒节气的C值 
const float CENTURY_ARRAY[2][24] = 
{
    {4.6295,19.4599,6.3826,21.4155,5.59,20.888,6.318,21.86,6.5,22.2,7.928,
    23.65,8.35,23.95,8.44,23.822,9.098,24.218,8.218,23.08,7.9,22.6,6.11,20.84} 
    ,{
    5.4055,//小寒
    20.12,   //大寒
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
    23.438,//霜降
    7.438,//立冬
    22.36,//小雪
    7.18,//大雪
    21.94,//冬至
    }
};



//以公历日期先后排序
const char *JieQiStr[24]=   
{
 // 名称        角度    公历日期     周期 //
	"小寒",     //285     1月 6日
	"大寒",     //300     1月20日    29.5天
	"立春",     //315     2月 4日
	"雨水",     //330     2月19日    29.8天
	"惊蛰",     //345     3月 6日
	"春分",     //  0     3月21日    30.2天
	"清明",     // 15     4月 5日
	"谷雨",     // 30     4月20日    30.7天
	"立夏",     // 45     5月 6日
	"夏满",     // 60     5月21日    31.2天
	"芒种",     // 75     6月 6日
	"夏至",     // 90     6月21日    31.4天
	"小暑",     //105     7月 7日
	"大暑",     //120     7月23日    31.4天
	"立秋",     //135     8月 8日
	"处暑",     //150     8月23日    31.1天
	"白露",     //165     9月 8日
	"秋分",     //180     9月23日    30.7天
	"寒露",     //195    10月 8日
	"霜降",     //210    10月24日    30.1天
	"立冬",     //225    11月 8日
	"小雪",     //240    11月22日    29.7天
	"大雪",     //255    12月 7日
	"冬至"      //270    12月22日    29.5天
};
const ChinaCalendar::Festival_t c_festival_table[9] = 
{
{1,1,"春节"},
{1,15,"元宵节"},
{4,5,"清明节"},
{5,5,"端午节"},
{7,7,"七夕节"},
{7,15,"中元节"},
{8,15,"中秋节"},
{9,9,"重阳节"},
{12,30,"除夕"},
};
