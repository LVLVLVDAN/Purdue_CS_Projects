/* 
Traffic light simulation 
*/

#include "csim.h"
#include <stdio.h>

#define A	9	 /*mean of inter-arrival time distribution - it is converted*/
#define B   12   /*mean of inter-arrival time distribution - it is convereted */
#define green1  58
#define green2  45
#define red   55
#define NARS	100	/*number of arrivals to be simulated*/

FACILITY strt1;
FACILITY strt2;
 
EVENT done;		/*pointer for counter */
TABLE tbl;		/*pointer for table */
TABLE tbl2;
QTABLE qtbl;		/*pointer for qhistogram */
QTABLE qtbl2;
int cnt;		/*number of active tasks*/
void cust();
void A_to_B();
void B_to_A();
void traffic_light();

void sim()				/*1st process - named sim */
{
	int i;

	set_model_name("Traffic light");
	create("sim");				/*required create statement*/
	strt1 = facility("A to B");		/*initialize facility*/
	strt2 = facility("B to A");		/*initialize facility*/
	done = event("done");			/*initialize event*/
	tbl = table("resp tms A to B");		/*initialize table */
	tbl2 = table("resp tms B to A");		/*initialize table */
	qtbl = qhistogram("A", 10l);	/*initialize qhistogram*/
    qtbl2 = qhistogram("B", 10l);	/*initialize qhistogram*/
	cnt = NARS;				/*initialize cnt*/
	
	for(i = 1; i <= NARS; i++) {
	            hold(expntl(A));
				hold(expntl(B));
				A_to_B();              /* time for green 1*/
				traffic_light();       // red for both direction
				B_to_A();              // time for green 2
				traffic_light();        // red for both direction 
		}
	wait(done);				/*wait until all done*/
	report();				/*print report*/
	mdlstat();
}


// This function is used to simulate traffic from A to B
void A_to_B()
{
     TIME t1;
	 t1 = clock;
   create("A to B");
	 note_entry(qtbl);			/*note arrival */
     reserve(strt1);				/*reserve traffic light for A to B*/
	 hold(2);
	 hold(green1);		/*hold green for A to B*/
	
	release(strt1);
	record(clock-t1, tbl);			/*record response time*/
	note_exit(qtbl);			/*note departure */
			/*if last arrival, signal*/

}

// This function is used to simulate traffic from B to A
void B_to_A()
{
    TIME t2;
	t2 = clock;
    create("B to A");
	 note_entry(qtbl2);			/*note arrival */
	 reserve(strt1);				/*reserve traffic light B*/
	 hold(2);             // wait for 2 seconds 
	 hold(green2);		/*hold green for B*/
	 
	release(strt1);
	record(clock-t2, tbl2);			/*record response time*/
	note_exit(qtbl2);			/*note departure */

}
// This function is used to simulate both sides are red. 
void traffic_light()
{
    
      hold(55);                   //stay red for 55 seconds
	  cnt--;					/*decrement cnt*/
	if(cnt == 0)
		set(done);			/*if last arrival, signal*/
}


