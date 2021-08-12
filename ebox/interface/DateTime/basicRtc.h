#ifndef __BASICRTC_H
#define __BASICRTC_H

#include "ebox_core.h"
#include "string.h"
#include "ebox_uart.h"
#include "DateTime.h"

extern const unsigned char day_code1[];
extern const unsigned int day_code2[];
extern const unsigned char year_code[];
extern const unsigned char  table_week[]; //月修正数据表
extern const  char  *sky[];//天干
extern const  char  *earth[];//地支
extern const  char  *zodiac_table[12];
extern const  char  *monthcode[];//农历月份
extern const float CENTURY_ARRAY[2][24];
extern const char *JieQiStr[];


        
class BasicRtc
{
public:
    BasicRtc(){dateTime = DateTime();};
    virtual void    begin()  = 0;
    virtual void    begin(DateTime &dt)  = 0;
    virtual void    loop()  = 0;
    virtual void    update()  = 0;
    virtual void            set(DateTime &dt) = 0;
    virtual DateTime   now() = 0;
    void            print(Uart &uart);            

public:
    DateTime      dateTime;

private:
//    virtual void        write_dt(DateTime_t &dt)  = 0;
//    virtual DateTime_t  read_dt()  = 0;
};

class RtcMillis :public BasicRtc
    {
public:
    RtcMillis(){};
    virtual void    begin(){};
    virtual void    begin(DateTime &dt){set(dt);};
    virtual void    loop(){update();};
    virtual void    update(){
        uint32_t rightNow = millis();
        uint32_t diff = rightNow - lastMillis;
        lastMillis = rightNow;
        dateTime.addMilliSeconds(diff);
    };
    virtual void    set(DateTime &dt){
        uint64_t unix_timestamp = dt.getTimeStamp();
        dateTime.parse(unix_timestamp);
    };
    virtual DateTime   now(){
        update();
        return dateTime;
    }
protected:
    uint32_t lastMillis;
};




class ChinaCalendar
{
//    public:

//        typedef struct 
//        {
//            uint8_t month;
//            uint8_t date;
//            String name;
//        }Festival_t;

//    public:
//        ChinaCalendar(){};
//            
//        DateTime_t update_cdt(DateTime_t &dt);
//        DateTime_t get_cdt();
//            
//        String get_str();
//            
//        uint8_t get_jieqi_mday(DateTime_t &dt);
//        String get_jieqi_str(DateTime_t &dt);
//            

//        uint8_t get_sky_earth_year(uint16_t year);//获取天干地支年
//        uint8_t get_zodiac();//获取生肖索引值0-11
//        String get_zodiac_str();//获取生肖字符串

//        String get_year_str();
//        String get_month_str();
//        String get_date_str();
//        
//        String get_fastival();
//        void print(Uart &uart);
//    private:
//        
//        uint8_t GetMoonDay(unsigned char month_p,unsigned short table_addr);
//        int spacil_year_offset(DateTime_t dt);
//        DateTime_t cdt;
//        String cdtStr;
//        String solar_terms;
//        String zodiac;
//    
};
//extern const ChinaCalendar::Festival_t c_festival_table[9];

#endif
