
// Function returns a single Exponential variate with mean xm
// Function Expon requires Uniform.c


#include <stdio.h>
#include <math.h>

double uniform (double *dseed);

float expon1(double *dseed, float xm) {

 //printf("seed %lf",*dseed);

      return( (-(xm) * log((float)(1-uniform(dseed)))) );
}
      
