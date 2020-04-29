#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "scheduler.h"
#include "process_control.h"

/*
CMP function:
Compares the processes pointed by a and b, which a and b are the type "Process *. "Process" is a user defined structure in "process_control.h".  
CMP compares the two processes by their ready time, if their ready time is the same, compare their names.
*/

int CMP(const void *a, const void *b){
	Process *c = (Process *)a;
	Process *d = (Process *)b;
	
	if( c->ready_time < d->ready_time )
		return -1;
	else if( c->ready_time > d->ready_time )
		return 1;
	else 
		return strcmp(c->name, d->name);
}

/*
main function:
1. Read input
2. Sort the processes by their ready time
3. Call corresponding scheduler
*/

int main(){
    /* SP used for scheduling policy FIFO, RR, SJF, PSFJ */
	char SP[8];
    /* n = number of processes */
	int n;
    /* read scheduling policy */
	scanf("%s", SP);
    /* read number of processes */
	scanf("%d", &n);
    /* allocate memory for n "Process" which we use for book-keeping */
	Process* proc = malloc(n * sizeof(Process));
    /* loop through each "Process" struct allocated */
	for(int i = 0 ; i < n ; i++){
        /* assign tmp to each 'Process' */
		Process *tmp = (proc + i);
        /* read the process name, ready time, execution time 
           and put it into the corresponding Process struct */
		scanf("%s%d%d", tmp -> name, &(tmp -> ready_time), &(tmp -> exec_time));
        /* initialize pid to -1 */
		tmp -> pid = -1; // initialize pid
        /* create a pipe between pipe_fd */
		if(pipe((tmp -> pipe_fd)) < 0){
			perror("error: pipe");
			exit(1);
		}
	}

    /* sort the processes in ready time increase order */
	qsort(proc, n, sizeof(Process), CMP);
    /* assign the main process to cpu 0 */
	assign_core(getpid(), SCHED_CORE);
	
/* this makes the process print out data of the
   emulated processes if we define PRINT_PROC */
#ifdef PRINT_PROC
	for(int i = 0 ; i < n ; i++)
		printf("%s %d %d\n", proc[i].name, proc[i].ready_time, proc[i].exec_time);
#endif

    /* schedule the emulated processes according to 
       the policy input */
	if(strcmp(SP, "FIFO") == 0){
		scheduler_FIFO(proc, n);
	}
	else if(strcmp(SP, "RR") == 0){
		scheduler_RR(proc, n);
	}
	else if(strcmp(SP, "SJF") == 0){
		scheduler_SJF(proc, n);
	}
	else if(strcmp(SP, "PSJF") == 0){
		scheduler_PSJF(proc, n);
	}
	else{
		printf("No such scheduling policy.\n");
	}

	return 0;
}
