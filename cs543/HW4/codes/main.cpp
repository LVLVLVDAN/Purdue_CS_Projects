
#include <iostream>
#include "Asim.h"
#include <iomanip>
#include <math.h>



int sim();
int cust();
facility f1;
int count1= 10;
double sSeed =  1234567;
bool end1;

double uniform (double *dseed);
double expon(double *dseed, float xm);

// user interface program. this file is used in order to define a CSIM simulation program. 

int sim() {
  
    std::cout << "Hello, I am simulator\n";
    int numCust = 10;
    while(numCust--){
        hold(10);
        create(cust);
    }
    wait(&end1);
    std::cout << "Simulation is Finished\n";
    report();
    return 0;
}

int cust() {
    std::cout << "I am a Customer\n";
    reserve(f1);
    hold(8);
    release(f1);
    count1--;
    std::cout<<count1<<"\n";
    if(count1 == 0){
        std::cout<<" All of the Customers got service \n";
        set(&end1);
    }
    return 0;
}
