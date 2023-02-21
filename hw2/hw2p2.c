#include <fcntl.h>
#include <getopt.h>
#include <linux/types.h>
#include <sched.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/timerfd.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>

#define MY_PRIORITY (49)  // kernel is priority 50

// Timer Functions
// ===============
// Timer related structures and functions:
struct period_info {
    struct timespec next_period;        // sec, nano-sec
    long period_ns;
};


// Thread related Real-time functions
// ========================

static void periodic_task_init(struct period_info *pinfo);
static void inc_period(struct period_info *pinfo);
static void wait_rest_of_period(struct period_info *pinfo);


// Thread-1 to read from "first.txt"

void *FirstThd(){
	//Get string pointer from main 


	// Declare it as a real time task and pass the necessary params to the scheduler 
	struct sched_param param;
    param.sched_priority = MY_PRIORITY;
    if (sched_setscheduler(0, SCHED_FIFO, &param) == -1) {
        printf("Run the program as a sudo user\n");
 	    perror("sched_setscheduler failed, thread 1");
    	exit(20);
    }
	
	// Initialize the periodic Task and read line at a time from "First.txt"
	struct period_info pinfo;
	periodic_task_init(&pinfo);
	
	struct timespec tstart = {0,0};
    struct timespec tend = {0,0};

    while (1)
    {
        clock_gettime(CLOCK_MONOTONIC, &tstart);
        sleep(2);
        //system("clear");
        printf("Test\n");
        clock_gettime(CLOCK_MONOTONIC, &tend);
        printf("Time Elapsed %f ns\n", ((((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) - ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec))-2)*1.0e9);
        sleep(2);
    }
	//Exit pthread
	pthread_exit(0);
}



int main(void) 
{
	//Declare variables	
	pthread_t thrd1, thrd2, thrd3;
    
	pthread_create(&thrd1, NULL, &FirstThd, NULL);


	//Join pthreads and check to make sure they joined correctly
	pthread_join(thrd1, NULL);

	return 0;	
}


// Helper Functions
// ================

//Write a function to determine the starting time of the thread
static void periodic_task_init(struct period_info *pinfo)
{
        /* for simplicity, hardcoding a 1ms period */
        pinfo->period_ns = 1000000;
 
        clock_gettime(CLOCK_MONOTONIC, &(pinfo->next_period));
}

// Write a function to the determine the ending time of the thread based on the initialized time
static void inc_period(struct period_info *pinfo) 
{
        pinfo->next_period.tv_nsec += pinfo->period_ns;
 
        while (pinfo->next_period.tv_nsec >= 1000000000) {
                /* timespec nsec overflow */
                pinfo->next_period.tv_sec++;
                pinfo->next_period.tv_nsec -= 1000000000;
        }
}

// Write a function to sleep for the remaining time of the period after finishing the task
static void wait_rest_of_period(struct period_info *pinfo)
{
        inc_period(pinfo);
 
        /* for simplicity, ignoring possibilities of signal wakes */
        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &pinfo->next_period, NULL);
}