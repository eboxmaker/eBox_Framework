#include "basicRtc.h"




void BasicRtc::set_dt(DateTime_t &dt)
{
    write_dt(dt);
}
void BasicRtc::set_dt_mb(DataU16_t *buf)
{
    DateTime_t dt;

    dt.year = buf[2].byte[1];
    dt.month = buf[2].byte[0];
    dt.date = buf[1].byte[1];
    dt.hour = buf[1].byte[0];
    dt.min = buf[0].byte[1];
    dt.sec = buf[0].byte[0];
    write_dt(dt);
}
void BasicRtc::set_dt_string(String &str)
{
    DateTime_t dt;
    String temp[10];
    int seek = 0;
    int last = 0;
    seek = str.indexOf("-",last);
    temp[0] = str.substring(last,seek);
    last = seek + 1;
    
    seek = str.indexOf("-",last);
    temp[1] = str.substring(last,seek);
    last = seek + 1;

    seek = str.indexOf(" ",last);
    temp[2] = str.substring(last,seek);
    last = seek + 1;
    
    seek = str.indexOf(":",last);
    temp[3] = str.substring(last,seek);
    last = seek + 1;

    seek = str.indexOf(":",last);
    temp[4] = str.substring(last,seek);
    last = seek + 1;

    temp[5] = str.substring(last);
    
    dt.year = temp[0].toInt();
    dt.month = temp[1].toInt();
    dt.date = temp[2].toInt();
    dt.hour = temp[3].toInt();
    dt.min = temp[4].toInt();
    dt.sec = temp[5].toInt();
    write_dt(dt);
}
void BasicRtc::set_dt_string(const char *buf)
{
    String str = buf;
    set_dt_string(str);
}

void BasicRtc::set_dt_buf(uint8_t *buf)
{
    DateTime_t dt;

    int i = 0; 
    dt.sec = buf[i++];
    dt.min = buf[i++];
    dt.hour = buf[i++];
    dt.date = buf[i++];
    dt.month = buf[i++];
    dt.year = buf[i++];
    
    write_dt(dt);

}
DateTime_t BasicRtc::get_dt()
{
    return dateTime;
}
String	BasicRtc::get_dt_string()
{
    String dt_str;
    char buf[40];
    sprintf(buf,"20%02d-%02d-%02d %02d:%02d:%02d",dateTime.year,dateTime.month,dateTime.date,\
        dateTime.hour,dateTime.min,dateTime.sec);
    dt_str = buf;
    return dt_str;

}
String	BasicRtc::get_date()
{
    String dt_str;
    char buf[20];
    sprintf(buf,"20%02d-%02d-%02d",dateTime.year,dateTime.month,dateTime.date);
    dt_str = buf;
    return dt_str;

}
String	BasicRtc::get_time()
{
    String dt_str;
    char buf[20];
    sprintf(buf,"%02d:%02d:%02d",dateTime.hour,dateTime.min,dateTime.sec);
    dt_str = buf;
    return dt_str;

}



void BasicRtc::get_date_only(char *buf)
{
    buf[0] = uint8_t( (time_buf[0] % 10) + 0x30);
    buf[1] = uint8_t( (time_buf[0] / 10) + 0x30);
    buf[2] = uint8_t( (time_buf[1] % 10) + 0x30);
    buf[3] = uint8_t( (time_buf[1] / 10) + 0x30);
    buf[4] = uint8_t( (time_buf[2] % 10) + 0x30);
    buf[5] = uint8_t( (time_buf[2] / 10) + 0x30);
    buf[6] = '\0';
}
void BasicRtc::get_time_only(char *buf)
{

    buf[0] = uint8_t( (time_buf[3] % 10) + 0x30);
    buf[1] = uint8_t( (time_buf[3] / 10) + 0x30);
    buf[2] = uint8_t( (time_buf[4] % 10) + 0x30);
    buf[3] = uint8_t( (time_buf[4] / 10) + 0x30);
    buf[4] = uint8_t( (time_buf[5] % 10) + 0x30);
    buf[5] = uint8_t( (time_buf[5] / 10) + 0x30);
    buf[6] = '\0';


}
void BasicRtc::dt_check(DateTime_t &dt)
{
    uint8_t max_date;
    limit(dt.sec,(uint8_t)0,(uint8_t)59);
    limit(dt.min,(uint8_t)0,(uint8_t)59);
    limit(dt.hour,(uint8_t)0,(uint8_t)23);
    limit(dt.date,(uint8_t)1,(uint8_t)31);
    limit(dt.month,(uint8_t)1,(uint8_t)12);
    limit(dt.year,(uint8_t)0,(uint8_t)99);
    max_date = get_max_days_in_month(dt.year,dt.month);
    if(dt.date >  max_date)
    {
        dt.date = max_date;
    }
    calculate_week(dt);

}

