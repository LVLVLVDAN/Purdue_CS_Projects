
// Function returns a single Exponential variate with mean xm
// Function Expon requires Uniform.c


#include <stdio.h>
#include <math.h>

double uniform (double *dseed);

double expon1(double *dseed, float xm) {

      return( (-(xm) * log((double)uniform(dseed))) );
}
      
