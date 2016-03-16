\begin{verbatim}
/* simulate two CSIM processes passing control to one another
   One is "sim" and the other is "cust"
*/

#include "csim.h"
#include <stdio.h>

EVENT done;

float busy = 10;

void cust();


void sim()				/*1st process - named sim */
{

	set_model_name("Two Processes");
	create("sim");				/*required create statement*/

        done = event("done"); 

        printf("\n Thread Sim is born at time %lf\n",simtime());

        printf("\n What happens when we call Cust? Let's find out \n");

        printf("\n Thread Sim is yielding control to Thread Cust at time %lf \n",simtime());

        cust();

        printf("\n Thread Sim regains control from Cust at time %lf \n",simtime());  

        printf("\n Thread Sim is going to hold for %lf units at time %lf\n",busy,simtime());  
       
        hold (busy);

        printf("\n Thread Sim is back again at time %lf\n",simtime());

        printf("\n Thread Sim waits for Thread Cust to set a flag\n");

        wait(done);

        printf("\n Thread Sim is now leaving the system\n");
}   
 

void cust()				/*process customer*/
{
	create("cust");				/*required create statement*/

        printf("\n Hi There! I am Thread Cust!\n");

        printf("\n I was born at time %lf\n",simtime());

        printf("\n I am going to hold for %lf units at time %lf\n",busy,simtime());

        hold(busy);

        printf("\n I'm Thread Cust and I'm back at time %lf! \n",simtime());

        printf("\n Set a flag so Thread Sim can leave\n");

        set(done); 

        printf("\n I have nothing left to do and so I am leaving\n");
}

\end{verbatim}
