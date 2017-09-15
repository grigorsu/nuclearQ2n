#include <stdio.h>

#ifndef _g_Math_
#define _g_Math_


struct gMath {

  double                            zero_if_nan(double in);

} gMath;


//------------------------------------------------------------------
// 
double gMath::zero_if_nan(double x) {
  return (x!=x) ? 0 : x;
};
//------------------------------------------------------------------
// 

#endif
