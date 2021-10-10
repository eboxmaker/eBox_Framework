#ifndef __HX711_H
#define __HX711_H

#include "ebox_core.h"
#include "filters.h"


class HX711 
{
    public:
        HX711(Gpio *sck, Gpio *dout);
        void begin();
        void loop();
        float get_raw();
    private:
        Gpio *_sck;
        Gpio *_dout; 
        uint32_t last;
        FilterMoveWindow *filter;
    
        uint32_t read();
        float raw;

};

#include "linear.h"
class Balance : public HX711
{
    public:
        Balance(Gpio *sck, Gpio *dout);
        void add_row(double y);
        void calibration();
        void set_zero();
    
        float get_weight();
    private:
        Linear *linear;
        double rate;
        double offset;
};

#endif
