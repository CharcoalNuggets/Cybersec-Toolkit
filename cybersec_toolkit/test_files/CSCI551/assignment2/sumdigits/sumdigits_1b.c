#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define COUNT  1000000  //n=1,000,000 for 1b

#define NUM_THREADS 10 //1b requires 10 threads

// Note that often the "digit sum" rather than "sum of the digits" or "sum of numbers in a sequence" is defined as the sum of the digit in each 10's place, but
// that's not what we want to model here.  E.g., Wikipedia - https://en.wikipedia.org/wiki/Digit_sum
//
// What we want to model is an arithmetic series sum best referred to as the "sum or an arithmetic progression" - https://en.wikipedia.org/wiki/Arithmetic_progression
//
// This si what we mean summing numbers in the range 1...n, where we know based on series facts that sum(1...n) = n(n+1)/2
//
// We can now have threads sum sub-ranges of a series as a service and then have them add up  the result after a join so that sum(1...n) = sum(1...n/2-1) + sum(n/2...n-1) for
// example.
//
// This sample code provides a simple example of an arithmetic progression sum (sometimes called sum of the digits for simplicity since we know sum(0...9)=9(10)/2=45.
//
// It should techically be called a sum of a series of numbers in an arithmetic progression.
//50000000000

typedef struct
{
    int threadIdx;
} threadParams_t;


// POSIX thread declarations and scheduling attributes
pthread_t threads[NUM_THREADS]; //change to universal NUM_THREAD instead of magic number
threadParams_t threadParams[NUM_THREADS];

// Thread specific globals
//unsigned long long gsum[10]={0, 100000, 200000, 300000, 400000, 500000, 600000, 700000, 800000, 900000};
unsigned long long gsum[NUM_THREADS] = {0};

void *sumThread(void *threadp)
{
    int i, idx, start, end;
    threadParams_t *threadParams = (threadParams_t *)threadp;

    idx = threadParams->threadIdx;

    start = (COUNT / NUM_THREADS) * idx + 1;
    end = (COUNT / NUM_THREADS) * (idx + 1);

    for(i=start; i <= end; i++)
    {
        gsum[idx] += i;
        //printf("Increment %d for thread idx=%d, gsum=%ld\n", i, idx, gsum[idx]);
    }
    printf("Thread %d finished.\n", idx);
}

int main (int argc, char *argv[])
{
   long rc, testsum=0; 
   int i=0;

   for(i=0; i<NUM_THREADS; i++)
   {
      threadParams[i].threadIdx=i;
      pthread_create(&threads[i], (void *)0, sumThread, (void *)&(threadParams[i]));
   }

   for(i=0; i<NUM_THREADS; i++)
     pthread_join(threads[i], NULL);
     

   printf("TEST COMPLETE: gsum[0]=%llu, gsum[1]=%llu, gsum[2]=%llu, gsum[3]=%llu, gsum[4]=%llu, gsum[5]=%llu, gsum[6]=%llu, gsum[7]=%llu, gsum[8]=%llu, gsum[9]=%llu, gsumall=%llu\n", 
          gsum[0], gsum[1], gsum[2], gsum[3], gsum[4], gsum[5], gsum[6], gsum[7], gsum[8], gsum[9], 
          (gsum[0]+gsum[1]+gsum[2]+gsum[3]+gsum[4]+gsum[5]+gsum[6]+gsum[7]+gsum[8]+gsum[9]));



   // Verfiy with single thread version and (n*(n+1))/2
   for(i=1; i<=COUNT; i++)
       testsum = testsum + i;

   printf("TEST COMPLETE: testsum=%ld, [n[n+1]]/2=%ld\n", testsum, ((((long int)(COUNT))*((long int)(COUNT+1)))/2));
}