void BasicRtc::print(Uart &uart)
{
    uart.printf("=========system date time==========\r\n");
    uart.printf("20%02d-%02d-%02d %02d:%02d:%02d weed:%d\r\n",dateTime.year,dateTime.month,dateTime.date,dateTime.hour,dateTime.min,dateTime.sec,dateTime.week);
    uart.printf("===================================\r\n");

}
void BasicRtc::print(Uart &uart,DateTime_t &dt)
{
    uart.printf("20%02d-%02d-%02d %02d:%02d:%02d weed:%d\r\n",dt.year,dt.month,dt.date,dt.hour,dt.min,dt.sec,dt.week);

}





/*********************************************************************************************************
** ��������:��ũ��ת��(ֻ����1901-2099��)
** �䡡��:  dt     ����
** �䡡��:  cdt    ũ��
********************************************************************************************************/
DateTime_t ChinaCalendar::update_cdt(DateTime_t &dt)
{ 
	unsigned char temp1,temp2,temp3,month_p,yearH,yearL;	
	unsigned char flag_y;
	unsigned short temp4,table_addr;
    
	unsigned char month,date;

	yearH=20;	yearL=dt.year;
	month = dt.month;
    date = dt.date;
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
	cdt.year=yearL;
	cdt.month=month;
	cdt.date = date;
    
    cdt.hour = dt.hour;
    cdt.min = dt.min;
    cdt.sec = dt.sec;
    return cdt;
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

void ChinaCalendar::print(Uart &uart)
{
    String str="";
    uart.printf("20%02d-%02d-%02d %02d:%02d:%02d weed:%d\r\n",cdt.year,cdt.month,cdt.date,cdt.hour,cdt.min,cdt.sec,cdt.week);
    str += get_month_str();
    str += get_date_str();
    uart.printf("%s\r\n",str.c_str());

}
String ChinaCalendar::get_year_str()
{
	u8 SEyear;
	SEyear = get_sky_earth_year(cdt.year + 2000);
    String str = "";
    
    str += sky[SEyear%10];//  ��
    str += earth[SEyear%12];//  ��	
}

String ChinaCalendar::get_month_str()
{
    String str = "";
    str += monthcode[cdt.month - 1];
    str += "��";
    return str;
}
String ChinaCalendar::get_date_str()
{
    String str = "";
    switch(cdt.date/10)
    {
        case 0:
            str += "��";break;
        case 1:
            str += "ʮ";break;
        case 2:
            str += "إ";break;
        case 3:
            str += "��";break;
    }
    str += monthcode[(cdt.date - 1 ) %10];
    return str;
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
	u8 SEyear;
	SEyear = get_sky_earth_year(cdt.year + 2000);
    
    str += sky[SEyear%10];//  ��
    str += earth[SEyear%12];//  ��	
    str += get_zodiac_str();
    str += "�� ";
    
    str += get_month_str();// 
    str += get_date_str();// 
    return str;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��������:���빫�����ڵõ�����24�������� day<15�����ϰ��½���,��֮�����°���	
// �䡡��:  dt     ���� 
// �䡡��:  ������ݽ�����ƫ��ֵ																		  
/////////////////////////////////////////////////////////////////////////////////////////////////////////

int ChinaCalendar::spacil_year_offset(DateTime_t dt)
{
    int year =dt.year + 2000;
    
    uint8_t jq_index = (dt.month-1) * 2 ;   //��ý���˳����(0��23
	if(dt.date >= 15) jq_index++;           //�ж��Ƿ����ϰ���
    
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
uint8_t ChinaCalendar::get_jieqi_mday(DateTime_t &dt)
{
    uint16_t y = dt.year;
    uint8_t day;
    uint8_t jq_index = (dt.month-1) * 2 ;                             //��ý���˳����(0��23
	if(dt.date >= 15) jq_index++;                             //�ж��Ƿ����ϰ���
    
    //ע�⣺������3��1��ǰ������Ҫ��һ������L=[(Y-1)/4],
    //��ΪС�����󺮡���������ˮ������������С��3��1��,���� y = y-1 
    if(is_leap_year(dt.year + 2000))
    {
        if(dt.month < 3)
            y -= 1;
    }
    day = dt.year*0.2422 + CENTURY_ARRAY[1][jq_index] - y/4;
    day += spacil_year_offset(dt);
    return day;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// ��������:���빫�����ڵõ�24�����ַ���	
//          ��:GetJieQiStr(2007,02,08,str) ����str="����ˮ����11��"
// �䡡��:  dt     ���� 
// �䡡��:  str    ����24�����ַ�����ַ   15Byte																		  
/////////////////////////////////////////////////////////////////////////////////////////////////////////
String ChinaCalendar::get_jieqi_str(DateTime_t &_dt)
{
    DateTime_t dt = _dt;
    String str;
    uint8_t days = 0;
	u8 jq_mday,jq_index,max_days_in_month;
    
    jq_mday = get_jieqi_mday(dt);
	if(jq_mday==0)	return "";

	jq_index = (dt.month-1) *2 ;                             //��ý���˳����(0��23
	if(dt.date >= 15) jq_index++;                             //�ж��Ƿ����ϰ���

	if(dt.date==jq_mday)                                 //��������һ��������
	{
        str = JieQiStr[jq_index];
		return str;
	}
    str += "��";
    if(dt.date<jq_mday)                                  //�����������С�ڱ��µĽ�������
	{
        str +=JieQiStr[jq_index];
		days=jq_mday-dt.date;
	} 
	else                                            //����������ڴ��ڱ��µĽ�������
	{
        if((jq_index+1) >23)  return "";
        str += JieQiStr[jq_index + 1];
		if(dt.date < 15)
		{
            uint8_t temp = dt.date;
            dt.date = 15;
			jq_mday = get_jieqi_mday(dt);
            dt.date = temp;
			days = jq_mday - dt.date;
		}
		else                                        //����
		{
			max_days_in_month = get_max_days_in_month(dt.year,dt.month);

			if(++dt.month==13)	
                dt.month=1;
            uint8_t temp = dt.date;
            dt.date = 1;
			jq_mday = get_jieqi_mday(dt);//  GetJieQi(year,month,1,&JQdate);
            dt.date = temp;
			days = max_days_in_month - dt.date + jq_mday;
		}
	}
    str += days ;
    str += "��";
    return str;
}
uint8_t ChinaCalendar::get_zodiac()
{
    return get_sky_earth_year(cdt.year + 2000)%12;
}
String ChinaCalendar::get_zodiac_str()
{
    return zodiac_table[get_sky_earth_year(cdt.year + 2000)%12];
}







bool is_leap_year(uint16_t _year)
{
    if(_year % 4 == 0) //�����ܱ�4����
    {
        if(_year % 100 == 0)
        {
            if(_year % 400 == 0)return true; //�����00��β,��Ҫ�ܱ�400����
            else return false;
        }
        else return true;
    }
    else return false;
}
//ʹ�û�ķ����ɭ���㹫ʽ
//��һ�ºͶ��¿�����һ���ʮ���º�ʮ����
uint8_t calculate_week(DateTime_t &dt)
{
    uint8_t week;
    uint16_t year = dt.year + 2000;
    uint8_t month = dt.month;
    if(month == 1 || month == 2)
    {
        month += 12;
        year--;
    }
    dt.week = (dt.date + 2 * month + 3 * (month + 1) / 5 + year + year / 4 - year / 100 + year / 400) % 7 + 1;
    return dt.week;
}


uint32_t sec_in_day(DateTime_t &_dt)
{
    return _dt.hour * 3600 + _dt.min * 60 + _dt.sec;
}
uint32_t seconds_between_2_time(DateTime_t &dt1, DateTime_t &dt2)
{
    uint32_t temp_sec1, temp_sec2;

    temp_sec1 = sec_in_day(dt1);
    temp_sec2 = sec_in_day(dt2);
    if(temp_sec1 > temp_sec2)
        return temp_sec1 - temp_sec2;
    else
        return temp_sec2 - temp_sec1;
}
void sec_to_time(uint32_t sec_source, uint8_t &hour, uint8_t &min, uint8_t &sec)
{
    hour	= sec_source / 3600;
    min		= (sec_source % 3600) / 60;
    sec		= (sec_source % 3600) % 60;
}

//���ݸ���������������Ǹ���ĵڼ���
//1��1�����һ��
uint16_t day_in_year(DateTime_t &_dt)
{
    uint16_t temp_year;
    uint16_t temp_month;
    uint16_t temp_day;

    temp_year 	= _dt.year + 2000;
    temp_month	= _dt.month;
    temp_day    = _dt.date;


    uint8_t DAY[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if(is_leap_year(temp_year))
        DAY[1] = 29;
    for(uint8_t i = 0; i < temp_month - 1; ++i)
    {
        temp_day += DAY[i];
    }
    return temp_day;
}
uint8_t get_max_days_in_month(uint16_t year,uint8_t month)
{
    bool flag; 
    year += 2000;
    switch(month)
    {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            return 31;
        case 4:
        case 6:
        case 9:
        case 11:
            return 30;
        case 2:
            if(is_leap_year(year))
            {
                return 29;
            }
            else
            {
                return 28;
            }
    }
}

uint16_t	days_between_2_date(DateTime_t &dt_current, DateTime_t &dt_target)
{
    uint16_t temp_year_c, temp_year_t;
    uint16_t temp_month_c, temp_month_t;
    uint16_t temp_day_c, temp_day_t;

    temp_year_c = dt_current.year + 2000;
    temp_year_t = dt_target.year + 2000;

    temp_month_c = dt_current.month;
    temp_month_t = dt_target.month;

    temp_day_c = dt_current.date;
    temp_day_t = dt_target.date;

    if((temp_year_c == temp_year_t) && (temp_month_c == temp_month_t))
    {
        return temp_day_t > temp_day_c ? temp_day_t - temp_day_c : temp_day_c - temp_day_t;
    }
    else if(temp_year_c == temp_year_t)
    {
        uint16_t c_day, t_day;
        c_day = day_in_year(dt_current);
        t_day = day_in_year(dt_target);
        return t_day > c_day ? t_day - c_day : c_day - t_day;

    }
    else
    {
        if(temp_year_c > temp_year_t)
        {
            swap(&temp_year_c, &temp_year_t);
            swap(&temp_month_c, &temp_month_t);
            swap(&temp_day_c, &temp_day_t);
        }
        uint16_t d1, d2, d3;
        if(is_leap_year(temp_year_c))
        {
            d1 = 366 - day_in_year(dt_current);
        }
        else
            d1 = 365 - day_in_year(dt_current);
        d2 = day_in_year(dt_target);
        d3 = 0;
        for(int year = temp_year_c + 1; year < temp_year_t; year++)
        {
            if(is_leap_year(year))
                d3 += 366;
            else
                d3 += 365;
        }
        return d1 + d2 + d3;
    }
}

uint32_t get_unix_timestamp(DateTime_t &dt)
{
    DateTime_t dt0 = {0};
    dt0.month = 1;
    dt0.date = 1;
    uint32_t timestamp = 946656000;
    uint16_t diff_days;
    diff_days = days_between_2_date(dt0, dt);

    timestamp += seconds_between_2_time( dt0, dt) + (diff_days ) * 3600 * 24; //����ʱ��֮���������;
    return timestamp;
}

DateTime_t  unix_timestamp_to_dt(uint32_t stamp,uint8_t time_zone)
{
    DateTime_t dt;

    stamp += time_zone*SEC_PER_HOUR;
    dt = get_utc_dt(stamp);
    return dt;
}
DateTime_t  get_utc_dt(DateTime_t &dt,uint8_t time_zone)
{
    uint32_t stamp = get_unix_timestamp(dt);
    return get_utc_dt(stamp);
}

DateTime_t  get_utc_dt(uint32_t stamp)
{
    int year = 0;
    int month = 0;
    int day = 0;
    int hour = 0;
    int minute = 0;
    int second = 0;

    //��ʱ���ֵת����������ͨ���������ԱȽϷ��������ꡢ�¡��ա�
    int days = stamp/ SEC_PER_DAY;
    //���ʱ���ֵ���ꡣ
    int yearTmp = 0;
    int dayTmp = 0;
    //ʹ�üбƷ����� days ���а�����������
    for (yearTmp = UTC_BASE_YEAR; days > 0; yearTmp++) {
        dayTmp = (DAY_PER_YEAR + is_leap_year(yearTmp)); //��һ���ж����죿
        if (days >= dayTmp) //������������ yearTmp �������ʱ���ֵ�������������
        {
           days -= dayTmp;
        }
        else
        {
           break;
        }
    }
    year = yearTmp;
    
  //���ʱ���ֵ����
    int monthTmp = 0;
    for (monthTmp = 1; monthTmp < MONTH_PER_YEAR; monthTmp++) {
       dayTmp = get_max_days_in_month(year,monthTmp);
       if (days >= dayTmp) {
           days -= dayTmp;
       }
       else
       {
           break;
       }
    }
    month = monthTmp;
    
    day = days + 1;
    
    //ת�����롣
    int secs = stamp % SEC_PER_DAY;
    //���ʱ���ֵ��Сʱ����
    hour = secs / SEC_PER_HOUR;
    //���ʱ���ֵ�ķ�������
    secs %= SEC_PER_HOUR;
    minute = secs / SEC_PER_MIN;
    //���ʱ�������������
    second = secs % SEC_PER_MIN;

    DateTime_t dt;
    if(year < 2000)
        dt.year = 0;
    else
        dt.year = year - 2000;
    dt.month = month;
    dt.date = day;
    dt.hour = hour;
    dt.min = minute;
    dt.sec = second;
    
    return dt;
}


void add_one_day(DateTime_t &dt)
{
    switch(dt.month)
    {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            mon_1_3_5_7_8_10_12_add_one_day(dt);
            break;

        case 4:
        case 6:
        case 9:
        case 11:
            mon_4_6_9_11_add_one_day(dt);
            break;

        case 2:
            mon_2_add_one_day(dt);
            break;
    }
    if(dt.month > 12)
    {
        dt.year += 1;
        dt.month -= 12;
    }
    calculate_week(dt);
}



///////���������������˽�к���/////////////////////////////////////////////
void mon_1_3_5_7_8_10_12_add_one_day(DateTime_t &dt)
{
    dt.date += 1;
    if(dt.date > 31)
    {
        dt.date -= 31;
        dt.month += 1;
    }
}
void mon_4_6_9_11_add_one_day(DateTime_t &dt)
{
    dt.date += 1;
    if(dt.date > 30)
    {
        dt.date -= 30;
        dt.month += 1;
    }
}
void mon_2_add_one_day(DateTime_t &dt)
{
    dt.date += 1;
    if(is_leap_year(dt.year))
    {
        if(dt.date > 29)
        {
            dt.date -= 29;
            dt.month += 1;
        }
    }
    else
    {
        if(dt.date > 28)
        {
            dt.date -= 28;
            dt.month += 1;
        }
    }
}



DateTime_t  date_next_n_days(DateTime_t &dt,uint16_t days)
{
    DateTime_t dtTarget = dt;
    for(int i = 0; i < days; i++)
    {
        add_one_day(dtTarget);
    }
    return dtTarget;
}

DateTime_t  date_before_n_days(DateTime_t &dt,uint16_t days)
{
    DateTime_t dtTarget = dt;
    uint32_t stamp = get_unix_timestamp(dt);
    stamp -= days * SEC_PER_DAY;
    return unix_timestamp_to_dt(stamp,8);
}


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
