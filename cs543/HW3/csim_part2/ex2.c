/* simulate A Production Company 

*/

#include "csim.h"
#include <stdio.h>

#define IATM	300.0	/*mean of inter-arrival time distribution */
#define NARS	100	/*number of arrivals to be simulated*/


FACILITY product;		/*pointer for facility */
FACILITY pack;
EVENT done;		/*pointer for counter */
TABLE tbl;		/*pointer for table */
TABLE tbl2;
TABLE s_tbl;
QTABLE qtbl;	/*pointer for qhistogram */
QTABLE qtbl2;
TABLE lost;
int cnt;		/*number of active tasks*/
int n_widgets;
int losts;
STORE s;
void  production();
void packing();
void make_storage_report();
FILE *fp;
void sim()				/*1st process - named sim */
{
	int i;
    
    fp = fopen("part2.out", "w");
    set_output_file(fp);
	set_model_name("Widget Company");
	create("sim");				/*required create statement*/
    s = storage("Backlog", 10);       /* storage define */
	pack = facility("pack");
	product = facility("product");
	done = event("done");			/*initialize event*/
	tbl = table("packing machine response time");		/*initialize table */
	tbl2 = table("production time response time");
	s_tbl = table("storage table");
	qtbl = qhistogram("num in packing", 10l);	/*initialize qhistogram*/
    qtbl2 = qhistogram("num in production", 10l);
	lost = table("Number of losts ");
	cnt = NARS;				/*initialize cnt*/
	for(i = 1; i <= NARS; i++) {
	     hold(expntl(IATM)); /*hold inter-arrival*/
		 long count = avail(s);
		if (avail(s) > 0)
		{
		 allocate(1, s);
		 production();
		// deallocate(1,s);
		}
		
		else 
		{
		 losts = losts + 1;            /// number of losts 
		 record(losts,lost);
		 deallocate(1,s);	 
		  
		}    
		
		 
		}
					
	report();				/*print report*/
	make_storage_report();
	mdlstat();
}


void  packing(n)				/*process customer*/
int n;
{
	TIME t1;
    create("packing");	
	/*required create statement*/
	t1 = clock;				/*time of request */
	note_entry(qtbl);			/*note arrival */
	reserve(pack);				/*reserve packing machine*/
	hold(90 + (50*n));		/*hold packing time*/
	release(pack);				/*release packing machine*/
	record(clock-t1, tbl);			/*record response time*/
	note_exit(qtbl);			/*note departure */

	
}

void production()
{
        
        TIME t2;
        t2 = clock;	
		create("production");
		allocate(1, s);
		record(1, s_tbl);
		note_entry(qtbl2);
		reserve(product);      /*reserve production unit - I have decided to design a facility for production since I wanted to have information about  		                             this unit however if you remove it the results will be the same*/
        hold(uniform(60,100));			/* production time*/
		release(product);                 // release production unit
		deallocate(1,s);           // production is finished deallocate storage 
		
		note_exit(qtbl2);
		double random = uniform(0,1);
		if (random <= 0.1)
		      n_widgets = 4;
		else if (random > 0.1 && random <= 0.5)
		      n_widgets = 5;
		else if (random >0.5 && random <= 0.8)
		      n_widgets = 6;
	    else if (random > 0.8)
		      n_widgets = 7; 
	    record(clock-t2, tbl2);
		packing(n_widgets);         // call packing machine after production 
		
		
		
}
void make_storage_report()
{
	long req_cnt, cap;
	double t, w;

	fprintf(fp, "\n\tStorage Report: %s\n", storage_name(s));
	cap = storage_capacity(s);
	fprintf(fp, "    capacity      %ld\n", cap);
	req_cnt = storage_request_cnt(s);
	fprintf(fp, "    amt           %.3f\n",
		 (double)storage_request_amt(s)/req_cnt);
	t = storage_time(s);
	fprintf(fp, "    util          %.3f\n", storage_busy_amt(s)/(t*cap));
	fprintf(fp, "    srv_tm        %.3f\n",
		(double)storage_number_amt(s)/req_cnt);
	w = storage_waiting_amt(s);
	fprintf(fp, "    qlen          %.3f\n", w/t);
	fprintf(fp, "    resp          %.3f\n",
		(double)w/req_cnt);
	fprintf(fp, "    completes     %ld\n", storage_release_cnt(s));
	fprintf(fp, "    queued        %ld\n", storage_queue_cnt(s));
}
	
