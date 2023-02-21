#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int main(void) {
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

    return 0;
}