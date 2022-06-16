#include "RtcMillis.h"

void RtcMillis::update()
{
    uint32_t rightNow = millis();
    uint32_t diff = rightNow - lastMillis;
    lastMillis = rightNow;
    dateTime.addMilliSeconds(diff);
};

void RtcMillis::set(DateTime &dt)
{
    uint64_t unix_timestamp = dt.getTimeStamp();
    dateTime.parse(unix_timestamp);
};

DateTime RtcMillis::now(){
    update();
    return dateTime;
}