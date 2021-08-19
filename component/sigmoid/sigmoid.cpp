#include "sigmoid.h"
#include <math.h>
#include <stdio.h>
#include "ebox_mem.h"

#define sigmoid_exp fastexp
#define e 2.7182818284
inline double fastexp(double x) {
	/* e^x = 1 + x + (x^2 / 2!) + (x^3 / 3!) + (x^4 / 4!) */

	double sum = 1 + x;
	double n = x;
	double d = 1;
	double i;

	for(i = 2; i < 100; i++) {
		n *= x;
		d *= i;
		sum += n / d;
	}

	return sum;
}

double sigmoid(double x) {
	return 1.00 / (1 + sigmoid_exp(0 - x));
//    return 1.00 / (1 + pow(e,x);
}





/*exp(90) throw error for Float size of 4 Bytes, so it must be cuted if the value is larger.*/
float expo(float y){
	if(y>80) y=80;
	return pow((float)e,(float)y);
}
/*This is Softmax function written in C*/
void Softmax(float *x,int n, float *out){
    
    float Sum=0;
    float *ex;
    ex = (float *)ebox_malloc(4*n);
    for(int i=0;i<n;i++)
    {
        ex[i] = expo(x[i]);
        Sum+=expo(x[i]);
    }
    
    for(int i = 0; i < n; i++)
        out[i] = ex[i]/Sum;
    return ;
}