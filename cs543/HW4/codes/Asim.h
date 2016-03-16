
#ifndef __Asim__Asim__
#define __Asim__Asim__

#include <pthread.h>
#include <functional>
#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <queue>
#include <string.h>
#include <thread>
#include <chrono>
#include <queue>

using namespace std;
//Entity for the heap
typedef struct Entity {
    double  time;
    bool begin;
    pthread_t process;
    int (*customer)();
}
Entity;

//Sorting the heap
class Sort_T {
public:
    bool operator()(Entity& e1, Entity& e2)
    {
        if(e1.time > e2.time)
            return true;
        return false;
    }
};

priority_queue<Entity, std::vector<Entity>, Sort_T> cal;



typedef struct facility {
    std::queue<pthread_t> q;
    bool busy = false;
}
facility;

//Functions
void create(int (*cust)());
void testJoin();
void hold(double t);
void *schedular(void *ptr);


void release(facility);
void reserve(facility);
void wait(bool *Event);


void set(bool *Event);
int (*cust_pointer)();
void report();


extern int (*mySim)();
extern int (*myCust)();
extern Entity currentThread;
int customerID = 1;
int SimulationTime = 0;

//thread variables
pthread_t schedule;
pthread_mutex_t run;
pthread_mutex_t currentThread_lock;
pthread_cond_t run_cv;
pthread_mutex_t timeLock;
pthread_mutex_t reserve_lock;
pthread_cond_t reserve_cond;
pthread_cond_t wait_cond;
pthread_mutex_t wait_m;
int done = 0;
int reserveDone = 0;

#endif







