#ifndef __EBOX_MATH_H
#define __EBOX_MATH_H
#include "ebox_type.h"

namespace eBoxMath{
    
//限制某个数的上下界
template<typename T>
    
T max(T *ptr, uint16_t size)
{
    T temp;
    temp = ptr[0];
    for(uint16_t i = 1; i < size; i++)
    {
       if(ptr[i] > temp) temp = ptr[i] ;
    }
    return temp;
}    

template<typename T>
T min(T *ptr, uint16_t size)
{
    T temp;
    temp = ptr[0];
    for(uint16_t i = 1; i < size; i++)
    {
       if(ptr[i] < temp) temp = ptr[i] ;
    }
    return temp;
}    

//平均值
double avarage(float *ptr, size_t size);

//方差 s= ∑(x-u)^2/N;;;
double variance(float *ptr, size_t size);

//标准差(均方差) o = sqrt(var);
double stdev(float *ptr, size_t size);

};

#endif


