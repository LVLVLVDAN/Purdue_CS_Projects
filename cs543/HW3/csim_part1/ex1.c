/* simulate an M/M/1 queue ( Preempt and Resume discipline/
  (an open queue with exponential service times and interarrival intervals)
*/

#include "csim.h"
#include <math.h>
#include <stdio.h>

#define	SVTM	1.0	/*mean of service time distribution */
#define IATM	4.5	/*mean of inter-arrival time distribution , Lambda1 (mean inter-arrival) + Lambda2 (mean inter-arrival)*/
#define Lambda1  2        // high priority 
#define Lambda2  2.5     // low priority 
#define NARS	2000	//number of arrivals to be simulated

FACILITY f;		/*pointer for facility */
EVENT done;		/*pointer for counter */
TABLE tbl;		/*pointer for table */
TABLE tbl2;
QTABLE qtbl;		/* for low priority customers*/
QTABLE qtbl2;      /* for high priority customers */
int cnt;		/*number of active tasks*/
void cust();
double expon(float mean);
double expon1(double *, float);
float emean = IATM;
float emean1 = SVTM;
float L1 = Lambda1;
float L2 = Lambda2;	
float p1;
float p2;

double seed = 1234567;
void sim()				/*1st process - named sim */
{   
  
	int i;
    
	set_model_name("M/M/1  Preempt and Resume ");
	create("sim");				/*required create statement*/

	f = facility("facility");		/*initialize facility*/
	set_servicefunc(f,pre_res);     /*Preempt  and Resume discipline */
	done = event("done");			/*initialize event*/
	tbl = table("resp tms high priority");		/*initialize table */
	tbl2 = table("resp tms low priority");
	qtbl = qhistogram("num in high priority ", 10l);	/*initialize qhistogram*/
    qtbl2 = qhistogram("num in low priority", 10l);	/*initialize qhistogram*/
	cnt = NARS; 	/*initialize cnt*/
	
	for(i = 1; i <= NARS; i++) {
		hold(expon(emean));		/*hold interarrival*/
		
		cust(i);				/*initiate process cust*/
		}
	wait(done);				/*wait until all done*/
	report();				/*print report*/					
	mdlstat();
}

void cust(n)				/*process customer*/
int n;
{
	TIME t1;
    p1 = L1/(L1 + L2);
	int rnd = random(0,1);
	create("cust");				/*required create statement*/
	
	
	if (rnd < p1)
	{
    set_priority(2);     /* high priority*/
	t1 = clock;				/*time of request */
	note_entry(qtbl);			/*note arrival */
	use(f,expon(emean1));		/*hold service time*/
	record(clock-t1, tbl);			/*record response time*/
	note_exit(qtbl);			/*note departure */
	}
    else 
	{
	  set_priority(1);   /* low priority */
	t1 = clock;				/*time of request */
	note_entry(qtbl2);			/*note arrival */
	use(f,expon(emean1));		/*hold service time*/
	record(clock-t1, tbl2);			/*record response time*/
	note_exit(qtbl2);			/*note departure */
	}
	cnt--;					/*decrement cnt*/
	if(cnt == 0)
		set(done);			/*if last arrival, signal*/
}


double expon(float mean)  /* Exponential variate generation function. */
{
    /* Return an exponential random variate with mean "mean". */

  

   return expon1(&seed,mean);
}
