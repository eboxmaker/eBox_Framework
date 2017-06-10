#if !defined(COUNTDOWN_H)
#define COUNTDOWN_H

#include "ebox.h"

class Countdown
{
public:
    Countdown()
    {
        interval_end_ms = 0L;
    }

    Countdown(int ms)
    {
        countdown_ms(ms);
    }

    bool        expired()
    {
        return (interval_end_ms > 0L) && (millis() >= interval_end_ms);
    }

    void        countdown_ms(unsigned long ms)
    {
        interval_end_ms = millis() + ms;
    }

    void        countdown(int seconds)
    {
        countdown_ms((unsigned long)seconds * 1000L);
    }

    int        left_ms()
    {
        return interval_end_ms - millis();
    }

private:
    uint64_t        interval_end_ms;
};

#endif
