
#include <stdio.h>
#include <math.h>

#define SIZE 400

#define UNIF 1

#define EXPMEAN 10

double uniform(double *);
double expon(double *, float);


int main() {

  int i;
  float emean = EXPMEAN;
  double seed;
  double r;
FILE *fp;
     
   
  seed = 1234567; //set the seed only once, at the start.
   fp = fopen("results.dat", "w");

  for (i=0; i < SIZE; i++) {

    if (UNIF) {
      r = uniform(&seed);
    }
    else {
      r = expon(&seed,emean);
    }


  
         fprintf(fp, "\n Variate #%d    %lf", i,r);


  }
}



      
      
      
    

