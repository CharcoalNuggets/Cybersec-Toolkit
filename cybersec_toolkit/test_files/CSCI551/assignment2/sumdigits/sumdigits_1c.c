#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

#define COUNT  1000000 
#define NUM_THREADS 10 
#define SUBRANGE (COUNT/NUM_THREADS)

unsigned long long gsum[NUM_THREADS] = {0};

int main (int argc, char *argv[])
{
    
   long testsum=0; 

#pragma omp parallel num_threads(NUM_THREADS)
{
   int idx = omp_get_thread_num();  //needed to add fopenmp flag to the makefile
   int start = (COUNT / NUM_THREADS) * idx + 1;
   int end = (COUNT / NUM_THREADS) * (idx + 1);

   for(int i=start; i <= end; i++)
   {
       gsum[idx] += i;
   }
   printf("Thread %d finished.\n", idx);
}


//Expected output section can be reused
   printf("TEST COMPLETE: gsum[0]=%llu, gsum[1]=%llu, gsum[2]=%llu, gsum[3]=%llu, gsum[4]=%llu, gsum[5]=%llu, gsum[6]=%llu, gsum[7]=%llu, gsum[8]=%llu, gsum[9]=%llu, gsumall=%llu\n", 
          gsum[0], gsum[1], gsum[2], gsum[3], gsum[4], gsum[5], gsum[6], gsum[7], gsum[8], gsum[9], 
          (gsum[0]+gsum[1]+gsum[2]+gsum[3]+gsum[4]+gsum[5]+gsum[6]+gsum[7]+gsum[8]+gsum[9]));



   // Verfiy with single thread version and (n*(n+1))/2
   for(int i=1; i<=COUNT; i++)
       testsum = testsum + i;

   printf("TEST COMPLETE: testsum=%ld, [n[n+1]]/2=%ld\n", testsum, ((((long int)(COUNT))*((long int)(COUNT+1)))/2));
}
