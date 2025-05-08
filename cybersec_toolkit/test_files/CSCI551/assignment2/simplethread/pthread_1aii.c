#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sched.h>

#define NUM_THREADS 2   //change thread count to 2 
#define MAX_RANGE 1000000   //maximum number that is being summed up
#define ITERATIONS 5    //number of times that the counterThread function repeats 

typedef struct
{
    int threadIdx;
} threadParams_t;


// POSIX thread declarations and scheduling attributes
//
pthread_t threads[NUM_THREADS];
threadParams_t threadParams[NUM_THREADS];


void *counterThread(void *threadp)
{
    int sum=0, i;
    threadParams_t *threadParams = (threadParams_t *)threadp;

    for(i=1; i < MAX_RANGE; i++)
        sum=sum+i;
 
    printf("Thread idx=%d, sum[0...%d]=%d\n", 
           threadParams->threadIdx, MAX_RANGE, sum);

    return((void *)0);
}


int main (int argc, char *argv[])
{
   int i, j;

   for(j=0; j < ITERATIONS; j++){

        for(i=0; i < NUM_THREADS; i++)
        {
            threadParams[i].threadIdx=i;

            pthread_create(&threads[i],   // pointer to thread descriptor
                            (void *)0,     // use default attributes
                            counterThread, // thread function entry point
                            (void *)&(threadParams[i]) // parameters to pass in
                            );

        }

        for(i=0;i<NUM_THREADS;i++){
            pthread_join(threads[i], NULL);
        }
        printf("\n");
   }

   printf("TEST COMPLETE\n");
}
