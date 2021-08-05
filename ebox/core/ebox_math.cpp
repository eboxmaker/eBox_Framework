#include "ebox_math.h"

namespace eBoxMath{

double avarage(float *ptr, size_t size)
{
    double sum = 0;
    for(uint16_t i = 0; i < size; i++)
    {
        sum += ptr[i];
    }
    sum = sum/(float)size;
    return sum;
}


//方差 s= ∑(x-u)^2/N;;;
double variance(float *ptr, size_t size)
{
    double sum = 0;
    double avg = avarage(ptr,size);
    
    for(uint16_t i = 0; i < size; i++)
    {
        sum += (ptr[i] - avg) * ( ptr[i] - avg);
    }
    return sum/size;
}

//标准差(均方差) o = sqrt(var);
double stdev(float *ptr, size_t size)
{
    double temp = variance(ptr,size);
    return sqrt(temp);
}

};
