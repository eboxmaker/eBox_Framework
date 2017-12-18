#ifndef __FILTERS_H__

#define __FILTERS_H__

#include "ebox.h"

class LpfRc
{
    public:
        LpfRc(float bandWidthInHz,uint16_t sampleFreq)
        {
            alpha = 0.01;
            last_output = 0;
            
        }
        float in(float input_val)
        {
            float out_put;
            out_put = alpha * input_val + (1 -alpha)* last_output;
            last_output = out_put;
        }

    private:
        double alpha;
        float last_output;
};
class FilterAverage
{
    public:
        FilterAverage(uint16_t window)
        {
            count = 0;
            this->window = window;
            history = (float *) ebox_malloc(window * sizeof(float));
        }
        bool sample(float input_val)
        {
            history[count++] = input_val;
            if(count == window)
            {
                float sum = 0;
                for(int i = 0; i < window ; i++)
                {
                    sum += history[i]; 
                    output = sum/(window);
                }                
                count = 0;
                return true;
            }
            else
                return false;
        }
        float out()
        {
            return  output;
        }
    private:
        float *history;
        uint16_t window;
        int count ;
        float output;

};
class FilterMidAverage
{
    public:
        FilterMidAverage(uint16_t window)
        {
            count = 0;
            if(window < 3) window = 3;
            this->window = window;
            history = (float *) ebox_malloc(window * sizeof(float));
        }
        bool sample(float input_val)
        {
            history[count++] = input_val;
            if(count == window)
            {
                float sum = 0;
                quick_sort(history,0,window - 1);
                for(int i = 1; i < window - 1; i++)
                {
                    sum += history[i]; 
                    output = sum/(window-2);
                }                
                count = 0;
                return true;
            }
            else
                return false;
        }
        float out()
        {
            return  output;
        }
    private:
        float *history;
        uint16_t window;
        int count ;
        float output;
};
class FilterMoveWindow
{
    public:
        FilterMoveWindow(uint16_t window)
        {
            count = 0;
            this->window = window;
            history = (float *) ebox_malloc(window * sizeof(float));
        }
        float in(float input_val)
        {
            float sum = 0;
            history[count++] = input_val;
            if(count == window) count = 0;
            
            for(int i = 0; i < window; i++)
            {
                sum += history[i]; 
            }
            return sum/window;

        }
    private:
        float *history;
        uint16_t window;
        int count ;
};

class FilterDelta
{
    public:
        FilterDelta(float delta)
        {
            this->delta = delta;
        }
        float in(float input_val)
        {
            float out;
            if(input_val >= last_input  )
            {
                if(input_val - last_input > delta)
                    out = last_input + delta;
                else
                    out = input_val;
            
            }
            else
            {
                if(  last_input - input_val > delta)
                    out = last_input - delta;
                else
                    out = input_val;

            }
            last_input = out;
            return out;
        }

    private:
        float delta;
        float last_input;
};
#endif