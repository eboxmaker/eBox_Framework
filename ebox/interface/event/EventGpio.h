#ifndef __IOEVENT_H
#define __IOEVENT_H
#include "ebox_core.h"
#include "eventManager.h"

/** 创建EventGpio对象，并挂载事件回调函数高电平，低电平，上升沿，
  *下降沿，单击，释放，长按.不需要响应的事件不需要处理
  * 使用长按事件会自动禁用单击事件，且长按发生后不触发释放事件。但不影响上升沿和下降沿事件
  * 使用长按事件，可以和释放事件配合，当没有触发长按事件的时候，释放事件会被执行
  
  *长按支持单次触发Single和连续触发Continue模式。且连续触发事件间隔可设置
*/


//软件滤波时间ms
#define IO_EDGE_FILTER_COUNTS   0//默认不使用软件滤波
//长按事件触发事件
#define LONG_PRESS_COUNTS       2000//2000ms
//长按事件连续触发间隔
#define LONG_PRESS_INTERVAL       500//2000ms



class EventGpio : public Event
{
 public:
    typedef enum {
        Single = 0,
        Continue = 1
    }LongPressType_t;
    
    LongPressType_t long_press_type;
    

public:
    EventGpio(Gpio *io,uint8_t first_state,String _name = "null")
    {
        this->io = io;
        this->first_state = first_state;
        state = first_state;
        click_pushed = 0;
        click_released = 0;
        long_pressed = 0;
        
        event_high = NULL;
        event_low = NULL;
        event_pos_edge = NULL;
        event_neg_edge = NULL;
        event_click = NULL;
        event_release = NULL;
        event_long_press = NULL;
        long_press_type = Continue;
        this->name = _name;

    }
    void begin();
    virtual void loop(void);
    void (*event_high)(Object *sender);
    void (*event_low)(Object *sender);
    void (*event_pos_edge)(Object *sender);
    void (*event_neg_edge)(Object *sender);
    void (*event_click)(Object *sender);
    void (*event_release)(Object *sender);
    void (*event_long_press)(Object *sender);


private:
    Gpio *io;
    uint8_t state;
    uint32_t last_time;
    uint16_t long_pressed;
    uint8_t click_pushed;
    uint8_t click_released;
    uint8_t first_state;
    uint8_t changed;
    uint16_t long_press_triger_time;

} ;


#endif
