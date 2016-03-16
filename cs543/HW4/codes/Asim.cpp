#include "Asim.h"
#include <pthread.h>
#include "main.cpp"
#include <time.h>

using namespace std;

// A shared mutex
pthread_mutex_t mutex;

int (*mySim)() = sim;
int (*myCust)() = cust;

// Current thread struct
Entity C_Thread;

void *sim_function( void *ptr );
void *cust_function( void *ptr );

// main function in the library which is used in order to create sim and scheduler threads 
int main(int argc, const char * argv[]) {
    
    pthread_t mysim;
    int sim_id = 1;
    int sed_id = 2;
    
    int ret = pthread_create(&mysim,NULL,sim_function,(void*)sim_id);
    ret = pthread_create(&schedule,NULL,schedular,(void*) sed_id);
    
    C_Thread.begin = true;
    C_Thread.time = 0;
    C_Thread.process = mysim;
    pthread_join(mysim,NULL);
    return 0;
}
// scheduler function whic is used in order to pick an entity from calender (Customer or sim threads)
void *schedular(void *ptr)
{
    for(;true;)
    {
        this_thread::sleep_for(chrono::microseconds(800));
        pthread_mutex_lock(&run);
        if(cal.empty() == false)
        {
            
            Entity en = cal.top();
            C_Thread = en;
            pthread_t next = en.process;
            
            //Sim time is forwarded
            if(SimulationTime > en.time)
            {
                break;
            }
             cout<<SimulationTime<<std::endl;
            SimulationTime = en.time;
            // cout<<SimulationTime<<std::endl;
            cal.pop();
            
            if(en.begin == 0)
            {
                int ret = pthread_create(&en.process,NULL,cust_function,(void*)myCust);
                en.begin = 1;
            }
            else
            {
                done = 1;
                pthread_cond_signal(&run_cv);
            }
        }
    }
    return 0;
}

//sim function which call mysim function

void *sim_function(void *p)
{
    mySim();
    return 0;
}

// create is used for creating customers. After a customer is created it pushed into calender. 
void create(int (*cust)()){
    // Create function
    Entity en;
    
    en.time = SimulationTime;
    en.begin = false;
    
    cust_pointer = cust;
    cal.push(en);
}

void *cust_function( void *ptr )
{
    int (*cust)();
    cust = (int (*)())ptr;
    cust();
    return 0;
}
// hold function which is used in order to hold running of a customer. In this case customer thread is suspended and it pushed into calender till scheduler pick it out from calender.
void hold(double time){
    pthread_t p_self = pthread_self();
    Entity currentEntity;
    currentEntity.process = p_self;
    pthread_mutex_lock(&timeLock);
    currentEntity.begin = true;
    
    currentEntity.time = time + SimulationTime;
    
    pthread_mutex_unlock(&timeLock);
    
    cal.push(currentEntity);
    
    done = 0;
    pthread_mutex_lock(&run);
    while (done == 0)
        pthread_cond_wait(&(run_cv), &run);
    pthread_mutex_unlock(&run);
}

// reserve a facility 

void reserve(facility fac)
{
    if(fac.busy == 1)
    {
        reserveDone = 0;

        pthread_t         self;
        fac.q.push(self);
        
        pthread_mutex_lock(&reserve_lock);
        while (reserveDone == 0)
            pthread_cond_wait(&(reserve_cond), &reserve_lock);
        pthread_mutex_unlock(&reserve_lock);
        
    }
    else{
        
        fac.busy = true;
        
    }
}

// release a facility 
void release(facility f)
{
    reserveDone = 1;
    pthread_cond_signal(&reserve_cond);
    //wait for thread
    this_thread::sleep_for(std::chrono::seconds(1));
    
}

// set and wait functions which are used in order to determine all of the customers are served or not.
void set(bool *e)
{
    //set event to true
    *e = 1;
    pthread_cond_signal(&wait_cond);
    
}

void wait(bool *e)
{
    *e = 0;
    pthread_mutex_lock(&wait_m);
    for(;*e == 0;){
        pthread_cond_wait(&(wait_cond), &wait_m);
    }
    pthread_mutex_unlock(&wait_m);
    
    
}

// This function can be used in order to calculate some results such as waiting time or queue length. 

void  report(){
    
    std::cout<<"Report are as follows:\n";
    
}
