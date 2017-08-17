
#ifndef __MATH_MISC_H
#define __MATH_MISC_H

#define min3v(v1, v2, v3)   ((v1)>(v2)? ((v2)>(v3)?(v3):(v2)):((v1)>(v3)?(v3):(v2)))
#define max3v(v1, v2, v3)   ((v1)<(v2)? ((v2)<(v3)?(v3):(v2)):((v1)<(v3)?(v3):(v1)))
#define min(v1, v2 )        ((v1)<(v2)? (v1):(v2)
#define max(v1, v2 )        ((v1)>(v2)? (v1):(v2)
#define round(x)            ((uint16_t)(x)+0.5)>(x)? ((uint16_t)(x)):((uint16_t)(x)+1) 
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

#endif
