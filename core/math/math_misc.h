
#ifndef __MATH_MISC_H
#define __MATH_MISC_H


#ifdef __cplusplus
 extern "C" {
#endif
     

#define min3v(v1, v2, v3)   ((v1)>(v2)? ((v2)>(v3)?(v3):(v2)):((v1)>(v3)?(v3):(v2)))
#define max3v(v1, v2, v3)   ((v1)<(v2)? ((v2)<(v3)?(v3):(v2)):((v1)<(v3)?(v3):(v1)))
#define min(v1, v2 )        ((v1)<(v2)? (v1):(v2)
#define max(v1, v2 )        ((v1)>(v2)? (v1):(v2)
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define round(x)            ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
#define radians(deg)        ((deg)*DEG_TO_RAD)
#define degrees(rad)        ((rad)*RAD_TO_DEG)
#define sq(x)               ((x)*(x))

#ifdef __cplusplus
}
#endif

#endif
