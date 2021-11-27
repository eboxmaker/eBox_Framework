#ifndef __BASICRTC_H
#define __BASICRTC_H

#include "string.h"
#include "DateTime.h"


extern const unsigned char day_code1[];
extern const unsigned int day_code2[];
extern const unsigned char year_code[];
extern const unsigned char  table_week[]; //���������ݱ�
extern const  char  *sky[];//���
extern const  char  *earth[];//��֧
extern const  char  *zodiac_table[12];
extern const  char  *monthcode[];//ũ���·�
extern const float CENTURY_ARRAY[2][24];
extern const char *JieQiStr[];


        
class BasicRtc
{
public:
    BasicRtc(){dateTime = DateTime();};
    virtual void    begin()  = 0;
    virtual void    begin(DateTime &dt)  = 0;
    virtual void    update()  = 0;
    virtual void    set(DateTime &dt) = 0;
    virtual DateTime    now() = 0;

#if DATETIME_USE_PRINT
    void            print(Stream &uart);            
#endif
    
public:
    DateTime      dateTime;
};


class ChinaCalendar
{
    public:

        typedef struct 
        {
            uint8_t month;
            uint8_t date;
            String name;
        }Festival_t;

    public:
        ChinaCalendar(){};
        
        void set(DateTime &dtSun);
        DateTime get_moon();
        DateTime get_sun();
            
        String get_year_str();
        String get_month_str();
        String get_date_str();
        String get_str();
            
        uint8_t get_jieqi_mday(DateTime &dt);
        String get_jieqi_str(DateTime &dt);
            

        uint8_t get_sky_earth_year(uint16_t year);//��ȡ��ɵ�֧��
        uint8_t get_zodiac();//��ȡ��Ф����ֵ0-11
        String get_zodiac_str();//��ȡ��Ф�ַ���

        
        String get_fastival();
#if DATETIME_USE_PRINT
        void print(Stream &uart);
#endif

    private:
        
        uint8_t GetMoonDay(unsigned char month_p,unsigned short table_addr);
        int spacil_year_offset(DateTime dt);
        DateTime moon_dt;
        DateTime sun_dt;
//    
};
extern const ChinaCalendar::Festival_t c_festival_table[9];

#endif
