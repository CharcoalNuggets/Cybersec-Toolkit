#include <stdio.h>
#include <string.h>
//#include <mpi.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <time.h>

// The 2 series used for computation of pi are documented well by Wikipedia
//
// https://en.wikipedia.org/wiki/Leibniz_formula_for_%CF%80
//
// The first series is simply 1 - 1/3 + 1/5 - 1/7 + 1/9 - ... = pi/4
//
// The second series is the summation of 2 / ((4n+1)(4n+3)) for n=0 to infinity
//

int main(int argc, char** argv)
{
  int idx;
  double local_sum=0.0, euler_sum = 0.0;
  unsigned int length;

  if(argc < 2)
  {
      printf("usage: piseriesreduce <series n>\n");
      exit(-1);
  }
  else
  {
      sscanf(argv[1], "%u", &length);
  }


  struct timespec start, end;
  clock_gettime(CLOCK_MONOTONIC_RAW, &start);



  // sum the sub-series for the rank for Leibniz's formula for pi/4
  /*
  for(idx = my_rank*sub_length; idx < (sub_length*(my_rank+1)); idx++)
  {
    local_sum += local_num / ((2.0 * (double)idx) + 1.0);
    local_num = -local_num;
  }*/

    #pragma omp parallel for reduction(+:local_sum)
    for(idx = 0; idx < length; idx++)
    {
        double term = (idx % 2 == 0 ? 1.0 : -1.0);
        local_sum += term / (2.0 * (double)idx + 1.0);
    }


  // sum the sub-series for the rank for Euler improved convergence of the Madhava-Leibniz's formula for pi/4
  /*
  for(idx = my_rank*sub_length; idx < (sub_length*(my_rank+1)); idx++)
  {
    euler_local_sum += 2.0 / (((4.0 * (double)idx) + 1.0) * (4.0 * (double)idx + 3.0));
  }
*/
    #pragma omp parallel for reduction(+:euler_sum)
    for(idx = 0; idx < length; idx++)
    {
        euler_sum += 2.0 / (((4.0 * (double)idx) + 1.0) * (4.0 * (double)idx + 3.0));
    }

    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)/1e9;

    printf("20 decimals of pi  = 3.14159265358979323846\n");
    printf("C math library pi  = %15.14lf\n", M_PI);
    printf("Madhava-Leibniz pi = %15.14lf, ppb error = %15.14lf\n", 4.0 * local_sum, 1e9 * (M_PI - 4.0 * local_sum));
    printf("Euler modified pi  = %15.14lf, ppb error = %15.14lf\n", 4.0 * euler_sum, 1e9 * (M_PI - 4.0 * euler_sum));
    printf("Elapsed time       = %f seconds\n", elapsed);


  return 0;
}
