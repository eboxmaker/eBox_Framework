#include "FilterDerivative.h"
#include "ebox.h"
#include "math.h"
float FilterDerivative::input( float inVal ) {
  long thisUS = micros();
  float dt = 1e-6*float(thisUS - LastUS);   // cast to float here, for math
  LastUS = thisUS;                          // update this now
  
  Derivative = (inVal-LastInput) / dt;
    
  LastInput = inVal;
  return output();
}
  
float FilterDerivative::output() { return Derivative; }
  
void testFilterDerivative() {
  FilterDerivative der;
  
  while(true) {
    float t = 1e-6 * float( micros() );
    float value = 100*sin(2*PI*t);
    
    der.input(value);
    
    uart1.print( "\n" );
    uart1.print( value ); uart1.print( "\t");
    uart1.print( der.output() );
    
    delay_ms(10);
  }
}